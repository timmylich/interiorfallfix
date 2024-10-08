# Interior Fall Fix
English | [Русский](README.ru.md)

# What is this?

This include helps fix the annoying issue where a player, after teleporting to an interior, falls through the floor due to connection problems. The script will teleport the player back to their last teleportation location if it detects that the player is falling.

To apply the fix, simply include the `InteriorFallFix.inc` file in your code. Do this after your main includes and anti-cheat hooks.
```pawn
#include <InteriorFallFix>
```

Demo video:
[![Watch the video](https://img.youtube.com/vi/-lnWa3OpdjA/maxresdefault.jpg)](https://youtu.be/-lnWa3OpdjA)

# Configuration
The include has several definitions that allow you to configure it. To change a value, define it before including the file, or modify it directly inside the include.

The time (in milliseconds) after the last teleport during which the script will check if the player is falling:
```pawn
#define ifx_FixTime 7000
```
Where to get information about which interior the player is in:
```pawn
#define ifx_GetPlayerInterior(%0) GetPlayerInterior(%0)
```
It is recommended to set up your own function or variable to properly work with anti-cheat, preventing unauthorized movement between interiors.

The delay between checks in `OnPlayerUpdate` (in milliseconds):
```pawn
#define ifx_PlayerUpdateCD 250
```
Lower values mean more frequent checks and reduced performance.

The player’s fall velocity threshold that triggers the script:
```pawn
#define ifx_FallVelocity 0.3
```

# Note

This include will not completely fix the issue – this is only possible if you use it in combination with freezing players during teleportation or if your interiors have static platforms (CreateObject).

Author - timmylich. | [VKontakte](vk.com/timmylich) | [Telegram](t.me/timmylich)