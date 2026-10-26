// CPlayerManager::GetPlayer(long number) const -- the player whose Number matches, else the neutral player
// Leaf: ?GetPlayer@CPlayerManager@@QBEABVCPlayer@@J@Z
// Retail: Fable.exe @ 0x00449910, 73 bytes, `ret 4`; Ego_r.pdb ?GetPlayer@CPlayerManager@@QBEABVCPlayer@@J@Z. 0x004498C0 and 0x00449910
// are byte-identical (the const and non-const overloads); which address is which overload is not recoverable from the bytes.
// Layout: retail CPlayerManager -- Players (12-byte STLport vector<CPlayer*>) +0x0c, PlayerNeutral +0x18 (Ego_r.pdb +0x1c with a
// 16-byte vector; engine/CPlayerManager.h carries the PDB offsets), modelled locally as at 0x00449990. CPlayer from
// engine/CPlayer.h (Number +0x28).
// Shape: /O2 (16-aligned entry, `lea ecx,[ecx]` loop pad); size() read once into a local count, `long` index.
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