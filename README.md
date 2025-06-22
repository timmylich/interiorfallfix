## [Читать на русском](README.ru.md)

# What is this?

This include allows you to forget once and for all about the problem of players falling through the floor in interiors, as well as prolonged loading during which the player arrives frozen.

The script contains 2 main functions:
1. **Creating a static platform** - before teleportation, an invisible platform is created under the player that prevents falling through the floor.
2. **"Rescue" system** - if the player still starts falling after teleportation, the script automatically teleports them back to the location of their last teleportation.

### Demo video:
[![Watch the video](https://img.youtube.com/vi/mz9TV2BE4ww/maxresdefault.jpg)](https://youtu.be/mz9TV2BE4ww)

# Installation

To apply the fix, simply include the `InteriorFallFix.inc` file in your code. Do this after your main includes and anti-cheat hooks.
```cpp
#include <InteriorFallFix>
```

- If your mode has freezes when teleporting players between interiors - disable them.

For the rescue platform generation to work, you need to insert a call to `IFX_GeneratePlatform` in the function that also receives the virtual world along with the interior when teleporting the player. Usually it's called `SetPlayerPosEx`.

The function accepts the following parameters:
- `playerid` - player ID
- `oldworld` - player's virtual world before teleportation (or -1)
- `newworld` - player's virtual world after teleportation (or -1)
- `oldint` - player's interior before teleportation (or -1)
- `newint` - player's interior after teleportation (or -1)
- `x, y, z` - teleportation coordinates

## Additional functions

### IFX_DisableAttempt()
Use before teleportation if you need to disable the fall rescue attempt:
```cpp
IFX_DisableAttempt();
// Your teleportation
SetPlayerPos(playerid, x, y, z);
```

### Automatic SetPlayerPos interception
The script automatically intercepts the `SetPlayerPos` function, so no additional setup is required. With each call to `SetPlayerPos`, the script automatically tracks teleportations and saves coordinates for subsequent fall checks.

# Configuration
The include has several definitions that allow you to configure it. To change a value, define it before including the file, or modify it directly inside the include.

The time (in milliseconds) after the last teleport during which the script will check if the player is falling:
```cpp
#define IFX_FixTime 7000
```

The time for which a static platform will be created under the player in the interior (in milliseconds):
```cpp
#define IFX_InteriorPlatformTime 4000
```

The time for which a static platform will be created under the player in the exterior (in milliseconds):
```cpp
#define IFX_ExteriorPlatformTime 2000
```

Where to get information about which interior the player is in:
```cpp
#define IFX_GetPlayerInterior(%0) GetPlayerInterior(%0)
```
It is recommended to set up your own function or variable to properly work with anti-cheat, preventing unauthorized movement between interiors (if your anti-cheat doesn't intercept `GetPlayerInterior`).

The delay between script checks in OnPlayerUpdate (in milliseconds):
```cpp
#define IFX_PlayerUpdateCD 250
```
Lower values mean more frequent checks and reduced performance.

The Z coordinate offset downward, upon reaching which the script will consider the player falling into the abyss:
```cpp
#define IFX_FallZShift 0.2
```

### Authors

**timmylich. | [VKontakte](vk.com/timmylich) | [Telegram](t.me/timmylich) | [Discord](http://discordapp.com/users/523177185062682685)**

**Originally for [Brentwood Project](bw-p.ru)**