@echo off
setlocal

:: Create the Prepare directory
echo Creating the Prepare directory...
mkdir "%~dp0Prepare"

:: Change to the Prepare directory
echo Changing to the Prepare directory...
cd /d "%~dp0Prepare"

:: Get user input
set /p PROJECT_NAME=Enter Project name (e.g., "Project1"): 
set /p PROJECT_DESC=Enter Project Description: 

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

endlocal