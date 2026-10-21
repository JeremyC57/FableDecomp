#include <cstdio>
#include "engine/CPlayer.h"
struct CPlayer_Methods : CPlayer {
    long GetJoystickDeviceNumber() const;                // 0x00487510
};
class CMainGameComponent;
class CGameDefinitionManager;
template <class T> class CVector {                       // STLport vector, 12 bytes
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    const T& operator[](unsigned int n) const { return _M_start[n]; }
};
class CPlayerManager {
public:
    void*                     _vfptr_or_pad;             // +0x00
    CMainGameComponent*       Component;                 // +0x04
    CGameDefinitionManager*   DefinitionManager;         // +0x08
    CVector<CPlayer_Methods*> Players;                   // +0x0c
    long                      PlayerNeutral;             // +0x18
    long                      MainPlayer;                // +0x1c
    long GetPlayerNumberFromJoystickDeviceNumber(long device_number) const;
};
long CPlayerManager::GetPlayerNumberFromJoystickDeviceNumber(long device_number) const
{
    const long count = Players.size();
    for (long i = 0; i < count; ++i)
    {
        const CPlayer_Methods* player = Players[i];
        if (player->GetJoystickDeviceNumber() == device_number)
            return player->Number;
    }
    return PlayerNeutral;
}

#include <cstring>
long CPlayer_Methods::GetJoystickDeviceNumber() const { return Number * 2; }
int main()
{
    static unsigned char pb[3][sizeof(CPlayer_Methods)];
    CPlayer_Methods* p[3];
    for (int k = 0; k < 3; ++k) { std::memset(pb[k], 0, sizeof pb[k]); p[k] = (CPlayer_Methods*)pb[k]; p[k]->Number = 10 + k; }
    static CPlayerManager m;
    m.Players._M_start = p; m.Players._M_finish = p + 3; m.PlayerNeutral = 4;
    bool ok = m.GetPlayerNumberFromJoystickDeviceNumber(22) == 11 && m.GetPlayerNumberFromJoystickDeviceNumber(20) == 10
        && m.GetPlayerNumberFromJoystickDeviceNumber(7) == 4;
    m.Players._M_finish = p; ok = ok && m.GetPlayerNumberFromJoystickDeviceNumber(20) == 4;
    if (ok) std::printf("00449990_TEST PASS\n");
    return 0;
}