/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

// Core includes
#include <qSlicerExtensionsManagerModel.h>
#include <qSlicerApplication.h>
#include <qSlicerCorePythonManager.h>

// Qt includes
#include <QtPlugin>
#include <QtCore>

// SlicerOpenCV Logic includes
#include <vtkSlicerOpenCVLogic.h>

// SlicerOpenCV includes
#include "qSlicerOpenCVModule.h"

// ITK includes

//-----------------------------------------------------------------------------
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
  #include <QtPlugin>
  Q_EXPORT_PLUGIN2(qSlicerOpenCVModule, qSlicerOpenCVModule);
#endif

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerOpenCVModulePrivate
{
public:
  qSlicerOpenCVModulePrivate();
};

//-----------------------------------------------------------------------------
// qSlicerOpenCVModulePrivate methods

//-----------------------------------------------------------------------------
qSlicerOpenCVModulePrivate::qSlicerOpenCVModulePrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerOpenCVModule methods

//-----------------------------------------------------------------------------
qSlicerOpenCVModule::qSlicerOpenCVModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qSlicerOpenCVModulePrivate)
{
}

//-----------------------------------------------------------------------------
qSlicerOpenCVModule::~qSlicerOpenCVModule()
{
}

//-----------------------------------------------------------------------------
QString qSlicerOpenCVModule::helpText() const
{
  return "This is a loadable module that provides access to OpenCV";
}

//-----------------------------------------------------------------------------
QString qSlicerOpenCVModule::acknowledgementText() const
{
  return "This work was supported by a supplement to the Quantitative Image Informatics project via the NIH-National Cancer Institute Grant U24 CA180918-03 as well as U24 CA180924 Tools to Analyze Morphology and Spatially Mapped Molecular Data with Stony Brook University.";
}

//-----------------------------------------------------------------------------
QStringList qSlicerOpenCVModule::contributors() const
{
  QStringList moduleContributors;
  moduleContributors << QString("Nicole Aucoin (BWH)");
  moduleContributors << QString("Andrey Fedorov (BWH)");
  moduleContributors << QString("Jean-Christophe Fillion-Robin (Kitware)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qSlicerOpenCVModule::icon() const
{
  return QIcon(":/Icons/SlicerOpenCV.png");
}

//-----------------------------------------------------------------------------
QStringList qSlicerOpenCVModule::categories() const
{
  return QStringList() << "Libraries";
}

//-----------------------------------------------------------------------------
QStringList qSlicerOpenCVModule::dependencies() const
{
  return QStringList();
}

//-----------------------------------------------------------------------------
void qSlicerOpenCVModule::setup()
{
  this->Superclass::setup();

  qSlicerExtensionsManagerModel* model = qSlicerApplication::application()->extensionsManagerModel();
  QString settingsPath = model->extensionsSettingsFilePath();
  QSettings settings(settingsPath, QSettings::IniFormat);
  if (settings.status() != QSettings::NoError)
  {
    qWarning() << (QString("Failed to open extensions settings file %1. Python access to OpenCV will not be available.").arg(settingsPath));
    return;
  }

  // Determine this extensions root install path
  QString path = model->extensionInstallPath("SlicerOpenCV");

  // Check if path already exists
  QStringList pythonPaths = qSlicerExtensionsManagerModel::readArrayValues(settings, "PYTHONPATH", "path");
  for (QStringList::const_iterator iter = pythonPaths.constBegin(); iter != pythonPaths.constEnd(); ++iter)
  {
    if (*iter == path)
    {
      // Found, no need to continue
      return;
    }
  }
  pythonPaths << path;
  qSlicerExtensionsManagerModel::writeArrayValues(settings, pythonPaths, "PYTHONPATH", "path");

  // Add the path to this running instances path list
  qSlicerApplication::application()->corePythonManager()->appendPythonPath(path);
}

//-----------------------------------------------------------------------------
qSlicerAbstractModuleRepresentation* qSlicerOpenCVModule
::createWidgetRepresentation()
{
  return NULL;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qSlicerOpenCVModule::createLogic()
{
  return vtkSlicerOpenCVLogic::New();
}

//-----------------------------------------------------------------------------
bool qSlicerOpenCVModule::isHidden()const
{
  return true;
}
