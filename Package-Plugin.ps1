#msbuild ts3-user-manager.sln -p:Configuration=Release -p:Platform=x64

Remove-Item -Force -ErrorAction SilentlyContinue "./ts3-user-manager.ts3_plugin"
Remove-Item -Force -Recurse -ErrorAction SilentlyContinue "./package/plugins"

New-Item -Name "plugins" -ItemType Directory -Path "./package/"

copy "./build/x64/Release/ts3-user-manager_win64.dll" "./package/plugins/"

Compress-Archive -Path './package/*' -DestinationPath "ts3-user-manager.zip" 

Compress-Archive "./package" "ts3-user-manager.zip"

move "ts3-user-manager.zip" "ts3-user-manager.ts3_plugin"
