// CAIStateGroup_MinionReactToCrime::CAIStateGroup_MinionReactToCrime() -- construct the state group: base CAIStateGroupBase, own vtable, the ForceOpenBuilding tracked pointer
// Leaf: ??0CAIStateGroup_MinionReactToCrime@@QAE@XZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00900B41, 36 bytes, thiscall, `ret`; a size-optimised TU packed without padding (the
//   previous function, the MinionReactToNoise ??_G, ends with `ret 4` at 0x00900B3E), so the unaligned start is a real one.
// Identity (proven): stores the CAIStateGroup_MinionReactToCrime vtable 0x0128C558 (RTTI .?AVCAIStateGroup_MinionReactToCrime@@;
//   slot 0 is its ??_G 0x00900B65 right after this body); the member vtable 0x01244344 is .?AV?$CIntelligentPointer@VCThingBuilding@@@@,
//   at +0x1B = Ego_r.pdb CAIStateGroup_MinionReactToCrime::ForceOpenBuilding (offset 27, CIntelligentPointer<CThingBuilding>);
//   Ego_r.pdb name set ??0CAIStateGroup_MinionReactToCrime@@QAE@XZ. Callers: the derived ctor
//   CAIStateGroup_MinionReactToCrimeFromSleep 0x00900B81 and the two Alloc-style factories 0x009028DB / 0x009028F4.
//   The catalog label "CAIStateGroupBehaviourBase::CAIStateGroupBehaviourBase" is wrong (NAME_FIXES).
// Callees: CAIStateGroupBase::CAIStateGroupBase 0x008FD255, CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10.
// Layout: Ego_r.pdb CAIStateGroupBase (0x14) + long OpinionDeedHandle +0x14, bool GuardTold +0x18, AllowDuplicateReactToCrime
//   +0x19, DeedMaskApplied +0x1A, CIntelligentPointer<CThingBuilding> ForceOpenBuilding +0x1B (0x24 bytes in all): the tracked
//   pointer type is 1-aligned (`#pragma pack(push, 1)` on CBaseIntelligentPointer, as the landed CThingSoundEmitter 0x008367B0
//   and the GOTCHAS CTCHero::LastWeaponEquipped +0x1D line), while the class keeps 4-byte alignment (PDB size 36, not 35).
// Shape: user default ctor, empty; the long / bools stay uninitialised; CIntelligentPointer's ctor inlines to the base call +
//   vtable store after this class's own vtable store.
#include "engine/CBaseClass.h"                          // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

class CAIBrain;
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
};
enum EScriptAIPriority { SCRIPT_PRIORITY_DEFAULT };

class CAIStateGroupBase : public CBaseClass {
public:
    CAIBrain*         PBrain;                           // +0x04
    EScriptAIPriority ScriptAIPriority;                 // +0x08
    CCharString       Name;                             // +0x0C
    bool              Terminated;                       // +0x10
    unsigned char     Priority;                         // +0x11
    bool              Enabled;                          // +0x12
    CAIStateGroupBase();                                // 0x008FD255
};

class CBaseObjectPointer;
class CThingBuilding;
#pragma pack(push, 1)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#pragma pack(pop)

class CAIStateGroup_MinionReactToCrime : public CAIStateGroupBase {   // vtable 0x0128C558
public:
    long                                OpinionDeedHandle;              // +0x14
    bool                                GuardTold;                      // +0x18
    bool                                AllowDuplicateReactToCrime;     // +0x19
    bool                                DeedMaskApplied;                // +0x1A
    CIntelligentPointer<CThingBuilding> ForceOpenBuilding;              // +0x1B
    CAIStateGroup_MinionReactToCrime();
};

CAIStateGroup_MinionReactToCrime::CAIStateGroup_MinionReactToCrime()
{
}