// CPlayerManager::IsPlayer(long player_number) -- true when one of the players has that number
// Leaf: ?IsPlayer@CPlayerManager@@QBE_NJ@Z
// Retail: Fable.exe @ 0x00449880, 61 bytes, `ret 4`; callers 0x0040758E, 0x00449C69, 0x00449CB9, 0x0044A1AC, 0x0044B9E0
// (jmp); Ego_r.pdb ?IsPlayer@CPlayerManager@@QBE_NJ@Z.
// Layout: retail CPlayerManager -- Players (12-byte STLport vector<CPlayer*>) +0x0c, PlayerNeutral +0x18 (Ego_r.pdb +0x1c with a
// 16-byte vector; engine/CPlayerManager.h carries the PDB offsets), modelled locally as at 0x0044A530. CPlayer from
// engine/CPlayer.h (Number +0x28).
// Shape: /O2 (16-aligned entry); size() read once into a local count, `sar` from the signed pointer difference, `long` index.
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
    bool IsPlayer(long player_number) const;
};
bool CPlayerManager::IsPlayer(long player_number) const
{
    const long count = Players.size();
    for (long i = 0; i < count; ++i)
        if (Players[i]->Number == player_number)
            return true;
    return false;
}