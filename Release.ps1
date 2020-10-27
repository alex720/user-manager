msbuild User-Manager.sln  -p:Configuration=Release -p:Platform=win32
msbuild User-Manager.sln  -p:Configuration=Release -p:Platform=x64

Get-ChildItem -Force -Path "./User-Manager" -Directory | Remove-Item -Force -Recurse
Remove-Item -Force "User-Manager.ts3_plugin"

New-Item -Name "User-Manager" -ItemType Directory -Path "./"
New-Item -Name "plugins" -ItemType Directory -Path "./User-Manager/"


#copy ".\x64\Release\user manager_win64.dll" "./User-Manager/plugins/"
#copy ".\Win32\Release\user manager_win32.dll" "./User-Manager/plugins/"
copy "./package.ini" "./User-Manager/"

$quelle = ".\User-Manager\*"
$ziel = ".\User-Manager.zip"
Add-Type -assembly "system.io.compression.filesystem"
[System.IO.Compression.ZipFile]::CreateFromDirectory($quelle, $ziel) 

Compress-Archive  -Path 'User-Manager/*' -DestinationPath "User-Manager.zip" 

move "User-Manager.zip" "User-Manager.ts3_plugin"
