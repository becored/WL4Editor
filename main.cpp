#include "Dialog/DoorConfigDialog.h"
#include "Dialog/RoomConfigDialog.h"
#include "Dialog/PatchEditDialog.h"
#include "Dialog/CreditsEditDialog.h"
#include "DockWidget/CameraControlDockWidget.h"
#include "WL4Application.h" // #include <QApplication>
#include "WL4EditorWindow.h"

/// <summary>
/// Perform all static class initializations.
/// </summary>
static void StaticInitialization_BeforeROMLoading()
{
    RoomConfigDialog::StaticComboBoxesInitialization();
    DoorConfigDialog::StaticInitialization();
    CameraControlDockWidget::StaticInitialization();
    CreditsEditDialog::StaticInitialization();
    PatchEditDialog::StaticComboBoxesInitialization();
}

/// <summary>
/// Perform static initializations, and then create the main window for the application.
/// </summary>
/// <param name="argc">
/// Number of command line arguments.
/// </param>
/// <param name="argv">
/// Array of command line arguments.
/// </param>
int main(int argc, char *argv[])
{
    StaticInitialization_BeforeROMLoading();

    // High DPI support, perhaps won't work in all the OS but better than nothing
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication application(argc, argv);
    SettingsUtils::InitProgramSetupPath(application);
    application.setWindowIcon(QIcon("./images/icon.ico"));
    WL4EditorWindow window;
    window.show();

    return application.exec();
}
