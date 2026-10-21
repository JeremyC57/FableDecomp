// CGameEventPackage::CGameEventPackage() -- empty package: no events, every event slot marked unowned and invalid
// Retail: Fable.exe @ 0x00419220, 35 bytes; Ego_r.pdb ??0CGameEventPackage@@QAE@XZ. Stores NoEvents (+0x04) = 0, then
// for each of the 40 CGameEvent slots (+0x08, stride 0x28) Player (+0x04) = -1 and Valid (+0x26) = false.
// Layout: engine/CGameEvent.h (as Ego_r.pdb). The package (TimeStamp, NoEvents, Events[40]) is local because the
// ctor cannot be added through a derived struct; offsets as Ego_r.pdb (0x648 bytes).
#include "engine/CGameEvent.h"
struct CGameEvent_Init : CGameEvent {
    CGameEvent_Init() { Player = -1; Valid = false; }
};
class CGameEventPackage {
public:
    CGameEventPackage();
    long            TimeStamp;                           // +0x00
    unsigned long   NoEvents;                            // +0x04
    CGameEvent_Init Events[40];                          // +0x08
};
CGameEventPackage::CGameEventPackage()
    : NoEvents(0)
{
}