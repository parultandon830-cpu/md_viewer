; ============================================================
; MD Reader - Windows Installer
; Generated for Inno Setup
; ============================================================

#define MyAppName "MD Reader"
#define MyAppVersion "1.0.0"
#define MyAppPublisher "MD Reader Project"
#define MyAppExeName "MDReader.exe"

[Setup]
AppId={{D37B7E89-8C84-4867-8C5C-9A5C0E616B2E}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}

DefaultDirName={autopf}\{#MyAppName}
DefaultGroupName={#MyAppName}

OutputDir=Output
OutputBaseFilename=MDReader-Setup-v1.0

Compression=lzma2/ultra64
SolidCompression=yes

WizardStyle=modern
ChangesAssociations=yes

PrivilegesRequired=lowest
DisableProgramGroupPage=yes


; ============================================================
; Languages
; ============================================================

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"


; ============================================================
; Installation Tasks
; ============================================================

[Tasks]

; Desktop shortcut
Name: "desktopicon";
Description: "{cm:CreateDesktopIcon}";
GroupDescription: "{cm:AdditionalIcons}";
Flags: unchecked

; Markdown file association
Name: "associate_md";
Description: "Associate .md files with MD Reader (Make Default)";
GroupDescription: "File Associations:"

; Right-click context menu
Name: "context_menu";
Description: "Add 'Open with MD Reader' to right-click context menu";
GroupDescription: "Windows Shell Integration:"


; ============================================================
; Application Files
; ============================================================

[Files]

; Packages the compiled executable and all windeployqt
; runtime DLLs/resources from the Release directory.
Source: "build\Release\*";
DestDir: "{app}";
Flags: ignoreversion recursesubdirs createallsubdirs


; ============================================================
; Shortcuts
; ============================================================

[Icons]

; Start Menu shortcut
Name: "{group}\{#MyAppName}";
Filename: "{app}\{#MyAppExeName}"

; Start Menu uninstall shortcut
Name: "{group}\Uninstall {#MyAppName}";
Filename: "{uninstallexe}"

; Desktop shortcut
Name: "{autodesktop}\{#MyAppName}";
Filename: "{app}\{#MyAppExeName}";
Tasks: desktopicon


; ============================================================
; Registry
; ============================================================

[Registry]

; ------------------------------------------------------------
; 1. Markdown File Associations
; ------------------------------------------------------------

; .md association
Root: HKCU;
Subkey: "Software\Classes\.md";
ValueType: string;
ValueName: "";
ValueData: "MDReader.Document";
Flags: uninsdeletevalue;
Tasks: associate_md

; .markdown association
Root: HKCU;
Subkey: "Software\Classes\.markdown";
ValueType: string;
ValueName: "";
ValueData: "MDReader.Document";
Flags: uninsdeletevalue;
Tasks: associate_md

; MDReader document type
Root: HKCU;
Subkey: "Software\Classes\MDReader.Document";
ValueType: string;
ValueName: "";
ValueData: "Markdown Document";
Flags: uninsdeletekey;
Tasks: associate_md

; Default icon
Root: HKCU;
Subkey: "Software\Classes\MDReader.Document\DefaultIcon";
ValueType: string;
ValueName: "";
ValueData: "{app}\{#MyAppExeName},0";
Tasks: associate_md

; Open command
Root: HKCU;
Subkey: "Software\Classes\MDReader.Document\shell\open\command";
ValueType: string;
ValueName: "";
ValueData: """{app}\{#MyAppExeName}"" ""%1""";
Tasks: associate_md


; ------------------------------------------------------------
; 2. Right-Click Menu for .md Files
; ------------------------------------------------------------

Root: HKCU;
Subkey: "Software\Classes\SystemFileAssociations\.md\shell\OpenWithMDReader";
ValueType: string;
ValueName: "";
ValueData: "Open with MD Reader";
Flags: uninsdeletekey;
Tasks: context_menu

Root: HKCU;
Subkey: "Software\Classes\SystemFileAssociations\.md\shell\OpenWithMDReader";
ValueType: string;
ValueName: "Icon";
ValueData: "{app}\{#MyAppExeName},0";
Tasks: context_menu

Root: HKCU;
Subkey: "Software\Classes\SystemFileAssociations\.md\shell\OpenWithMDReader\command";
ValueType: string;
ValueName: "";
ValueData: """{app}\{#MyAppExeName}"" ""%1""";
Tasks: context_menu


; ------------------------------------------------------------
; 3. Right-Click Menu for .markdown Files
; ------------------------------------------------------------

Root: HKCU;
Subkey: "Software\Classes\SystemFileAssociations\.markdown\shell\OpenWithMDReader";
ValueType: string;
ValueName: "";
ValueData: "Open with MD Reader";
Flags: uninsdeletekey;
Tasks: context_menu

Root: HKCU;
Subkey: "Software\Classes\SystemFileAssociations\.markdown\shell\OpenWithMDReader";
ValueType: string;
ValueName: "Icon";
ValueData: "{app}\{#MyAppExeName},0";
Tasks: context_menu

Root: HKCU;
Subkey: "Software\Classes\SystemFileAssociations\.markdown\shell\OpenWithMDReader\command";
ValueType: string;
ValueName: "";
ValueData: """{app}\{#MyAppExeName}"" ""%1""";
Tasks: context_menu


; ============================================================
; Run Application After Installation
; ============================================================

[Run]

Filename: "{app}\{#MyAppExeName}";
Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}";
Flags: nowait postinstall skipifsilent
