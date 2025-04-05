@echo off
setlocal

:: Define source and destination directories
set "SOURCE_DIR=%~dp0"
set /p DEST_DIR=Enter Project Path enclosed with quotes: 
set /p PROJECT_NAME=Enter Project name (e.g., "Project1"): 
set /p PROJECT_DESC=Enter Project Description: 

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

cd %DEST_DIR%

:: Create the Prepare directory
echo Creating the Prepare directory...
mkdir "%~dp0Prepare"

:: Change to the Prepare directory
echo Changing to the Prepare directory...
cd /d "%~dp0Prepare"

:: Check if the repository already exists
if exist ".git" (
    echo Pulling the latest changes from Git...
    git pull
) else (
    echo Cloning from Git...
    git clone "https://github.com/ohmcodes/Tools.StringManipulation.git" .
)

:: Create config-dev.json if it does not exist
if not exist "%~dp0Configs\config-dev.json" (
    echo {} > "%~dp0Configs\config-dev.json"
    echo Created config-dev.json in Configs directory.
)

:: Run the Node.js script with parameters
echo Running Node.js script...
call npm install

for /f "tokens=*" %%a in ('node index.js "%PROJECT_NAME%" "%PROJECT_DESC%"') do (
    echo %%a
)

:: Delete the Prepare directory
echo Deleting the Prepare directory...
cd /d "%~dp0"
rmdir /s /q "%~dp0Prepare"


echo All tasks completed.
pause


:: Delete this batch file
echo Deleting this batch file...
del "%~f0"




pause

endlocal