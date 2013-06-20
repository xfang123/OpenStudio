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

#ifndef STANDARDSINTERFACE_TEST_STANDARDSINTERFACEFIXTURE_HPP
#define STANDARDSINTERFACE_TEST_STANDARDSINTERFACEFIXTURE_HPP

#include <gtest/gtest.h>

#include <utilities/core/Logger.hpp>
#include <utilities/core/FileLogSink.hpp>
#include <utilities/core/Path.hpp>

#include <boost/optional.hpp>

#include <resources.hxx>

namespace openstudio {
namespace model {
class Model;
}
}

class StandardsInterfaceFixture : public ::testing::Test {
 protected:

  // TEST FIXTURE METHODS

  /// initialize for each test
  virtual void SetUp();

  /// tear down after each test
  virtual void TearDown();

  /// initiallize static members
  static void SetUpTestCase();

  /// tear down static members
  static void TearDownTestCase();

  /// set up logging
  REGISTER_LOGGER("StandardsInterfaceFixture");

  /// static data
  static boost::optional<openstudio::FileLogSink> logFile;
  static openstudio::path runDir;

  static openstudio::model::Model exampleModel(bool withResults,bool clearModels=false);

  virtual ~StandardsInterfaceFixture() {}
};

#endif // STANDARDSINTERFACE_TEST_STANDARDSINTERFACEFIXTURE_HPP