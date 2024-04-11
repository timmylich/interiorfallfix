/*
- Исправление провалов в интерьере путём телепортации игрока обратно на место позиции последней телепортации.
- Автор: timmylich.
- ВКонтакте - @timmylich, TG - @timmylich.
- Кто спиздит себе - тот пидор и за тем уже выехали.

- Данное исправление работает по принципу "подключил и забыл".
- То есть для его работы достаточно будет просто подключить его как include сразу после основных включений(a_samp или open.mp).
- Вы можете изменить настройки скрипта с помощью ряда define'ов.
- Для полного исправления проваливания игроков под пол рекомендуется сделать полы в интерьерах статичеными(CreateObject), а также ввести систему расчёта заморозки игрока при входе в интерьер на основе его пинга.

- Версия: 1.0.
- Дата обновления: 11.04.2024.
- https://github.com/timmylich/interiorfallfix
*/


#if !defined ifx_FixTime // Время, которое после телепортации будет проверяться падает ли игрок в миллисекундах
    #define ifx_FixTime 10000
#endif

#if !defined ifx_GetPlayerInterior // Откуда брать информацию о том, в каком интеьере находится игрок, рекомендую заменить на свою переменную для слаженной работы с античитом
    #define ifx_GetPlayerInterior(%0) GetPlayerInterior(%0)
#endif

#if !defined ifx_PlayerUpdateCD // Задержка между проверками OnPlayerUpdate в миллисекундах
    #define ifx_PlayerUpdateCD 250
#endif

static Float:LastTP[MAX_PLAYERS][3]; // Координаты последней телепортации
static LastTPTime[MAX_PLAYERS]; // Время последней телепортации игрока


// Перехват SetPlayerPos
stock ifx_SetPlayerPos(playerid, Float:x, Float:y, Float:z)
{
    LastTPTime[playerid] = GetTickCount(); // Сохраняем время последнего телепорта
    // Сохраняем позицию последнего телепорта
    LastTP[playerid][0] = x;
    LastTP[playerid][1] = y;
    LastTP[playerid][2] = z;
    return SetPlayerPos(playerid, x, y, z);
}
#if defined _ALS_SetPlayerPos
	#undef SetPlayerPos
#else
	#define _ALS_SetPlayerPos
#endif
#define SetPlayerPos ifx_SetPlayerPos


static LastPlayerUpdate[MAX_PLAYERS]; // Тик последнего срабатывания проверки на падение
// Перехват коллбэка обновления игрока
public OnPlayerUpdate(playerid)
{
    if(GetTickCount()-ifx_PlayerUpdateCD > LastPlayerUpdate[playerid]) // Проверка на задержку между срабатываниями
    {
        LastPlayerUpdate[playerid] = GetTickCount();
        if(LastTPTime[playerid]+ifx_FixTime > GetTickCount() && ifx_GetPlayerInterior(playerid)) // Проверка на прошедшее время и находится ли игрок в интерьере
        {
            static Float:FallVelocity;
            GetPlayerVelocity(playerid, FallVelocity, FallVelocity, FallVelocity); // Сохраняем скорость падения по оси Z

            if(FallVelocity < -0.3) // Проверяем скорость падения
            {
                SetPlayerPos(playerid, LastTP[playerid][0], LastTP[playerid][1], LastTP[playerid][2]);
            }
        }
    }

	#if defined ifx_OnPlayerUpdate
		return ifx_OnPlayerUpdate(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerUpdate
	#undef OnPlayerUpdate
#else
	#define _ALS_OnPlayerUpdate
#endif
#define OnPlayerUpdate ifx_OnPlayerUpdate
#if defined ifx_OnPlayerUpdate
	forward ifx_OnPlayerUpdate(playerid);
#endif