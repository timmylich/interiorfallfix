/*
 | English:
 - This include fixes players falling through interior floors and mapping. The issue is quite serious and has not been properly resolved in most places.
 - The script performs two functions that prevent falling in 99.9% of cases:
 - 1. Creates a static platform under the player before teleportation.
 - 2. If falling is detected after teleportation, the player is teleported back.

 | Русский:
 - Данный инклуд исправляет проваливания игроками под полы интерьеров и маппинг. Проблема достаточно серьезная и почти нигде полноценно не исправленная.
 - Скрипт выполняет сразу 2 функции, на 99.9%, исключающие проваливания:
 - 1. Создаёт под игроком статичную платформу перед телепортацией.
 - 2. При обнаружении падения игрока после телепортации - телепортирует его назад.

 - Author: timmylich. (originally for Brentwood Project)
 - Version: 2.0.
 - Date: 15 June 2025.
 - GitHub Repo: https://github.com/timmylich/interiorfallfix

 - Connect with author:
 - VKontake: https://vk.com/timmylich
 - Telegram: https://t.me/timmylich
 - Discord: http://discordapp.com/users/523177185062682685 (@timmylich.)
*/

#if defined _IFX_
    #endinput
#endif
#define _IFX_



/*
[EN] The time after teleportation during which the player will be checked for falling, in order to "rescue" them if necessary (in milliseconds).
[RU] Время, которое после телепортации будет проверяться падает ли игрок для его "спасения" (в миллисекундах).
*/
#if !defined IFX_FixTime
    #define IFX_FixTime 7000
#endif

/*
[EN] The duration for which the static platform will be created under the player in the interior (in milliseconds).
[RU] Время, на которое будет создаваться статичная платформа под игроком в интерьере (в миллисекундах).
*/
#if !defined IFX_InteriorPlatformTime
    #define IFX_InteriorPlatformTime 4000
#endif

/*
[EN] The duration for which the static platform will be created under the player in the exterior (in milliseconds).
[RU] Время, на которое будет создаваться статичная платформа под игроком в экстерьере (в миллисекундах).
*/
#if !defined IFX_InteriorPlatformTime
    #define IFX_ExteriorPlatformTime 2000
#endif

/*
[EN] Substitute your own function to get the player's interior, if necessary.
[RU] Подставьте свою функцию для получения интерьера игрока, если это необходимо.
*/
#if !defined IFX_GetPlayerInterior
    #define IFX_GetPlayerInterior(%0) GetPlayerInterior(%0)
#endif

/*
[EN] Timing between checks in OnPlayerUpdate (ms).
[RU] Тайминг между срабатываниями проверок в OnPlayerUpdate (в миллисекундах).
*/
#if !defined IFX_PlayerUpdateCD
    #define IFX_PlayerUpdateCD 250
#endif

/*
[EN] Z-axis offset downward at which the script will consider the player falling into the void.
[RU] Смещение по координате Z вниз, по достижению которого скрипт посчитает игрока падающим в бездну.
*/
#if !defined IFX_FallZShift
    #define IFX_FallZShift 0.2
#endif


/*
 | English:
 - Function for creating a static surface under the player. Place it inside the teleportation logic, like SetPlayerPosEx, filling in all required arguments (or -1 in certain situations).
 * oldworld - the player's virtual world before teleportation.
 * newworld - the player's virtual world after teleportation.
 * oldint - the player's interior before teleportation.
 * newint - the player's interior after teleportation.

 | Русский:
 - Функция создания статичной поверхности под игроком. Поместите её внутрь телепортации, по типу SetPlayerPosEx, заполнив все нужные аргументы (либо -1 в некоторых ситуациях).
 * oldworld - виртуальный мир игрока до телепортации.
 * newworld - виртуальный мир игрока после телепортации.
 * oldint - интерьер игрока до телепортации.
 * newint - интерьер игрока после телепортации.
*/
stock IFX_GeneratePlatform(playerid, oldworld = -1, newworld = -1, oldint = -1, newint = -1, Float:x, Float:y, Float:z)
{
    if((newworld >= 0 && newworld != oldworld) || (newint >= 0 && newint != oldint))
	{
        new Float:_a;
        GetPlayerFacingAngle(playerid, _a);
        new obj = CreatePlayerObject(playerid, 19890, x, y, z-1.0, 0.0, 0.0, _a, 10000.0);
        SetPlayerObjectMaterial(playerid, obj, 0, 19962, !"samproadsigns", !"materialtext1", 0x00000000);
        SetTimerEx(!"IFX_ObjectDeletion", newint ? IFX_InteriorPlatformTime : IFX_ExteriorPlatformTime, false, !"dd", playerid, obj);
    }
}
forward IFX_ObjectDeletion(playerid, objectid);
public IFX_ObjectDeletion(playerid, objectid) return DestroyPlayerObject(playerid, objectid);


static _disabled;
/*
 | English:
 - Use before teleportation if you need to disable the fall recovery attempt.

 | Русский:
 - Используйте перед телепортацией, если вам необходимо отключить попытку работы "спасалки" от падений.
*/
stock IFX_DisableAttempt() _disabled = true;


static Float:LastTP[MAX_PLAYERS][3];
static LastTPTime[MAX_PLAYERS];
// SetPlayerPos hook
stock IFX_SetPlayerPos(playerid, Float:x, Float:y, Float:z)
{
    new Float:_x, Float:_y, Float:_z;
    GetPlayerPos(playerid, _x, _y, _z);
    if(!_disabled && (!(-0.5 <= (x-_x) <= 0.5) || !(-0.5 <= (y-_y) <= 0.5) || !(-5.0 <= (z-_z) <= 5.0)))
	{
		LastTPTime[playerid] = GetTickCount();
    
        LastTP[playerid][0] = x;
        LastTP[playerid][1] = y;
        LastTP[playerid][2] = z;
	}
    else if(_disabled) _disabled = false;
   
    return SetPlayerPos(playerid, x, y, z);
}
#if defined _ALS_SetPlayerPos
	#undef SetPlayerPos
#else
	#define _ALS_SetPlayerPos
#endif
#define SetPlayerPos IFX_SetPlayerPos


static LastPlayerUpdate[MAX_PLAYERS];
// OnPlayerUpdate hook
public OnPlayerUpdate(playerid)
{
    if(GetTickCount()-IFX_PlayerUpdateCD > LastPlayerUpdate[playerid])
    {
        LastPlayerUpdate[playerid] = GetTickCount();
        if(LastTPTime[playerid]+IFX_FixTime > GetTickCount() && IFX_GetPlayerInterior(playerid))
        {
            static Float:x, Float:y, Float:z;
            GetPlayerPos(playerid, x, y, z);

            if((-0.1 <= (x - LastTP[playerid][0]) <= 0.1) && (-0.1 <= (y - LastTP[playerid][1]) <= 0.1))
            {
                if(LastTP[playerid][2]-IFX_FallZShift > z)
                {
                    SetPlayerPos(playerid, LastTP[playerid][0], LastTP[playerid][1], LastTP[playerid][2]);
                    return 1;
                }
            }
        }
    }

	#if defined IFX_OnPlayerUpdate
		return IFX_OnPlayerUpdate(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerUpdate
	#undef OnPlayerUpdate
#else
	#define _ALS_OnPlayerUpdate
#endif
#define OnPlayerUpdate IFX_OnPlayerUpdate
#if defined IFX_OnPlayerUpdate
	forward IFX_OnPlayerUpdate(playerid);
#endif