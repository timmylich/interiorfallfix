/*
- ����������� �������� � ��������� ���� ������������ ������ ������� �� ����� ������� ��������� ������������.
- �����: timmylich.
- ��������� - @timmylich, TG - @timmylich.
- ��� ������� ���� - ��� ����� � �� ��� ��� �������.

- ������ ����������� �������� �� �������� "��������� � �����".
- �� ���� ��� ��� ������ ���������� ����� ������ ���������� ��� ��� include ����� ����� �������� ���������(a_samp ��� open.mp).
- �� ������ �������� ��������� ������� � ������� ���� define'��.
- ��� ������� ����������� ������������ ������� ��� ��� ������������� ������� ���� � ���������� �����������(CreateObject), � ����� ������ ������� ������� ��������� ������ ��� ����� � �������� �� ������ ��� �����.

- ������: 1.1.
- ���� ����������: 11.04.2024.
- https://github.com/timmylich/interiorfallfix
*/


#if !defined ifx_FixTime // �����, ������� ����� ������������ ����� ����������� ������ �� ����� � �������������
    #define ifx_FixTime 7000
#endif

#if !defined ifx_GetPlayerInterior // ������ ����� ���������� � ���, � ����� �������� ��������� �����, ���������� �������� �� ���� ���������� ��� ��������� ������ � ���������
    #define ifx_GetPlayerInterior(%0) GetPlayerInterior(%0)
#endif

#if !defined ifx_PlayerUpdateCD // �������� ����� ���������� OnPlayerUpdate � �������������
    #define ifx_PlayerUpdateCD 250
#endif

#if !defined ifx_FallVelocity // �������� �������, ����������� ��� ������������
    #define ifx_FallVelocity 0.3
#endif

static Float:LastTP[MAX_PLAYERS][3]; // ���������� ��������� ������������
static LastTPTime[MAX_PLAYERS]; // ����� ��������� ������������ ������


// �������� SetPlayerPos
stock ifx_SetPlayerPos(playerid, Float:x, Float:y, Float:z)
{
    LastTPTime[playerid] = GetTickCount(); // ��������� ����� ���������� ���������
    // ��������� ������� ���������� ���������
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


static LastPlayerUpdate[MAX_PLAYERS]; // ��� ���������� ������������ �������� �� �������
// �������� �������� ���������� ������
public OnPlayerUpdate(playerid)
{
    if(GetTickCount()-ifx_PlayerUpdateCD > LastPlayerUpdate[playerid]) // �������� �� �������� ����� ��������������
    {
        LastPlayerUpdate[playerid] = GetTickCount();
        if(LastTPTime[playerid]+ifx_FixTime > GetTickCount() && ifx_GetPlayerInterior(playerid)) // �������� �� ��������� ����� � ��������� �� ����� � ���������
        {
            static Float:FallVelocity;
            GetPlayerVelocity(playerid, FallVelocity, FallVelocity, FallVelocity); // ��������� �������� ������� �� ��� Z

            if(FallVelocity < -ifx_FallVelocity) // ��������� �������� �������
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