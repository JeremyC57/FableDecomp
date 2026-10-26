// CPlayerManager::GetPlayerNumberFromJoystickDeviceNumber(long device_number) -- number of the player on that joystick, else the neutral player
// Leaf: ?GetPlayerNumberFromJoystickDeviceNumber@CPlayerManager@@QBEJJ@Z
// Retail: Fable.exe @ 0x00449990, 63 bytes, `ret 4`; Ego_r.pdb ?GetPlayerNumberFromJoystickDeviceNumber@CPlayerManager@@QBEJJ@Z.
// Callee CPlayer::GetJoystickDeviceNumber 0x00487510 (out of line).
// Layout: retail CPlayerManager -- Players (12-byte STLport vector<CPlayer*>) +0x0c, PlayerNeutral +0x18 (Ego_r.pdb +0x1c with a
// 16-byte vector; engine/CPlayerManager.h carries the PDB offsets), modelled locally as at 0x0044A530. CPlayer from
// engine/CPlayer.h (Number +0x28).
// Shape: /O2 (16-aligned entry); size() read once into a local count, the player held in a local, `sar` from the signed pointer difference, `long` index.
#define CPlayer CPlayerLayout
#include "engine/CPlayer.h"
#undef CPlayer

class CPlayer : public CPlayerLayout
{
public:
    long GetJoystickDeviceNumber() const;   // 0x00487510
};
struct CPlayer_Methods : CPlayer {
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