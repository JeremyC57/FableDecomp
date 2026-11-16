// CTCAtmosPlayer::CTCAtmosPlayer(CThing* thing) -- CTCBase(thing), PSoundEmitter and AtmosDefIndex cleared, own vtable
// Leaf: ??0CTCAtmosPlayer@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase_Calls (which bound ??0CTCBase_Calls@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as Ego_r.pdb and FableWin.pdb spell it (??0CTCAtmosPlayer@@QAE@AAVCThing@@@Z).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E35A0, length 33 bytes (+ int3 padding).
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800 (manifest "CEngineSubPrimitive2DClockSprite" is propagated).
// Vtable 0x01270484 = CTCAtmosPlayer (rtti-vtables.tsv).
// Layout = Ego_r.pdb CTCAtmosPlayer (20 bytes): CTCBase 0xc, PSoundEmitter +0xc, AtmosDefIndex +0x10.
// engine/CTCBase.h holds the layout only, so the base is modelled as `CTCBase_Calls` (as the landed
// 0x007AEE10). Shape: both zeroes share one `xor eax,eax` register and are stored before the vtable.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;
struct CASoundEmitter;

struct CTCAtmosPlayer : CTCBase {                     // vtable 0x01270484
    CASoundEmitter* PSoundEmitter;                          // +0x0c
    long            AtmosDefIndex;                          // +0x10
    CTCAtmosPlayer(CThing& thing);
};

CTCAtmosPlayer::CTCAtmosPlayer(CThing& thing)
    : CTCBase(thing)
    , PSoundEmitter(0)
    , AtmosDefIndex(0)
{
}