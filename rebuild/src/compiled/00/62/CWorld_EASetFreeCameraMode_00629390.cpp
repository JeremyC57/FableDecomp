// CWorld::EASetFreeCameraMode(const CGameEvent*) -- game-event action: turn the event player's free-camera mode (player mode 19) on or off
// Grade: DIFFER(141v141) -- retail reads the flag byte into the dead event argument slot ([esp+3Ch]) with a 0x30 frame; ours gives the byte its own frame slot (0x34 frame) and swaps esi/edi
// Leaf: EASetFreeCameraMode
// Flags: /GS
// Retail: Fable.exe @ 0x00629390, 139 bytes (/GS cookie 0x0139C8A8). PDB ?EASetFreeCameraMode@CWorld@@QAEXPBVCGameEvent@@@Z
// Callees: CGameEvent::GetData 0x009F17A0, CMemoryBuffer::Read 0x009F18F0, CWorld::GetPlayer(long) 0x0049D850
// (forwards to PlayerManager +0xC; manifest label "_Cons_val<...>" is wrong), CPlayer::AddMode(EPlayerMode, bool)
// 0x006345C0, CPlayer::RemoveMode(EPlayerMode) 0x00634190, __security_check_cookie 0x00BFE9F9.
// Shape: the flag byte is read into the dead `event` argument slot and compared with true (`cmp byte ptr [arg],1`).
// CGameEvent (40 bytes, Ego_r.pdb = retail): +0x00 Type, +0x04 signed char Player, +0x05 Data[32], +0x25 EndPos,
// +0x26 Valid, +0x27 Replacement. Its payload is read through a 44-byte CMemoryBuffer local: Data[0x20], then the
// length +0x20 and two cursor words +0x24/+0x28 that the inline constructor zeroes; CGameEvent::GetData(CMemoryBuffer&)
// 0x009F17A0 (label "CEventPacket::CopyToBuffer") fills it and CMemoryBuffer::Read(unsigned long, void*) 0x009F18F0
// (label "CircularBuffer_Read") consumes it.
// NF95 2026-11-19 (q11320): the unpacking buffer is Ego_r.pdb CGameEventDataPackage (engine/CGameEventDataPackage.h layout, 0x2C bytes)
//   read through ?GetData@CGameEventDataPackage@@QAEXJPAX@Z 0x009F18F0 (Ego_r.pdb 0x00B454F0 / FableWin.pdb 0x030A2200) and filled by
//   ?GetDataPackage@CGameEvent@@QBEXPAVCGameEventDataPackage@@@Z 0x009F17A0 (Ego_r.pdb 0x00B453F0); were the stand-ins
//   CMemoryBuffer::Read(unsigned long, void*) and CGameEvent::GetData(CMemoryBuffer&).
// NF97 2026-11-19 (port): the read buffer is the shared engine/CGameEventDataPackage.h class (FABLE_RETAIL_CGAMEEVENTDATAPACKAGE:
//   inline ctor zeroing Length / StartPos / EndPos, GetData 0x009F18F0), no longer a local model.
//   The local made-up reader helper ReadBool is replaced by a direct GetData read into the typed local (no new member names;
//   FableWin.pdb names typed readers GetUBYTE / GetUWORD / GetFloat / GetSLONG, out of line only in FableWin).
#define FABLE_RETAIL_CGAMEEVENTDATAPACKAGE
#include "fable_compiler.h"
#include "engine/CGameEventDataPackage.h"
class CGameEvent { public:
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    void GetDataPackage(CGameEventDataPackage* package) const;     // 0x009F17A0
};

enum EPlayerMode { PLAYER_MODE_FREE_CAMERA = 0x13 };
class CPlayer { public:
    void AddMode(EPlayerMode mode, bool b);        // 0x006345C0
    void RemoveMode(EPlayerMode mode);             // 0x00634190
};

class CWorld { public:
protected:
    CPlayer* GetPlayer(unsigned long number);               // 0x0049D850 ?GetPlayer@CWorld@@IAEPAVCPlayer@@K@Z (Ego_r/FableWin, protected)
public:
    void EASetFreeCameraMode(const CGameEvent* event);
};

void CWorld::EASetFreeCameraMode(const CGameEvent* event)
{
    CGameEventDataPackage buffer;
    event->GetDataPackage(&buffer);
    bool on;
    buffer.GetData(1, &on);
    CPlayer* player = GetPlayer(event->Player);
    if (on == true)
        player->AddMode(PLAYER_MODE_FREE_CAMERA, false);
    else
        player->RemoveMode(PLAYER_MODE_FREE_CAMERA);
}