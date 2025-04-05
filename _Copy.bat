@echo off
setlocal

:: Define source and destination directories
set "SOURCE_DIR=%~dp0"
set /p DEST_DIR=Enter Project Path enclosed with quotes: 

:: Create destination directory if it does not exist
if not exist "%DEST_DIR%" mkdir "%DEST_DIR%"

:: Copy directories
xcopy "%SOURCE_DIR%\Configs" "%DEST_DIR%\Configs" /s /i /e
xcopy "%SOURCE_DIR%\Includes" "%DEST_DIR%\Includes" /s /i /e
xcopy "%SOURCE_DIR%\Libs" "%DEST_DIR%\Libs" /s /i /e
xcopy "%SOURCE_DIR%\Source" "%DEST_DIR%\Source" /s /i /e

:: Copy files
copy "%SOURCE_DIR%\gitignore" "%DEST_DIR%\gitignore"
copy "%SOURCE_DIR%\_Install.bat" "%DEST_DIR%\_Install.bat"
copy "%SOURCE_DIR%\AsaApi.Plugins.Template.vcxproj" "%DEST_DIR%\AsaApi.Plugins.Template.vcxproj"
copy "%SOURCE_DIR%\AsaApi.Plugins.Template.vcxproj.filters" "%DEST_DIR%\AsaApi.Plugins.Template.vcxproj.filters"
copy "%SOURCE_DIR%\AsaApi.Plugins.Template.vcxproj.user" "%DEST_DIR%\AsaApi.Plugins.Template.vcxproj.user"
copy "%SOURCE_DIR%\PluginTemplate.sln" "%DEST_DIR%\PluginTemplate.sln"
copy "%SOURCE_DIR%\vcpkg.json" "%DEST_DIR%\vcpkg.json"
copy "%SOURCE_DIR%\vcpkg-configuration.json" "%DEST_DIR%\vcpkg-configuration.json"

echo Files and directories copied successfully.
pause