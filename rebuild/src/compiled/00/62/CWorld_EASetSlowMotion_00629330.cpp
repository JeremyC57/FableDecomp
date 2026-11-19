// CWorld::EASetSlowMotion(const CGameEvent*) -- game-event action: switch the world's slow-motion flag from the event
// Leaf: ?EASetSlowMotion@CWorld@@QAEXPBVCGameEvent@@@Z
// Flags: /GS
// Retail: Fable.exe @ 0x00629330, 90 bytes (/GS cookie 0x0139C8A8). PDB ?EASetSlowMotion@CWorld@@QAEXPBVCGameEvent@@@Z
// Callees: CGameEvent::GetData(CMemoryBuffer&) 0x009F17A0, CMemoryBuffer::Read(unsigned long, void*) 0x009F18F0,
// __security_check_cookie 0x00BFE9F9. Retail CWorld SlowMotion +0xA5 (PDB = retail, ahead of the containers).
// Model of CGameEvent / CMemoryBuffer as in the landed sibling CWorld::EASetFreeCameraMode 0x00629390: a 44-byte
// buffer (Data[0x20] + three zeroed words) is filled from the event and one bool read from it.
// Shape: the event pointer is dead after GetData, so VC7.1 homes the read flag in the event's argument slot
// (`lea ecx,[esp+38h] ; push ecx`) and compares it with true (`mov al,[esp+38h] ; cmp al,1 ; sete dl`) before the byte store.
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
class CGameEvent {                                 // PDB class (mangled V)
public:
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    void GetDataPackage(CGameEventDataPackage* package) const;     // 0x009F17A0
};

#define CWorld DonorCWorld
#include "engine/CWorld.h"
#undef CWorld
class CWorld : public DonorCWorld { public:                      // engine/CWorld.h: SlowMotion +0xA5 (PDB = retail here)
    void EASetSlowMotion(const CGameEvent* event);
};

void CWorld::EASetSlowMotion(const CGameEvent* event)
{
    CGameEventDataPackage buffer;
    event->GetDataPackage(&buffer);
    bool slowMotion;
    buffer.GetData(1, &slowMotion);
    SlowMotion = (slowMotion == true);
}