/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include <gtest/gtest.h>
#include <analysisdriver/test/AnalysisDriverFixture.hpp>

#include <analysisdriver/SimpleProject.hpp>
#include <analysisdriver/CurrentAnalysis.hpp>
#include <analysisdriver/AnalysisRunOptions.hpp>

#include <analysis/Analysis.hpp>
#include <analysis/Problem.hpp>
#include <analysis/Algorithm.hpp>
#include <analysis/DataPoint.hpp>
#include <analysis/DDACEAlgorithmOptions.hpp>
#include <analysis/DDACEAlgorithm.hpp>
#include <analysis/DesignOfExperimentsOptions.hpp>
#include <analysis/DesignOfExperiments.hpp>
#include <analysis/DiscretePerturbation.hpp>
#include <analysis/DiscreteVariable.hpp>
#include <analysis/NullPerturbation.hpp>
#include <analysis/RubyPerturbation.hpp>
#include <analysis/WorkflowStep.hpp>

#include <project/ProjectDatabase.hpp>
#include <project/AnalysisRecord.hpp>
#include <project/ProblemRecord.hpp>
#include <project/DataPointRecord.hpp>
#include <project/InputVariableRecord.hpp>
#include <project/DakotaAlgorithmRecord.hpp>
#include <project/DakotaAlgorithmRecord_Impl.hpp>
#include <project/FileReferenceRecord.hpp>
#include <project/WorkflowRecord.hpp>

#include <runmanager/lib/Job.hpp>
#include <runmanager/lib/RunManager.hpp>
#include <runmanager/Test/ToolBin.hxx>
#include <runmanager/lib/Workflow.hpp>
#include <runmanager/lib/WorkItem.hpp>

#include <ruleset/OSArgument.hpp>

#include <model/Model.hpp>
#include <model/Building.hpp>
#include <model/Building_Impl.hpp>

#include <utilities/core/FileReference.hpp>
#include <utilities/core/PathHelpers.hpp>
#include <utilities/sql/SqlFile.hpp>

#include <OpenStudio.hxx>
#include <resources.hxx>

#include <boost/foreach.hpp>

using namespace openstudio;
using namespace openstudio::model;
using namespace openstudio::ruleset;
using namespace openstudio::runmanager;
using namespace openstudio::analysis;
using namespace openstudio::project;
using namespace openstudio::analysisdriver;

TEST_F(AnalysisDriverFixture,SimpleProject_Create) {
  openstudio::path projectDir = toPath("AnalysisDriverFixtureData");
  if (!boost::filesystem::exists(projectDir)) {
    boost::filesystem::create_directory(projectDir);
  }
  projectDir = projectDir / toPath("NewProject");
  boost::filesystem::remove_all(projectDir);

  OptionalSimpleProject project = SimpleProject::create(projectDir);

  ASSERT_TRUE(project);

  EXPECT_TRUE(boost::filesystem::exists(projectDir));
  EXPECT_TRUE(boost::filesystem::is_directory(projectDir));
  EXPECT_TRUE(boost::filesystem::exists(projectDir / toPath("project.osp")));
  EXPECT_TRUE(boost::filesystem::exists(projectDir / toPath("run.db")));
  EXPECT_TRUE(boost::filesystem::exists(projectDir / toPath("project.log")));

  Analysis analysis = project->analysis();
  EXPECT_EQ(0,analysis.problem().numVariables());
  EXPECT_FALSE(analysis.algorithm());
  EXPECT_EQ(0u,analysis.dataPoints().size());

  AnalysisRecord analysisRecord = project->analysisRecord();
  EXPECT_EQ(0u,analysisRecord.problemRecord().inputVariableRecords().size());
  EXPECT_EQ(0u,analysisRecord.dataPointRecords().size());
}

TEST_F(AnalysisDriverFixture,SimpleProject_Open_IsNotPATProject) {
  openstudio::path projectDir;
  {
    // Create a non-PAT project
    // RETRIEVE PROBLEM
    Problem problem = retrieveProblem("Continuous",true,false);

    // DEFINE SEED
    Model model = model::exampleModel();
    openstudio::path p = toPath("./example.osm");
    model.save(p,true);
    FileReference seedModel(p);

    // CREATE ANALYSIS
    DDACEAlgorithmOptions algOptions(DDACEAlgorithmType::lhs);
    algOptions.setSamples(1);
    Analysis analysis("Non-PAT Project",
                      problem,
                      DDACEAlgorithm(algOptions),
                      seedModel);

    // RUN ANALYSIS
    ProjectDatabase database = getCleanDatabase("Non-PAT Project");
    AnalysisDriver analysisDriver(database);
    AnalysisRunOptions runOptions = standardRunOptions(analysisDriver.database().path().parent_path());
    CurrentAnalysis currentAnalysis = analysisDriver.run(analysis,runOptions);
    EXPECT_TRUE(analysisDriver.waitForFinished());

    projectDir = analysisDriver.database().path().parent_path();
  }

  // Open project
  OptionalSimpleProject project = SimpleProject::open(projectDir);
  ASSERT_TRUE(project);
  EXPECT_FALSE(project->isPATProject());
}

TEST_F(AnalysisDriverFixture,PATProject_CreateOpenAndSaveAs) {
  openstudio::path projectDir;
  {
    // Create a PAT project
    SimpleProject patProject = getCleanPATProject("PATProject_CreateOpenAndSaveAs");

    // Populate it
    patProject.analysis().setName("PAT Project: Create, Open, and Save As");

    Model model = model::exampleModel();
    openstudio::path p = toPath("./example.osm");
    model.save(p,true);
    FileReference seedModel(p);
    patProject.setSeed(seedModel);

    // (feature not in PAT, but not incompatible)
    DesignOfExperimentsOptions algOptions(DesignOfExperimentsType::FullFactorial);
    DesignOfExperiments algorithm(algOptions);
    patProject.analysis().setAlgorithm(algorithm);

    // Save it
    patProject.save();

    projectDir = patProject.projectDir();
  }

  // Open Project
  OptionalSimpleProject project = openPATProject(projectDir);
  ASSERT_TRUE(project);
  EXPECT_TRUE(project->isPATProject());

  // Save Project As
  openstudio::path newProjectDir = toPath("AnalysisDriverFixtureData") / toPath("PATProjectCreateOpenAndSaveAsCopy");
  boost::filesystem::remove_all(newProjectDir);
  ASSERT_TRUE(project->saveAs(newProjectDir));
  OptionalSimpleProject projectCopy = openPATProject(newProjectDir);
  ASSERT_TRUE(projectCopy);
  EXPECT_TRUE(project->isPATProject());
  EXPECT_TRUE(project->analysis().algorithm());
}

TEST_F(AnalysisDriverFixture,SimpleProject_InsertMeasure) {
  // create a new project
  SimpleProject project = getCleanSimpleProject("SimpleProject_InsertMeasure");

  // open a measure
  openstudio::path dir = resourcesPath() / toPath("/utilities/BCL/Measures/SetWindowToWallRatioByFacade");
  BCLMeasure measure = BCLMeasure::load(dir).get();

  // insert it into the project
  BCLMeasure projectMeasure = project.insertMeasure(measure);

  // verify that there is a project-specific copy
  EXPECT_NE(toString(measure.directory()),toString(projectMeasure.directory()));
  openstudio::path scriptsDir = project.projectDir() / toPath("scripts");
  EXPECT_EQ(toString(completeAndNormalize(scriptsDir)),
            toString(completeAndNormalize(projectMeasure.directory().parent_path())));
  EXPECT_EQ(measure.uuid(),projectMeasure.uuid());
  EXPECT_EQ(measure.versionUUID(),projectMeasure.versionUUID()); // DLM: should this be not equal?

  // verify that it can be retrieved from the project, but its arguments cannot
  ASSERT_TRUE(project.getMeasureByUUID(measure.uuid()));
  BCLMeasure temp = project.getMeasureByUUID(measure.uuid()).get();
  EXPECT_EQ(toString(projectMeasure.directory()),toString(temp.directory()));
  EXPECT_EQ(projectMeasure.uuid(),temp.uuid());
  EXPECT_EQ(projectMeasure.versionUUID(),temp.versionUUID());
  EXPECT_EQ(1u,project.measures().size());

  // use embedded Ruby to extract the arguments
  // (see AnalysisDriverFixture.cpp for interpreter set-up)
  OSArgumentVector args = argumentGetter->getArguments(projectMeasure,
                                                       project.seedModel(),
                                                       project.seedIdf());
  EXPECT_FALSE(args.empty());

  // register and subsequently retrieve those arguments
  project.registerArguments(measure,args); // doesn't matter which copy of the measure we use
  EXPECT_TRUE(project.hasStoredArguments(temp));
  OSArgumentVector argsCopy = project.getStoredArguments(projectMeasure);
  ASSERT_EQ(args.size(),argsCopy.size());
  for (int i = 0, n = args.size(); i < n; ++i) {
    EXPECT_EQ(args[i].name(),argsCopy[i].name());
    EXPECT_EQ(args[i].type(),argsCopy[i].type());
  }

  // use this measure to create a new variable/ruby perturbation
  // now it is important to use the copy in the project
  DiscreteVariable dv("New Measure Group",DiscretePerturbationVector());
  Problem problem = project.analysis().problem();
  problem.push(dv);
  // here, expect this test to pass.
  // in pat, if it fails, let user know and call problem.erase(dv).
  EXPECT_TRUE(problem.fileTypesAreCompatible(dv,
                                             projectMeasure.inputFileType(),
                                             projectMeasure.outputFileType()));
  EXPECT_TRUE(dv.push(RubyPerturbation(projectMeasure)));
}

TEST_F(AnalysisDriverFixture,SimpleProject_UpdateMeasure) {
  // create a new project
  SimpleProject project = getCleanSimpleProject("SimpleProject_UpdateMeasure");
  Problem problem = project.analysis().problem();

  // insert a measure into the project, extract and register its arguments
  openstudio::path measuresDir = resourcesPath() / toPath("/utilities/BCL/Measures");
  openstudio::path dir = measuresDir / toPath("SetWindowToWallRatioByFacade");
  BCLMeasure measure = BCLMeasure::load(dir).get();
  BCLMeasure projectMeasure = project.insertMeasure(measure);
  OSArgumentVector args = argumentGetter->getArguments(projectMeasure,
                                                       project.seedModel(),
                                                       project.seedIdf());
  project.registerArguments(projectMeasure,args);
  EXPECT_EQ(1u,project.measures().size());

  // use the measure to create a new variable/ruby perturbation
  DiscreteVariable dv("New Measure Group",DiscretePerturbationVector());
  EXPECT_TRUE(problem.push(dv));
  RubyPerturbation rp(projectMeasure);
  rp.setArguments(args);
  EXPECT_TRUE(dv.push(rp));
  EXPECT_EQ(args.size(),rp.arguments().size());
  EXPECT_TRUE(rp.hasIncompleteArguments());
  BOOST_FOREACH(const OSArgument& arg,args) {
    if (arg.name() == "wwr") {
      OSArgument temp = arg.clone();
      temp.setValue(0.6);
      rp.setArgument(temp);
    }
    if (arg.name() == "sillHeight") {
      OSArgument temp = arg.clone();
      temp.setValue(1.0);
      rp.setArgument(temp);
    }
    if (arg.name() == "facade") {
      OSArgument temp = arg.clone();
      temp.setValue("South");
      rp.setArgument(temp);
    }
  }
  EXPECT_FALSE(rp.hasIncompleteArguments());

  openstudio::path tempDir = measuresDir / toPath(toString(createUUID()));
  {
    // create fake new version of the measure
    BCLMeasure newVersion = measure.clone(tempDir).get();
    newVersion.incrementVersionId();
    newVersion.save();
    OSArgumentVector newArgs = args;
    newArgs.push_back(OSArgument::makeDoubleArgument("frame_width"));

    // update the measure
    project.updateMeasure(newVersion,newArgs);

    // verify the final state of SimpleProject and RubyPerturbation
    EXPECT_EQ(1u,project.measures().size());
    BCLMeasure retrievedMeasure = project.getMeasureByUUID(measure.uuid()).get();
    EXPECT_NE(measure.versionUUID(),retrievedMeasure.versionUUID());
    EXPECT_EQ(newVersion.versionUUID(),retrievedMeasure.versionUUID());
    ASSERT_TRUE(project.hasStoredArguments(retrievedMeasure));
    OSArgumentVector retrievedArgs = project.getStoredArguments(retrievedMeasure);
    EXPECT_EQ(args.size() + 1u,retrievedArgs.size());

    EXPECT_EQ(retrievedArgs.size(),rp.arguments().size());
    EXPECT_TRUE(rp.hasIncompleteArguments());
  }
  boost::filesystem::remove_all(tempDir);
}

TEST_F(AnalysisDriverFixture,SimpleProject_NonPATToPATProject) {
  {
    // create project with one discrete variable
    SimpleProject project = getCleanSimpleProject("SimpleProject_NonPATToPATProject");
    openstudio::path measuresDir = resourcesPath() / toPath("/utilities/BCL/Measures");
    openstudio::path dir = measuresDir / toPath("SetWindowToWallRatioByFacade");
    BCLMeasure measure = BCLMeasure::load(dir).get();
    RubyPerturbation rpert(measure);
    project.analysis().problem().push(DiscreteVariable("My Variable",DiscretePerturbationVector(1u,rpert)));
    DataPoint baseline = project.baselineDataPoint();
    EXPECT_EQ(1u,baseline.variableValues().size());
    EXPECT_TRUE(project.analysis().isDirty());
    EXPECT_EQ(1u,project.analysis().dataPoints().size());
    project.save();
  }

  {
    SimpleProject project = getPATProject("SimpleProject_NonPATToPATProject");
    DataPoint baseline = project.baselineDataPoint();
    EXPECT_EQ(2u,baseline.variableValues().size());
    EXPECT_FALSE(project.analysis().isDirty()); // openPATProject calls save in this case
    EXPECT_EQ(1u,project.analysis().dataPoints().size());
    project.save();
  }

  {
    SimpleProject project = getPATProject("SimpleProject_NonPATToPATProject");
    DataPoint baseline = project.baselineDataPoint();
    EXPECT_EQ(2u,baseline.variableValues().size());
    EXPECT_FALSE(project.analysis().isDirty()); // nothing to change in this case
    EXPECT_EQ(1u,project.analysis().dataPoints().size());
  }
}

TEST_F(AnalysisDriverFixture,SimpleProject_ClearAllResults) {
  // March 12, 2013 - ETH
  // Writing test because clearing results mostly working, but there seem to be some residual
  // FileReferenceRecord ids left in the DataPointRecord.
  // Test passes without making any changes, however.

  // Create and run a project with a single LHS point
  SimpleProject project = getCleanPATProject("SimpleProject_ClearAllResults");
  Analysis analysis = project.analysis();
  Problem problem = retrieveProblem(AnalysisDriverFixtureProblem::Continuous,true,false);
  analysis.setProblem(problem);
  openstudio::path p = toPath("./example.osm");
  model::exampleModel().save(p,true);
  FileReference seedModel(p);
  project.setSeed(seedModel);
  DDACEAlgorithmOptions algopts(DDACEAlgorithmType::lhs);
  algopts.setSamples(1);
  DDACEAlgorithm algorithm(algopts);
  analysis.setAlgorithm(algorithm);
  if (!dakotaExePath().empty()) {
    AnalysisDriver analysisDriver = project.analysisDriver();
    AnalysisRunOptions runOptions = standardRunOptions(project.projectDir());
    CurrentAnalysis currentAnalysis = analysisDriver.run(analysis,runOptions);
    analysisDriver.waitForFinished();

    // Check DakotaAlgorithm, DataPoint, and records for expected state
    EXPECT_TRUE(algorithm.isComplete());
    EXPECT_FALSE(algorithm.failed());
    ASSERT_TRUE(algorithm.restartFileReference());
    EXPECT_TRUE(boost::filesystem::exists(algorithm.restartFileReference().get().path()));
    ASSERT_TRUE(algorithm.outFileReference());
    EXPECT_TRUE(boost::filesystem::exists(algorithm.outFileReference().get().path()));
    EXPECT_TRUE(algorithm.job());

    ASSERT_EQ(1u,analysis.dataPoints().size());
    DataPoint dataPoint = analysis.dataPoints()[0];
    EXPECT_TRUE(dataPoint.isComplete());
    EXPECT_FALSE(dataPoint.failed());
    EXPECT_FALSE(dataPoint.responseValues().empty());
    EXPECT_FALSE(dataPoint.directory().empty());
    EXPECT_TRUE(boost::filesystem::exists(dataPoint.directory()));
    ASSERT_TRUE(dataPoint.osmInputData());
    EXPECT_TRUE(boost::filesystem::exists(dataPoint.osmInputData().get().path()));
    ASSERT_TRUE(dataPoint.idfInputData());
    EXPECT_TRUE(boost::filesystem::exists(dataPoint.idfInputData().get().path()));
    EXPECT_FALSE(dataPoint.sqlOutputData());
    EXPECT_FALSE(dataPoint.xmlOutputData());
    EXPECT_TRUE(dataPoint.model());
    EXPECT_TRUE(dataPoint.workspace());
    EXPECT_FALSE(dataPoint.sqlFile());
    EXPECT_TRUE(dataPoint.outputAttributes().empty());
    EXPECT_TRUE(dataPoint.topLevelJob());

    openstudio::UUID dakotaJobUUID, dataPointJobUUID;
    {
      AnalysisRecord analysisRecord = project.analysisRecord();
      RunManager runManager = project.runManager();

      OptionalAlgorithmRecord algorithmRecord = analysisRecord.algorithmRecord();
      ASSERT_TRUE(algorithmRecord);
      ASSERT_TRUE(algorithmRecord.get().optionalCast<DakotaAlgorithmRecord>());
      DakotaAlgorithmRecord dakotaAlgorithmRecord = algorithmRecord->cast<DakotaAlgorithmRecord>();

      EXPECT_TRUE(dakotaAlgorithmRecord.isComplete());
      OptionalFileReferenceRecord frr = dakotaAlgorithmRecord.restartFileReferenceRecord();
      ASSERT_TRUE(frr);
      EXPECT_TRUE(boost::filesystem::exists(frr->path()));
      frr = dakotaAlgorithmRecord.outFileReferenceRecord();
      ASSERT_TRUE(frr);
      EXPECT_TRUE(boost::filesystem::exists(frr->path()));
      ASSERT_TRUE(dakotaAlgorithmRecord.jobUUID());
      dakotaJobUUID = dakotaAlgorithmRecord.jobUUID().get();
      EXPECT_NO_THROW(runManager.getJob(dakotaJobUUID));

      DataPointRecordVector dataPointRecords = analysisRecord.dataPointRecords();
      ASSERT_EQ(1u,dataPointRecords.size());
      DataPointRecord dataPointRecord = dataPointRecords[0];

      EXPECT_TRUE(dataPointRecord.isComplete());
      EXPECT_FALSE(dataPointRecord.failed());
      EXPECT_FALSE(dataPointRecord.directory().empty());
      EXPECT_TRUE(boost::filesystem::exists(dataPointRecord.directory()));
      EXPECT_FALSE(dataPointRecord.responseValues().empty());
      frr = dataPointRecord.osmInputDataRecord();
      ASSERT_TRUE(frr);
      EXPECT_TRUE(boost::filesystem::exists(frr->path()));
      frr = dataPointRecord.idfInputDataRecord();
      ASSERT_TRUE(frr);
      EXPECT_TRUE(boost::filesystem::exists(frr->path()));
      frr = dataPointRecord.sqlOutputDataRecord();
      EXPECT_FALSE(frr);
      frr = dataPointRecord.xmlOutputDataRecord();
      EXPECT_FALSE(frr);
      ASSERT_TRUE(dataPointRecord.topLevelJobUUID());
      dataPointJobUUID = dataPointRecord.topLevelJobUUID().get();
      EXPECT_NO_THROW(runManager.getJob(dataPointJobUUID));
    }

    // Clear all results
    project.clearAllResults();
    project.save();

    // Check DakotaAlgorithm, DataPoint, and records for expected state
    EXPECT_FALSE(algorithm.isComplete());
    EXPECT_EQ(-1,algorithm.iter());
    EXPECT_FALSE(algorithm.failed());
    EXPECT_FALSE(algorithm.restartFileReference());
    EXPECT_FALSE(algorithm.outFileReference());
    EXPECT_FALSE(algorithm.job());

    ASSERT_EQ(1u,analysis.dataPoints().size());
    EXPECT_FALSE(dataPoint.isComplete());
    EXPECT_FALSE(dataPoint.failed());
    EXPECT_TRUE(dataPoint.responseValues().empty());
    EXPECT_TRUE(dataPoint.directory().empty());
    EXPECT_FALSE(dataPoint.osmInputData());
    EXPECT_FALSE(dataPoint.idfInputData());
    EXPECT_FALSE(dataPoint.sqlOutputData());
    ASSERT_FALSE(dataPoint.xmlOutputData());
    EXPECT_FALSE(dataPoint.model());
    EXPECT_FALSE(dataPoint.workspace());
    EXPECT_FALSE(dataPoint.sqlFile());
    EXPECT_TRUE(dataPoint.outputAttributes().empty());
    EXPECT_FALSE(dataPoint.topLevelJob());

    {
      AnalysisRecord analysisRecord = project.analysisRecord();
      RunManager runManager = project.runManager();

      OptionalAlgorithmRecord algorithmRecord = analysisRecord.algorithmRecord();
      ASSERT_TRUE(algorithmRecord);
      ASSERT_TRUE(algorithmRecord.get().optionalCast<DakotaAlgorithmRecord>());
      DakotaAlgorithmRecord dakotaAlgorithmRecord = algorithmRecord->cast<DakotaAlgorithmRecord>();

      EXPECT_FALSE(dakotaAlgorithmRecord.isComplete());
      OptionalFileReferenceRecord frr = dakotaAlgorithmRecord.restartFileReferenceRecord();
      EXPECT_FALSE(frr);
      frr = dakotaAlgorithmRecord.outFileReferenceRecord();
      EXPECT_FALSE(frr);
      EXPECT_FALSE(dakotaAlgorithmRecord.jobUUID());
      EXPECT_ANY_THROW(runManager.getJob(dakotaJobUUID));

      DataPointRecordVector dataPointRecords = analysisRecord.dataPointRecords();
      ASSERT_EQ(1u,dataPointRecords.size());
      DataPointRecord dataPointRecord = dataPointRecords[0];

      EXPECT_FALSE(dataPointRecord.isComplete());
      EXPECT_FALSE(dataPointRecord.failed());
      EXPECT_TRUE(dataPointRecord.directory().empty());
      EXPECT_TRUE(dataPointRecord.responseValues().empty());
      frr = dataPointRecord.osmInputDataRecord();
      EXPECT_FALSE(frr);
      frr = dataPointRecord.idfInputDataRecord();
      EXPECT_FALSE(frr);
      frr = dataPointRecord.sqlOutputDataRecord();
      EXPECT_FALSE(frr);
      frr = dataPointRecord.xmlOutputDataRecord();
      EXPECT_FALSE(frr);
      EXPECT_FALSE(dataPointRecord.topLevelJobUUID());
      EXPECT_ANY_THROW(runManager.getJob(dataPointJobUUID));
    }
  }
}

TEST_F(AnalysisDriverFixture,SimpleProject_EditProblemWithTwoWorkflows) {
  // Written to reproduce Bug 1189
  {
    // Create PAT project with an IDF measure
    SimpleProject project = getCleanPATProject("SimpleProject_EditProblemWithTwoWorkflows");

    Problem problem = project.analysis().problem();
    OptionalInt index = problem.getWorkflowStepIndexByJobType(JobType::EnergyPlusPreProcess);
    ASSERT_TRUE(index);
    DiscreteVariable dv("Idf Measure",DiscretePerturbationVector(1u,NullPerturbation()));
    problem.insert(*index,dv);
    BOOST_FOREACH(const BCLMeasure& measure,BCLMeasure::patApplicationMeasures()) {
      if (measure.inputFileType() == FileReferenceType::IDF) {
        RubyPerturbation pert(measure);
        dv.push(pert);
        break;
      }
    }

    project.save();

    ProjectDatabase database = project.projectDatabase();
    RunManager runManager = project.runManager();
    WorkflowRecordVector workflowRecords = WorkflowRecord::getWorkflowRecords(database);
    EXPECT_EQ(2u,workflowRecords.size());
    BOOST_FOREACH(const WorkflowRecord& wr,workflowRecords) {
      EXPECT_NO_THROW(runManager.loadWorkflow(wr.runManagerWorkflowKey()));
    }
  }

  openstudio::path ospPath;
  {
    // Reopen, change and save
    SimpleProject project = getPATProject("SimpleProject_EditProblemWithTwoWorkflows");

    Model model = exampleModel();
    openstudio::path p = toPath("example.osm");
    model.save(p,false);

    EXPECT_TRUE(project.addAlternateModel(p));

    project.save();

    // For some reason, problem manifested below does not show up here, while ProjectDatabase
    // and RunManager still in memory.
    ProjectDatabase database = project.projectDatabase();
    ospPath = database.path();
    RunManager runManager = project.runManager();
    WorkflowRecordVector workflowRecords = WorkflowRecord::getWorkflowRecords(database);
    EXPECT_EQ(2u,workflowRecords.size());
    BOOST_FOREACH(const WorkflowRecord& wr,workflowRecords) {
      EXPECT_NO_THROW(runManager.loadWorkflow(wr.runManagerWorkflowKey()));
    }
  }

  {
    // underlying problem -- workflow key in ProjectDatabase and RunManager database get out of sync
    // See ProblemRecord::constructRelatedRecords for code where old WorkflowRecords are removed
    // and new ones are created.
    ProjectDatabase database = ProjectDatabase::open(ospPath).get();
    RunManager runManager = database.runManager();
    WorkflowRecordVector workflowRecords = WorkflowRecord::getWorkflowRecords(database);
    EXPECT_EQ(2u,workflowRecords.size());
    BOOST_FOREACH(const WorkflowRecord& wr,workflowRecords) {
      EXPECT_NO_THROW(runManager.loadWorkflow(wr.runManagerWorkflowKey()));
    }

    // symptom in PAT -- BOOST assert on deserialize Problem (buried in call to openPATProject)
    ProblemRecordVector problemRecords = ProblemRecord::getProblemRecords(database);
    ASSERT_EQ(1u,problemRecords.size());
    ASSERT_NO_FATAL_FAILURE(problemRecords[0].problem());
  }
}