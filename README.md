# AsaApi.Plugins.Template
Simple Template to make AsaAPI plugins

# PLEASE ⭐STAR⭐ THE REPO IF YOU LIKE IT! THANKS!

Watch Video Tutorial
- Part1 https://youtu.be/m80BljxIqcA
- Part2 https://youtu.be/gsWLgR6kFsQ
- Part3 https://youtu.be/3UwMR8t4pHQ
- Part4 https://youtu.be/o1odrRlZsNo
- Part5 https://youtu.be/QhI3F0Bpl_0
- Part6 https://youtu.be/4kzAjWrjO6g (Broken Audio)
- Part6 https://youtu.be/2iqkIE4nwUs (Fixed)
- Part7 https://youtu.be/Y37ZDl7kN_A

Required
- Clone, compile, and link library https://ark-server-api.com/resources/asa-server-api.31/
- Install Visual Studio required components https://visualstudio.microsoft.com/
- Using Github Desktop https://desktop.github.com/download/
- Using Git-SCM https://git-scm.com/
- How to host your game server with WindowsGSM https://www.youtube.com/watch?v=5aL7jJgkpuw
- Getting offsets and dumper https://dumper.pelayori.com/
- Windows console application for RCON https://github.com/Tiiffi/mcrcon
- Validate your json file here https://jsonlint.com/
- Download SQLite amalgamation - https://www.sqlite.org/download.html
- SQLite DB Browser - https://sqlitebrowser.org/dl/
- SQLite Docs - https://sqlite.org/docs.html

You can download other plugins here https://ark-server-api.com/resources/

TODO: `replace <PROJECT_NAME> with your Project name`
- vcpkg.json line 2 replace "name": "asa-api-plugin-template" to "asa-api-plugin-<PROJECT_NAME>"
- PluginTemplate.sln line 6 Project("{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}") = "<PROJECT_NAME>", "AsaApi.Plugins.<PROJECT_NAME>.vcxproj", "{F7095F8F-7BBD-40A5-B61D-B442C92A88B2}"
- AsaApi.Plugins.<PROJECT_NAME>.vcxproj.filters
	- line 45 <ClCompile Include="Source\<PROJECT_NAME>.cpp">
	- line 59 <ClInclude Include="Source\Public\<PROJECT_NAME>.h">
- AsaApi.Plugins.<PROJECT_NAME>.vcxproj
	- line 20 <ClCompile Include="Source\<PROJECT_NAME>.cpp" />
	- line30 <ClInclude Include="Source\Public\<PROJECT_NAME>.h" />
	- line40 <RootNamespace>AsaApi<PROJECT_NAME></RootNamespace>
	- line42 <ProjectName><PROJECT_NAME></ProjectName>
	- line81 ASAAPIPLUGINS<PROJECT_NAME>_EXPORTS
- Configs/PluginInfo.json replace FullName and Description

- Find and Replace all .h and .cpp files namespaces PluginTemplate

- AsaApi.Plugins.<PROJECT_NAME>.vcxproj
- AsaApi.Plugins.<PROJECT_NAME>.vcxproj.filter
- AsaApi.Plugins.<PROJECT_NAME>.vcxproj.user
- <PROJECT_NAME>.sln
- Source/<PROJECT_NAME>.cpp
- Source/Public/<PROJECT_NAME>.h

Notes: 
- if you dont wanna do all of this just run `Install.bat`
- if this repo is existing in your machine and you just created a different repo without using this template you can just run `_Copy.bat` and specified your directory path
