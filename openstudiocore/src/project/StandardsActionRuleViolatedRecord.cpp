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

#include <project/StandardsActionRuleViolatedRecord.hpp>
#include <project/StandardsActionRuleViolatedRecord_Impl.hpp>
#include <project/JoinRecord.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/Rule_Clause_JoinRecord.hpp>
#include <project/Rule_Clause_JoinRecord_Impl.hpp>
#include <project/StandardsRulesetOptionRecord.hpp>
#include <project/StandardsFilterObjectAttributeRecord.hpp>

#include <ruleset/StandardsActionRuleViolated.hpp>
#include <ruleset/StandardsFilterObjectAttribute.hpp>
#include <ruleset/StandardsRulesetOption.hpp>

#include <utilities/core/Assert.hpp>

#include <boost/optional/optional.hpp>
#include <boost/foreach.hpp>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace openstudio {
namespace project {

namespace detail {

  StandardsActionRuleViolatedRecord_Impl::StandardsActionRuleViolatedRecord_Impl(const openstudio::ruleset::StandardsActionRuleViolated& standardsActionRuleViolated, const ProjectDatabase& projectDatabase)
    : StandardsActionRuleResultRecord_Impl(standardsActionRuleViolated, StandardsActionRuleResultRecordType::StandardsActionRuleViolatedRecord, projectDatabase)
  {
  }

  StandardsActionRuleViolatedRecord_Impl::StandardsActionRuleViolatedRecord_Impl(const QSqlQuery& query, const ProjectDatabase& projectDatabase)
    : StandardsActionRuleResultRecord_Impl(query, projectDatabase)
  {
  }

  openstudio::ruleset::StandardsActionRuleViolated StandardsActionRuleViolatedRecord_Impl::standardsActionRuleViolated() const
  {
    StandardsFilterObjectAttributeRecord standardsFilterObjectAttributeRecord = this->standardsFilterObjectAttributeRecord();
    openstudio::ruleset::StandardsActionRuleViolated result(standardsFilterObjectAttributeRecord.standardsFilterObjectAttribute(), this->handle(), this->uuidLast());
    return result;
  }

  openstudio::ruleset::ActionClause StandardsActionRuleViolatedRecord_Impl::actionClause() const
  {
    return this->standardsActionRuleViolated();
  }

  boost::optional<ObjectRecord> StandardsActionRuleViolatedRecord_Impl::parent() const
  {
    return boost::none;
  }

  std::vector<ObjectRecord> StandardsActionRuleViolatedRecord_Impl::children() const
  {
    return std::vector<ObjectRecord>();
  }

  std::vector<ObjectRecord> StandardsActionRuleViolatedRecord_Impl::resources() const
  {
    StandardsFilterObjectAttributeRecord record = this->standardsFilterObjectAttributeRecord();
    return std::vector<ObjectRecord>(1u,record);
  }

  std::vector<JoinRecord> StandardsActionRuleViolatedRecord_Impl::joinRecords() const
  {
    std::vector<Rule_Clause_JoinRecord> result = JoinRecord::getJoinRecordsForRightId<Rule_Clause_JoinRecord>(this->id(), this->projectDatabase());
    return std::vector<JoinRecord>(result.begin(), result.end());
  }

  void StandardsActionRuleViolatedRecord_Impl::saveRow(const boost::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<StandardsActionRuleViolatedRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  void StandardsActionRuleViolatedRecord_Impl::bindValues(QSqlQuery& query) const
  {
    StandardsActionRuleResultRecord_Impl::bindValues(query);
  }

  void StandardsActionRuleViolatedRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  {
    StandardsActionRuleResultRecord_Impl::setLastValues(query, projectDatabase);
  }

  bool StandardsActionRuleViolatedRecord_Impl::compareValues(const QSqlQuery& query) const
  {
    bool result = true;

    result = result && StandardsActionRuleResultRecord_Impl::compareValues(query);

    return result;
  }

  void StandardsActionRuleViolatedRecord_Impl::saveLastValues()
  {
    StandardsActionRuleResultRecord_Impl::saveLastValues();
  }

  void StandardsActionRuleViolatedRecord_Impl::revertToLastValues()
  {
    StandardsActionRuleResultRecord_Impl::revertToLastValues();
  }

} // detail

StandardsActionRuleViolatedRecord::StandardsActionRuleViolatedRecord(const openstudio::ruleset::StandardsActionRuleViolated& standardsActionRuleViolated, ProjectDatabase& projectDatabase)
  : StandardsActionRuleResultRecord(boost::shared_ptr<detail::StandardsActionRuleViolatedRecord_Impl>(new detail::StandardsActionRuleViolatedRecord_Impl(standardsActionRuleViolated, projectDatabase)), projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::StandardsActionRuleViolatedRecord_Impl>());
}

StandardsActionRuleViolatedRecord::StandardsActionRuleViolatedRecord(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  : StandardsActionRuleResultRecord(boost::shared_ptr<detail::StandardsActionRuleViolatedRecord_Impl>(new detail::StandardsActionRuleViolatedRecord_Impl(query, projectDatabase)), projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::StandardsActionRuleViolatedRecord_Impl>());
}

StandardsActionRuleViolatedRecord::StandardsActionRuleViolatedRecord(boost::shared_ptr<detail::StandardsActionRuleViolatedRecord_Impl> impl, ProjectDatabase projectDatabase)
  : StandardsActionRuleResultRecord(impl, projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::StandardsActionRuleViolatedRecord_Impl>());
}

/// @cond
StandardsActionRuleViolatedRecord::StandardsActionRuleViolatedRecord(boost::shared_ptr<detail::StandardsActionRuleViolatedRecord_Impl> impl)
  : StandardsActionRuleResultRecord(impl)
{
  BOOST_ASSERT(getImpl<detail::StandardsActionRuleViolatedRecord_Impl>());
}
/// @endcond

boost::optional<StandardsActionRuleViolatedRecord> StandardsActionRuleViolatedRecord::factoryFromQuery(const QSqlQuery& query, 
                                                                 ProjectDatabase& database)
{
  OptionalStandardsActionRuleViolatedRecord result;
  try {
    result = StandardsActionRuleViolatedRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct StandardsActionRuleViolatedRecord from query, because '" 
        << e.what() << "'.");
  }
  return result;
}

std::vector<StandardsActionRuleViolatedRecord> StandardsActionRuleViolatedRecord::getStandardsActionRuleViolatedRecords(ProjectDatabase& database) {
  std::vector<StandardsActionRuleViolatedRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + 
      " WHERE clauseRecordType=:clauseRecordType" +
      " AND actionClauseRecordType=:actionClauseRecordType" +
      " AND standardsActionClauseRecordType=:standardsActionClauseRecordType" +
      " AND standardsActionRuleResultRecordType=:standardsActionRuleResultRecordType"));
  query.bindValue(":clauseRecordType",ClauseRecordType::ActionClauseRecord);
  query.bindValue(":actionClauseRecordType",ActionClauseRecordType::StandardsActionClauseRecord);
  query.bindValue(":standardsActionClauseRecordType", StandardsActionClauseRecordType::StandardsActionRuleResultRecord);
  query.bindValue(":standardsActionRuleResultRecordType", StandardsActionRuleResultRecordType::StandardsActionRuleViolatedRecord);
  assertExec(query);
  while (query.next()) {
    result.push_back(StandardsActionRuleViolatedRecord(query, database));
  }

  return result;
}

boost::optional<StandardsActionRuleViolatedRecord> StandardsActionRuleViolatedRecord::getStandardsActionRuleViolatedRecord(int id, ProjectDatabase& database) {
  boost::optional<StandardsActionRuleViolatedRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + 
      " WHERE clauseRecordType=:clauseRecordType" +
      " AND actionClauseRecordType=:actionClauseRecordType" +
      " AND standardsActionClauseRecordType=:standardsActionClauseRecordType" +
      " AND standardsActionRuleResultRecordType=:standardsActionRuleResultRecordType AND id=:id"));
  query.bindValue(":clauseRecordType",ClauseRecordType::ActionClauseRecord);
  query.bindValue(":actionClauseRecordType",ActionClauseRecordType::StandardsActionClauseRecord);
  query.bindValue(":standardsActionClauseRecordType", StandardsActionClauseRecordType::StandardsActionRuleResultRecord);
  query.bindValue(":standardsActionRuleResultRecordType", StandardsActionRuleResultRecordType::StandardsActionRuleViolatedRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = StandardsActionRuleViolatedRecord(query, database);
  }

  return result;
}

openstudio::ruleset::StandardsActionRuleViolated StandardsActionRuleViolatedRecord::standardsActionRuleViolated() const
{
  return this->getImpl<detail::StandardsActionRuleViolatedRecord_Impl>()->standardsActionRuleViolated();
}

} // project
} // openstudio