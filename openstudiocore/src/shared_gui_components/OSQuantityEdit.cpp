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

#include <shared_gui_components/OSQuantityEdit.hpp>

#include <model/ModelObject_Impl.hpp>

#include <utilities/core/Containers.hpp>
#include <utilities/core/StringHelpers.hpp>
#include <utilities/data/Attribute.hpp>
#include <utilities/document/DocumentRegex.hpp>
#include <utilities/units/Quantity.hpp>

#include <QVBoxLayout>
#include <QHBoxLayout>

using openstudio::model::ModelObject;

namespace openstudio {

OSQuantityEdit::OSQuantityEdit(bool isIP, QWidget * parent)
  : m_lineEdit(new QLineEdit(parent)),
    m_units(new QLabel("",parent)),
    m_isIP(isIP),
    m_isScientific(false)
{
  this->setAcceptDrops(false);
  m_lineEdit->setAcceptDrops(false);
  setEnabled(false);

  QHBoxLayout * hLayout = new QHBoxLayout();
  setLayout(hLayout);
  hLayout->setContentsMargins(0,0,0,0);
  hLayout->addWidget(m_lineEdit);
  hLayout->addWidget(m_units);

  m_lineEdit->setFixedWidth(90);

  setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);
}

void OSQuantityEdit::bind(model::ModelObject& modelObject,
                          const char* property,
                          bool isIP,
                          const boost::optional<std::string>& isDefaultedProperty,
                          const boost::optional<std::string>& isAutosizedProperty,
                          const boost::optional<std::string>& isAutocalculatedProperty)
{
  m_modelObject = modelObject;
  m_property = property;
  m_isIP = isIP;
  m_isDefaultedProperty = isDefaultedProperty;
  m_isAutosizedProperty = isAutosizedProperty;
  m_isAutocalculatedProperty = isAutocalculatedProperty;

  // only let one of autosize/autocalculate
  if (isAutosizedProperty && isAutocalculatedProperty) {
    LOG_AND_THROW("A field can only be autosized or autocalculated, it cannot be both.");
  }

  // check for attribute existence
  StringVector attributeNames = modelObject.attributeNames();
  StringVector::const_iterator anb(attributeNames.begin()),ane(attributeNames.end());
  BOOST_ASSERT(std::find(anb,ane,m_property + "_SI") != ane);
  BOOST_ASSERT(std::find(anb,ane,m_property + "_IP") != ane);
  if (m_isDefaultedProperty) {
    BOOST_ASSERT(std::find(anb,ane,*m_isDefaultedProperty) != ane);
  }
  if (m_isAutosizedProperty) {
    BOOST_ASSERT(std::find(anb,ane,*m_isAutosizedProperty) != ane);
  }
  if (m_isAutocalculatedProperty) {
    BOOST_ASSERT(std::find(anb,ane,*m_isAutocalculatedProperty) != ane);
  }
  
  setEnabled(true);
  
  bool isConnected = false;
  isConnected = connect( m_lineEdit, SIGNAL(editingFinished()), this, SLOT(onEditingFinished()) );
  BOOST_ASSERT(isConnected);

  isConnected = connect( m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get(),SIGNAL(onChange()),
                         this,SLOT(onModelObjectChange()) );
  BOOST_ASSERT(isConnected);

  isConnected = connect( m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get(),SIGNAL(onRemoveFromWorkspace(Handle)),
                         this,SLOT(onModelObjectRemove(Handle)) );
  BOOST_ASSERT(isConnected);

  refreshTextAndLabel();
}

void OSQuantityEdit::unbind() {
  if (m_modelObject){
    this->disconnect(m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get());
    m_modelObject.reset();
    m_property = "";
    setEnabled(false);
  }
}

void OSQuantityEdit::onEditingFinished() {
  if (m_modelObject) {
    std::string str = m_lineEdit->text().toStdString();
    boost::regex autore("[aA][uU][tT][oO]");
    ModelObject modelObject = m_modelObject.get();

    if (str.empty()) {
      modelObject.resetAttribute(m_property);
    }
    else if (boost::regex_search(str,autore)) {
      if (m_isAutosizedProperty) {
        if (modelObject.isSettableAttribute(*m_isAutosizedProperty)) {
          modelObject.setAttribute(*m_isAutosizedProperty,true);
        }
        else {
          modelObject.resetAttribute(m_property);
        }
      }
      if (m_isAutocalculatedProperty) {
        if (modelObject.isSettableAttribute(*m_isAutocalculatedProperty)) {
          modelObject.setAttribute(*m_isAutocalculatedProperty,true);
        }
        else {
          modelObject.resetAttribute(m_property);
        }
      }
    }
    else {
      try {
        double value = boost::lexical_cast<double>(str);
        setPrecision(str);
        OptionalUnit units;
        Attribute attr = modelObject.getAttribute(propertyWithSystem()).get();
        if (attr.valueType() == AttributeValueType::Quantity) {
          units = attr.valueAsQuantity().units();
        }
        else {
          BOOST_ASSERT(attr.valueType() == AttributeValueType::Unit);
          units = attr.valueAsUnit();
        }
        modelObject.setAttribute(propertyWithSystem(),Quantity(value,*units));
      }
      catch (...) {}
    }
  }
}
  
void OSQuantityEdit::onModelObjectChange() {
  refreshTextAndLabel();
}
  
void OSQuantityEdit::onUnitSystemChange(bool isIP) {
  m_isIP = isIP;
  refreshTextAndLabel();
}
  
void OSQuantityEdit::onModelObjectRemove(Handle handle) {
  m_modelObject.reset();
  m_property = "";
  setEnabled(false);
}

std::string OSQuantityEdit::propertyWithSystem() const {
  std::string result(m_property);
  if (m_isIP) {
    result += "_IP";
  }
  else {
    result += "_SI";
  }
  return result;
}

void OSQuantityEdit::refreshTextAndLabel() {
  if (m_modelObject) {
    QString textValue;
    ModelObject modelObject = m_modelObject.get();
    std::stringstream ss;

    if (m_isAutosizedProperty) {
      Attribute autosized = modelObject.getAttribute(*m_isAutosizedProperty).get();
      if (autosized.valueAsBoolean()) {
        textValue = QString("autosize");
        m_units->setStyleSheet("color:grey");
      }
    }

    if (m_isAutocalculatedProperty) {
      Attribute autocalculated = modelObject.getAttribute(*m_isAutocalculatedProperty).get();
      if (autocalculated.valueAsBoolean()) {
        textValue = QString("autocalculate");
        m_units->setStyleSheet("color:grey");
      }
    }

    Attribute attribute = modelObject.getAttribute(propertyWithSystem()).get();
    OptionalUnit units;
    if (attribute.valueType() == AttributeValueType::Quantity) {
      Quantity q = attribute.valueAsQuantity();
      if (m_isScientific) {
        ss << std::scientific;
      }
      else {
        ss << std::fixed;
      }
      if (m_precision) {
        ss << std::setprecision(*m_precision);
      }
      ss << q.value();
      textValue = toQString(ss.str());
      ss.str("");
      units = q.units();
      m_units->setStyleSheet("color:black");
    }
    else {
      BOOST_ASSERT(attribute.valueType() == AttributeValueType::Unit);
      units = attribute.valueAsUnit();
      m_units->setStyleSheet("color:grey");
    }

    m_lineEdit->setText(textValue);
    ss << *units;
    m_units->setTextFormat(Qt::RichText);
    m_units->setText(toQString(formatUnitString(ss.str(),DocumentFormat::XHTML)));

    if (m_isDefaultedProperty) {
      Attribute defaulted = modelObject.getAttribute(*m_isDefaultedProperty).get();
      if (defaulted.valueAsBoolean()) {
        m_lineEdit->setStyleSheet("color:green");
      }
      else {
        m_lineEdit->setStyleSheet("color:black");
      }
    }
  }
}

void OSQuantityEdit::setPrecision(const std::string& str) {
  boost::regex rgx("-?([[:digit:]]*)(\\.)?([[:digit:]]+)([EDed][-\\+]?[[:digit:]]+)?");
  boost::smatch m;
  if (boost::regex_match(str,m,rgx)) {
    std::string sci, prefix, postfix;
    if (m[1].matched) {
      prefix = std::string(m[1].first,m[1].second);
    }
    if (m[3].matched) {
      postfix = std::string(m[3].first,m[3].second);
    }
    if (m[4].matched) {
      sci = std::string(m[4].first,m[4].second);
    }
    m_isScientific = !sci.empty();

    if (m_isScientific) {
      m_precision = prefix.size() + postfix.size() - 1;
    }
    else {
      if (m[2].matched) {
        m_precision = postfix.size();
      }
      else {
        m_precision = 0;
      }
    }
  }
  else {
    m_isScientific = false;
    m_precision.reset();
  }
}

} // openstudio
