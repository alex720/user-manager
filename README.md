# TeamSpeak 3 User Manager

### A plugin to assist you with moderating your TeamSpeak 3 channels

This is the brainchild of shitty720, he created the entirety of this plugin, I just ported it to modern MSVC and fixed up the file structure.

### Features
- Works on any server (You just need to enter the ID of the channel-admin,-mod,-ban groups)
- Give your buddies talk power automatically if you're channel-mod
- Give your buddies channel-mod automatically if you're channel-admin
- Ban users you blocked automatically from your channel if you're channel-mod or -admin
- Automatically mute users when you block them with the plugins
- Automatically remove channel-ban from your buddies when you're channel-admin
- Set a custom kick message

### Setup
1. Download and install the [latest release](https://github.com/aequabit/ts3-user-manager/releases/latest)
2. If you're updating User Manager, make sure TeamSpeak is closed while trying to install
3. Connect to the server you'd like to use User Manager on
4. Open channel groups window by clicking on `Permissions -> Channel groups`
5. Move the window to the far right of your screen so only the group names and IDs in `()` are visible
6. In your TeamSpeak client, go to `Tools -> Options -> Addons -> My addons -> Plugins`
7. Search for "User Manager" and open it's settings
8. (A faster way to this is to click on `Plugins -> User Manager -> Open Configuration`)
9. Check the "Current Server" checkbox
10. Enter the IDs of the groups in the channel groups window we opened earlier
11. Change the settings to your liking
12. Click all 3 buttons on the bottom of the window (importing contacts can cause lag, just be patient)

### Usage
- To block and ban a user from your channel, right-click on him and then go to `User Manager -> Set / remove client on the blocklist`. The process to unblock a user is the same.

### Building (Windows)
- Install Visual Studio 2022 and Qt 5.15.2
- Install [Qt Visual Studio Tools](https://marketplace.visualstudio.com/items?itemName=TheQtCompany.QtVisualStudioTools2022)
- Open `msvc/ts3-user-manager.sln` and build the solution
- To package the plugin as a `.ts3_plugin` file, run `Package-Plugin.ps1` in PowerShell

### Building (Linux)
- CMake and Qt5 :^)
