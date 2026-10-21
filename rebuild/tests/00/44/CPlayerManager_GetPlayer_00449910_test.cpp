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
    T& operator[](unsigned int n) { return _M_start[n]; }
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
    CPlayer& GetPlayer(long number);
    const CPlayer& GetPlayer(long number) const;
};
const CPlayer& CPlayerManager::GetPlayer(long number) const
{
    const long count = Players.size();
    for (long i = 0; i < count; ++i)
    {
        if (Players[i]->Number == number)
            return *Players[i];
    }
    return *Players[PlayerNeutral];
}


#include <cstring>
long CPlayer_Methods::GetJoystickDeviceNumber() const { return 0; }
int main()
{
    static unsigned char pb[3][sizeof(CPlayer_Methods)];
    CPlayer_Methods* p[3];
    for (int k = 0; k < 3; ++k) { std::memset(pb[k], 0, sizeof pb[k]); p[k] = (CPlayer_Methods*)pb[k]; p[k]->Number = 10 + k; }
    static CPlayerManager m;
    m.Players._M_start = p; m.Players._M_finish = p + 3; m.PlayerNeutral = 2;
    const CPlayerManager& r = m;
    bool ok = &r.GetPlayer(11) == p[1] && &r.GetPlayer(10) == p[0] && &r.GetPlayer(99) == p[2];
    m.Players._M_finish = p; m.PlayerNeutral = 0; ok = ok && &r.GetPlayer(10) == p[0];
    if (ok) std::printf("00449910_TEST PASS\n");
    return 0;
}