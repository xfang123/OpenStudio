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

#include <model/DaylightingControl.hpp>
#include <model/DaylightingControl_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_Daylighting_Control_FieldEnums.hxx>

#include <utilities/geometry/Transformation.hpp>
#include <utilities/geometry/EulerAngles.hpp>
#include <utilities/geometry/Vector3d.hpp>
#include <utilities/geometry/Point3d.hpp>
#include <utilities/geometry/Geometry.hpp>
#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  DaylightingControl_Impl::DaylightingControl_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : SpaceItem_Impl(idfObject,model,keepHandle)
  {
    BOOST_ASSERT(idfObject.iddObject().type() == DaylightingControl::iddObjectType());
  }

  DaylightingControl_Impl::DaylightingControl_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                   Model_Impl* model,
                                                   bool keepHandle)
    : SpaceItem_Impl(other,model,keepHandle)
  {
    BOOST_ASSERT(other.iddObject().type() == DaylightingControl::iddObjectType());
  }

  DaylightingControl_Impl::DaylightingControl_Impl(const DaylightingControl_Impl& other,
                                                   Model_Impl* model,
                                                   bool keepHandle)
    : SpaceItem_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& DaylightingControl_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType DaylightingControl_Impl::iddObjectType() const {
    return DaylightingControl::iddObjectType();
  }

  double DaylightingControl_Impl::positionXCoordinate() const {
    boost::optional<double> value = getDouble(OS_Daylighting_ControlFields::PositionXCoordinate,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  double DaylightingControl_Impl::positionYCoordinate() const {
    boost::optional<double> value = getDouble(OS_Daylighting_ControlFields::PositionYCoordinate,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  double DaylightingControl_Impl::positionZCoordinate() const {
    boost::optional<double> value = getDouble(OS_Daylighting_ControlFields::PositionZCoordinate,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  double DaylightingControl_Impl::psiRotationAroundXAxis() const {
    boost::optional<double> value = getDouble(OS_Daylighting_ControlFields::PsiRotationAroundXAxis,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool DaylightingControl_Impl::isPsiRotationAroundXAxisDefaulted() const {
    return isEmpty(OS_Daylighting_ControlFields::PsiRotationAroundXAxis);
  }

  double DaylightingControl_Impl::thetaRotationAroundYAxis() const {
    boost::optional<double> value = getDouble(OS_Daylighting_ControlFields::ThetaRotationAroundYAxis,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool DaylightingControl_Impl::isThetaRotationAroundYAxisDefaulted() const {
    return isEmpty(OS_Daylighting_ControlFields::ThetaRotationAroundYAxis);
  }

  double DaylightingControl_Impl::phiRotationAroundZAxis() const {
    boost::optional<double> value = getDouble(OS_Daylighting_ControlFields::PhiRotationAroundZAxis,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool DaylightingControl_Impl::isPhiRotationAroundZAxisDefaulted() const {
    return isEmpty(OS_Daylighting_ControlFields::PhiRotationAroundZAxis);
  }

  double DaylightingControl_Impl::illuminanceSetpoint() const {
    boost::optional<double> value = getDouble(OS_Daylighting_ControlFields::IlluminanceSetpoint,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool DaylightingControl_Impl::isIlluminanceSetpointDefaulted() const {
    return isEmpty(OS_Daylighting_ControlFields::IlluminanceSetpoint);
  }

  std::string DaylightingControl_Impl::lightingControlType() const {
    boost::optional<std::string> value = getString(OS_Daylighting_ControlFields::LightingControlType,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool DaylightingControl_Impl::isLightingControlTypeDefaulted() const {
    return isEmpty(OS_Daylighting_ControlFields::LightingControlType);
  }

  double DaylightingControl_Impl::minimumInputPowerFractionforContinuousDimmingControl() const {
    boost::optional<double> value = getDouble(OS_Daylighting_ControlFields::MinimumInputPowerFractionforContinuousDimmingControl,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool DaylightingControl_Impl::isMinimumInputPowerFractionforContinuousDimmingControlDefaulted() const {
    return isEmpty(OS_Daylighting_ControlFields::MinimumInputPowerFractionforContinuousDimmingControl);
  }

  double DaylightingControl_Impl::minimumLightOutputFractionforContinuousDimmingControl() const {
    boost::optional<double> value = getDouble(OS_Daylighting_ControlFields::MinimumLightOutputFractionforContinuousDimmingControl,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool DaylightingControl_Impl::isMinimumLightOutputFractionforContinuousDimmingControlDefaulted() const {
    return isEmpty(OS_Daylighting_ControlFields::MinimumLightOutputFractionforContinuousDimmingControl);
  }

  int DaylightingControl_Impl::numberofSteppedControlSteps() const {
    boost::optional<int> value = getInt(OS_Daylighting_ControlFields::NumberofSteppedControlSteps,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool DaylightingControl_Impl::isNumberofSteppedControlStepsDefaulted() const {
    return isEmpty(OS_Daylighting_ControlFields::NumberofSteppedControlSteps);
  }

  double DaylightingControl_Impl::probabilityLightingwillbeResetWhenNeededinManualSteppedControl() const {
    boost::optional<double> value = getDouble(OS_Daylighting_ControlFields::ProbabilityLightingwillbeResetWhenNeededinManualSteppedControl,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool DaylightingControl_Impl::isProbabilityLightingwillbeResetWhenNeededinManualSteppedControlDefaulted() const {
    return isEmpty(OS_Daylighting_ControlFields::ProbabilityLightingwillbeResetWhenNeededinManualSteppedControl);
  }

  int DaylightingControl_Impl::numberofDaylightingViews() const {
    boost::optional<int> value = getInt(OS_Daylighting_ControlFields::NumberofDaylightingViews,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool DaylightingControl_Impl::isNumberofDaylightingViewsDefaulted() const {
    return isEmpty(OS_Daylighting_ControlFields::NumberofDaylightingViews);
  }

  boost::optional<double> DaylightingControl_Impl::maximumAllowableDiscomfortGlareIndex() const {
    return getDouble(OS_Daylighting_ControlFields::MaximumAllowableDiscomfortGlareIndex,true);
  }

  void DaylightingControl_Impl::setPositionXCoordinate(double positionXCoordinate) {
    bool result = false;
    result = setDouble(OS_Daylighting_ControlFields::PositionXCoordinate, positionXCoordinate);
    BOOST_ASSERT(result);
  }

  void DaylightingControl_Impl::setPositionYCoordinate(double positionYCoordinate) {
    bool result = false;
    result = setDouble(OS_Daylighting_ControlFields::PositionYCoordinate, positionYCoordinate);
    BOOST_ASSERT(result);
  }

  void DaylightingControl_Impl::setPositionZCoordinate(double positionZCoordinate) {
    bool result = false;
    result = setDouble(OS_Daylighting_ControlFields::PositionZCoordinate, positionZCoordinate);
    BOOST_ASSERT(result);
  }


  void DaylightingControl_Impl::setPsiRotationAroundXAxis(double psiRotationAroundXAxis) {
    bool result = false;
    result = setDouble(OS_Daylighting_ControlFields::PsiRotationAroundXAxis, psiRotationAroundXAxis);
    BOOST_ASSERT(result);
  }

  void DaylightingControl_Impl::resetPsiRotationAroundXAxis() {
    bool result = setString(OS_Daylighting_ControlFields::PsiRotationAroundXAxis, "");
    BOOST_ASSERT(result);
  }

  void DaylightingControl_Impl::setThetaRotationAroundYAxis(double thetaRotationAroundYAxis) {
    bool result = false;
    result = setDouble(OS_Daylighting_ControlFields::ThetaRotationAroundYAxis, thetaRotationAroundYAxis);
    BOOST_ASSERT(result);
  }

  void DaylightingControl_Impl::resetThetaRotationAroundYAxis() {
    bool result = setString(OS_Daylighting_ControlFields::ThetaRotationAroundYAxis, "");
    BOOST_ASSERT(result);
  }

  void DaylightingControl_Impl::setPhiRotationAroundZAxis(double phiRotationAroundZAxis) {
    bool result = false;
    result = setDouble(OS_Daylighting_ControlFields::PhiRotationAroundZAxis, phiRotationAroundZAxis);
    BOOST_ASSERT(result);
  }

  void DaylightingControl_Impl::resetPhiRotationAroundZAxis() {
    bool result = setString(OS_Daylighting_ControlFields::PhiRotationAroundZAxis, "");
    BOOST_ASSERT(result);
  }

  bool DaylightingControl_Impl::setIlluminanceSetpoint(double illuminanceSetpoint) {
    bool result = false;
    result = setDouble(OS_Daylighting_ControlFields::IlluminanceSetpoint, illuminanceSetpoint);
    return result;
  }

  void DaylightingControl_Impl::resetIlluminanceSetpoint() {
    bool result = setString(OS_Daylighting_ControlFields::IlluminanceSetpoint, "");
    BOOST_ASSERT(result);
  }

  bool DaylightingControl_Impl::setLightingControlType(std::string lightingControlType) {
    bool result = false;
    result = setString(OS_Daylighting_ControlFields::LightingControlType, lightingControlType);
    return result;
  }

  void DaylightingControl_Impl::resetLightingControlType() {
    bool result = setString(OS_Daylighting_ControlFields::LightingControlType, "");
    BOOST_ASSERT(result);
  }

  bool DaylightingControl_Impl::setMinimumInputPowerFractionforContinuousDimmingControl(double minimumInputPowerFractionforContinuousDimmingControl) {
    bool result = false;
    result = setDouble(OS_Daylighting_ControlFields::MinimumInputPowerFractionforContinuousDimmingControl, minimumInputPowerFractionforContinuousDimmingControl);
    return result;
  }

  void DaylightingControl_Impl::resetMinimumInputPowerFractionforContinuousDimmingControl() {
    bool result = setString(OS_Daylighting_ControlFields::MinimumInputPowerFractionforContinuousDimmingControl, "");
    BOOST_ASSERT(result);
  }

  bool DaylightingControl_Impl::setMinimumLightOutputFractionforContinuousDimmingControl(double minimumLightOutputFractionforContinuousDimmingControl) {
    bool result = false;
    result = setDouble(OS_Daylighting_ControlFields::MinimumLightOutputFractionforContinuousDimmingControl, minimumLightOutputFractionforContinuousDimmingControl);
    return result;
  }

  void DaylightingControl_Impl::resetMinimumLightOutputFractionforContinuousDimmingControl() {
    bool result = setString(OS_Daylighting_ControlFields::MinimumLightOutputFractionforContinuousDimmingControl, "");
    BOOST_ASSERT(result);
  }

  void DaylightingControl_Impl::setNumberofSteppedControlSteps(int numberofSteppedControlSteps) {
    bool result = false;
    result = setInt(OS_Daylighting_ControlFields::NumberofSteppedControlSteps, numberofSteppedControlSteps);
    BOOST_ASSERT(result);
  }

  void DaylightingControl_Impl::resetNumberofSteppedControlSteps() {
    bool result = setString(OS_Daylighting_ControlFields::NumberofSteppedControlSteps, "");
    BOOST_ASSERT(result);
  }

  bool DaylightingControl_Impl::setProbabilityLightingwillbeResetWhenNeededinManualSteppedControl(double probabilityLightingwillbeResetWhenNeededinManualSteppedControl) {
    bool result = false;
    result = setDouble(OS_Daylighting_ControlFields::ProbabilityLightingwillbeResetWhenNeededinManualSteppedControl, probabilityLightingwillbeResetWhenNeededinManualSteppedControl);
    return result;
  }

  void DaylightingControl_Impl::resetProbabilityLightingwillbeResetWhenNeededinManualSteppedControl() {
    bool result = setString(OS_Daylighting_ControlFields::ProbabilityLightingwillbeResetWhenNeededinManualSteppedControl, "");
    BOOST_ASSERT(result);
  }

  bool DaylightingControl_Impl::setNumberofDaylightingViews(int numberofDaylightingViews) {
    bool result = false;
    result = setInt(OS_Daylighting_ControlFields::NumberofDaylightingViews, numberofDaylightingViews);
    return result;
  }

  void DaylightingControl_Impl::resetNumberofDaylightingViews() {
    bool result = setString(OS_Daylighting_ControlFields::NumberofDaylightingViews, "");
    BOOST_ASSERT(result);
  }

  bool DaylightingControl_Impl::setMaximumAllowableDiscomfortGlareIndex(boost::optional<double> maximumAllowableDiscomfortGlareIndex) {
    bool result = false;
    if (maximumAllowableDiscomfortGlareIndex) {
      result = setDouble(OS_Daylighting_ControlFields::MaximumAllowableDiscomfortGlareIndex, maximumAllowableDiscomfortGlareIndex.get());
    } else {
      result = setString(OS_Daylighting_ControlFields::MaximumAllowableDiscomfortGlareIndex, "");
    }
    return result;
  }

  void DaylightingControl_Impl::resetMaximumAllowableDiscomfortGlareIndex() {
    bool result = setString(OS_Daylighting_ControlFields::MaximumAllowableDiscomfortGlareIndex, "");
    BOOST_ASSERT(result);
  }

  Point3d DaylightingControl_Impl::position() const
  {
    Point3d result(this->positionXCoordinate(),
                   this->positionYCoordinate(),
                   this->positionZCoordinate());
    return result;
  }

  bool DaylightingControl_Impl::setPosition(const Point3d& position)
  {
    this->setPositionXCoordinate(position.x());
    this->setPositionYCoordinate(position.y());
    this->setPositionZCoordinate(position.z());
    return true;
  }

  openstudio::Transformation DaylightingControl_Impl::transformation() const
  {
    Vector3d origin(this->positionXCoordinate(),
                    this->positionYCoordinate(),
                    this->positionZCoordinate());

    EulerAngles angles(degToRad(this->psiRotationAroundXAxis()),
                       degToRad(this->thetaRotationAroundYAxis()),
                       degToRad(this->phiRotationAroundZAxis()));

    Transformation result = Transformation::translation(origin)*Transformation::rotation(angles);

    return result;
  }

  bool DaylightingControl_Impl::setTransformation(const openstudio::Transformation& transformation) 
  {
    Vector3d translation = transformation.translation();
    this->setPositionXCoordinate(translation.x());
    this->setPositionYCoordinate(translation.y());
    this->setPositionZCoordinate(translation.z());

    EulerAngles eulerAngles = transformation.eulerAngles();
    setPsiRotationAroundXAxis(radToDeg(eulerAngles.psi()));
    setThetaRotationAroundYAxis(radToDeg(eulerAngles.theta()));
    setPhiRotationAroundZAxis(radToDeg(eulerAngles.phi()));
    
    return true;  
  }

  bool DaylightingControl_Impl::isPrimaryDaylightingControl() const
  {
    bool result = false;
    std::vector<WorkspaceObject> workspaceObjects = this->getSources(IddObjectType::OS_ThermalZone);
    if (!workspaceObjects.empty()){
      BOOST_ASSERT(workspaceObjects.size() == 1);
      boost::optional<DaylightingControl> control = workspaceObjects[0].cast<ThermalZone>().primaryDaylightingControl();
      if (control){
        result = (this->handle() == control->handle());
      }
    }
    return result;
  }

  bool DaylightingControl_Impl::isSecondaryDaylightingControl() const
  {
    bool result = false;
    std::vector<WorkspaceObject> workspaceObjects = this->getSources(IddObjectType::OS_ThermalZone);
    if (!workspaceObjects.empty()){
      BOOST_ASSERT(workspaceObjects.size() == 1);
      boost::optional<DaylightingControl> control = workspaceObjects[0].cast<ThermalZone>().secondaryDaylightingControl();
      if (control){
        result = (this->handle() == control->handle());
      }
    }
    return result;  }

  int DaylightingControl_Impl::spaceIndex() const
  {
    return OS_Daylighting_ControlFields::SpaceName;
  }

} // detail

DaylightingControl::DaylightingControl(const Model& model)
  : SpaceItem(DaylightingControl::iddObjectType(),model)
{
  BOOST_ASSERT(getImpl<detail::DaylightingControl_Impl>());
}

IddObjectType DaylightingControl::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Daylighting_Control);
  return result;
}

std::vector<std::string> DaylightingControl::validLightingControlTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Daylighting_ControlFields::LightingControlType);
}

double DaylightingControl::positionXCoordinate() const {
  return getImpl<detail::DaylightingControl_Impl>()->positionXCoordinate();
}

double DaylightingControl::positionYCoordinate() const {
  return getImpl<detail::DaylightingControl_Impl>()->positionYCoordinate();
}

double DaylightingControl::positionZCoordinate() const {
  return getImpl<detail::DaylightingControl_Impl>()->positionZCoordinate();
}

double DaylightingControl::psiRotationAroundXAxis() const {
  return getImpl<detail::DaylightingControl_Impl>()->psiRotationAroundXAxis();
}

bool DaylightingControl::isPsiRotationAroundXAxisDefaulted() const {
  return getImpl<detail::DaylightingControl_Impl>()->isPsiRotationAroundXAxisDefaulted();
}

double DaylightingControl::thetaRotationAroundYAxis() const {
  return getImpl<detail::DaylightingControl_Impl>()->thetaRotationAroundYAxis();
}

bool DaylightingControl::isThetaRotationAroundYAxisDefaulted() const {
  return getImpl<detail::DaylightingControl_Impl>()->isThetaRotationAroundYAxisDefaulted();
}

double DaylightingControl::phiRotationAroundZAxis() const {
  return getImpl<detail::DaylightingControl_Impl>()->phiRotationAroundZAxis();
}

bool DaylightingControl::isPhiRotationAroundZAxisDefaulted() const {
  return getImpl<detail::DaylightingControl_Impl>()->isPhiRotationAroundZAxisDefaulted();
}

double DaylightingControl::illuminanceSetpoint() const {
  return getImpl<detail::DaylightingControl_Impl>()->illuminanceSetpoint();
}

bool DaylightingControl::isIlluminanceSetpointDefaulted() const {
  return getImpl<detail::DaylightingControl_Impl>()->isIlluminanceSetpointDefaulted();
}

std::string DaylightingControl::lightingControlType() const {
  return getImpl<detail::DaylightingControl_Impl>()->lightingControlType();
}

bool DaylightingControl::isLightingControlTypeDefaulted() const {
  return getImpl<detail::DaylightingControl_Impl>()->isLightingControlTypeDefaulted();
}

double DaylightingControl::minimumInputPowerFractionforContinuousDimmingControl() const {
  return getImpl<detail::DaylightingControl_Impl>()->minimumInputPowerFractionforContinuousDimmingControl();
}

bool DaylightingControl::isMinimumInputPowerFractionforContinuousDimmingControlDefaulted() const {
  return getImpl<detail::DaylightingControl_Impl>()->isMinimumInputPowerFractionforContinuousDimmingControlDefaulted();
}

double DaylightingControl::minimumLightOutputFractionforContinuousDimmingControl() const {
  return getImpl<detail::DaylightingControl_Impl>()->minimumLightOutputFractionforContinuousDimmingControl();
}

bool DaylightingControl::isMinimumLightOutputFractionforContinuousDimmingControlDefaulted() const {
  return getImpl<detail::DaylightingControl_Impl>()->isMinimumLightOutputFractionforContinuousDimmingControlDefaulted();
}

int DaylightingControl::numberofSteppedControlSteps() const {
  return getImpl<detail::DaylightingControl_Impl>()->numberofSteppedControlSteps();
}

bool DaylightingControl::isNumberofSteppedControlStepsDefaulted() const {
  return getImpl<detail::DaylightingControl_Impl>()->isNumberofSteppedControlStepsDefaulted();
}

double DaylightingControl::probabilityLightingwillbeResetWhenNeededinManualSteppedControl() const {
  return getImpl<detail::DaylightingControl_Impl>()->probabilityLightingwillbeResetWhenNeededinManualSteppedControl();
}

bool DaylightingControl::isProbabilityLightingwillbeResetWhenNeededinManualSteppedControlDefaulted() const {
  return getImpl<detail::DaylightingControl_Impl>()->isProbabilityLightingwillbeResetWhenNeededinManualSteppedControlDefaulted();
}

int DaylightingControl::numberofDaylightingViews() const {
  return getImpl<detail::DaylightingControl_Impl>()->numberofDaylightingViews();
}

bool DaylightingControl::isNumberofDaylightingViewsDefaulted() const {
  return getImpl<detail::DaylightingControl_Impl>()->isNumberofDaylightingViewsDefaulted();
}

boost::optional<double> DaylightingControl::maximumAllowableDiscomfortGlareIndex() const {
  return getImpl<detail::DaylightingControl_Impl>()->maximumAllowableDiscomfortGlareIndex();
}

void DaylightingControl::setPositionXCoordinate(double positionXCoordinate) {
  getImpl<detail::DaylightingControl_Impl>()->setPositionXCoordinate(positionXCoordinate);
}

void DaylightingControl::setPositionYCoordinate(double positionYCoordinate) {
  getImpl<detail::DaylightingControl_Impl>()->setPositionYCoordinate(positionYCoordinate);
}

void DaylightingControl::setPositionZCoordinate(double positionZCoordinate) {
  getImpl<detail::DaylightingControl_Impl>()->setPositionZCoordinate(positionZCoordinate);
}

void DaylightingControl::setPsiRotationAroundXAxis(double psiRotationAroundXAxis) {
  return getImpl<detail::DaylightingControl_Impl>()->setPsiRotationAroundXAxis(psiRotationAroundXAxis);
}

void DaylightingControl::resetPsiRotationAroundXAxis() {
  getImpl<detail::DaylightingControl_Impl>()->resetPsiRotationAroundXAxis();
}

void DaylightingControl::setThetaRotationAroundYAxis(double thetaRotationAroundYAxis) {
  return getImpl<detail::DaylightingControl_Impl>()->setThetaRotationAroundYAxis(thetaRotationAroundYAxis);
}

void DaylightingControl::resetThetaRotationAroundYAxis() {
  getImpl<detail::DaylightingControl_Impl>()->resetThetaRotationAroundYAxis();
}

void DaylightingControl::setPhiRotationAroundZAxis(double phiRotationAroundZAxis) {
  return getImpl<detail::DaylightingControl_Impl>()->setPhiRotationAroundZAxis(phiRotationAroundZAxis);
}

void DaylightingControl::resetPhiRotationAroundZAxis() {
  getImpl<detail::DaylightingControl_Impl>()->resetPhiRotationAroundZAxis();
}

bool DaylightingControl::setIlluminanceSetpoint(double illuminanceSetpoint) {
  return getImpl<detail::DaylightingControl_Impl>()->setIlluminanceSetpoint(illuminanceSetpoint);
}

void DaylightingControl::resetIlluminanceSetpoint() {
  getImpl<detail::DaylightingControl_Impl>()->resetIlluminanceSetpoint();
}

bool DaylightingControl::setLightingControlType(std::string lightingControlType) {
  return getImpl<detail::DaylightingControl_Impl>()->setLightingControlType(lightingControlType);
}

void DaylightingControl::resetLightingControlType() {
  getImpl<detail::DaylightingControl_Impl>()->resetLightingControlType();
}

bool DaylightingControl::setMinimumInputPowerFractionforContinuousDimmingControl(double minimumInputPowerFractionforContinuousDimmingControl) {
  return getImpl<detail::DaylightingControl_Impl>()->setMinimumInputPowerFractionforContinuousDimmingControl(minimumInputPowerFractionforContinuousDimmingControl);
}

void DaylightingControl::resetMinimumInputPowerFractionforContinuousDimmingControl() {
  getImpl<detail::DaylightingControl_Impl>()->resetMinimumInputPowerFractionforContinuousDimmingControl();
}

bool DaylightingControl::setMinimumLightOutputFractionforContinuousDimmingControl(double minimumLightOutputFractionforContinuousDimmingControl) {
  return getImpl<detail::DaylightingControl_Impl>()->setMinimumLightOutputFractionforContinuousDimmingControl(minimumLightOutputFractionforContinuousDimmingControl);
}

void DaylightingControl::resetMinimumLightOutputFractionforContinuousDimmingControl() {
  getImpl<detail::DaylightingControl_Impl>()->resetMinimumLightOutputFractionforContinuousDimmingControl();
}

void DaylightingControl::setNumberofSteppedControlSteps(int numberofSteppedControlSteps) {
  getImpl<detail::DaylightingControl_Impl>()->setNumberofSteppedControlSteps(numberofSteppedControlSteps);
}

void DaylightingControl::resetNumberofSteppedControlSteps() {
  getImpl<detail::DaylightingControl_Impl>()->resetNumberofSteppedControlSteps();
}

bool DaylightingControl::setProbabilityLightingwillbeResetWhenNeededinManualSteppedControl(double probabilityLightingwillbeResetWhenNeededinManualSteppedControl) {
  return getImpl<detail::DaylightingControl_Impl>()->setProbabilityLightingwillbeResetWhenNeededinManualSteppedControl(probabilityLightingwillbeResetWhenNeededinManualSteppedControl);
}

void DaylightingControl::resetProbabilityLightingwillbeResetWhenNeededinManualSteppedControl() {
  getImpl<detail::DaylightingControl_Impl>()->resetProbabilityLightingwillbeResetWhenNeededinManualSteppedControl();
}

bool DaylightingControl::setNumberofDaylightingViews(int numberofDaylightingViews) {
  return getImpl<detail::DaylightingControl_Impl>()->setNumberofDaylightingViews(numberofDaylightingViews);
}

void DaylightingControl::resetNumberofDaylightingViews() {
  getImpl<detail::DaylightingControl_Impl>()->resetNumberofDaylightingViews();
}

bool DaylightingControl::setMaximumAllowableDiscomfortGlareIndex(double maximumAllowableDiscomfortGlareIndex) {
  return getImpl<detail::DaylightingControl_Impl>()->setMaximumAllowableDiscomfortGlareIndex(maximumAllowableDiscomfortGlareIndex);
}

void DaylightingControl::resetMaximumAllowableDiscomfortGlareIndex() {
  getImpl<detail::DaylightingControl_Impl>()->resetMaximumAllowableDiscomfortGlareIndex();
}

Point3d DaylightingControl::position() const
{
  return getImpl<detail::DaylightingControl_Impl>()->position();
}

bool DaylightingControl::setPosition(const Point3d& position)
{
  return getImpl<detail::DaylightingControl_Impl>()->setPosition(position);
}

openstudio::Transformation DaylightingControl::transformation() const {
  return getImpl<detail::DaylightingControl_Impl>()->transformation();
}

bool DaylightingControl::setTransformation(const openstudio::Transformation& transformation)
{
  return getImpl<detail::DaylightingControl_Impl>()->setTransformation(transformation);
}

bool DaylightingControl::isPrimaryDaylightingControl() const
{
  return getImpl<detail::DaylightingControl_Impl>()->isPrimaryDaylightingControl();
}

bool DaylightingControl::isSecondaryDaylightingControl() const
{
  return getImpl<detail::DaylightingControl_Impl>()->isSecondaryDaylightingControl();
}

/// @cond
DaylightingControl::DaylightingControl(boost::shared_ptr<detail::DaylightingControl_Impl> impl)
  : SpaceItem(impl)
{}
/// @endcond


} // model
} // openstudio
