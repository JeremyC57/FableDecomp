// CAIStateGroup_MinionFollowThing::CAIStateGroup_MinionFollowThing() -- base CAIStateGroupBase(), own vtable, then the PFollowee member (+0x14)
// Leaf: ??0CAIStateGroup_MinionFollowThing@@QAE@XZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0090148C, 36 bytes, thiscall, `ret`.
// Identity: stores own vtable 0x0128CC24 (RTTI .?AVCAIStateGroup_MinionFollowThing@@, base CAIStateGroupBase : CBaseClass); member vtable 0x0123E7D8 = RTTI .?AV?$CIntelligentPointer@VCThingCreatureBase@@@@; Ego_r.pdb / FableWin.pdb
//   ??0CAIStateGroup_MinionFollowThing@@QAE@XZ.
// Callees: CAIStateGroupBase::CAIStateGroupBase 0x008FD255, CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10 (??0CBaseIntelligentPointer@@QAE@XZ, Ego_r.pdb / FableWin.pdb).
// Layout: engine/CAIStateGroupBase.h's retail branch (CAIStateGroupBase : CBaseClass, 0x14); Ego_r.pdb CAIStateGroup_MinionFollowThing::PFollowee +0x14; the members before it
//   are left unmodelled. CBaseIntelligentPointer / CIntelligentPointer<T>: the chunk 703 model (0x00900B41).
// Shape: user default ctor, empty; CIntelligentPointer's ctor inlines to the base call + vtable store after this class's own vtable store.
// History: generated in a chunk batch for a repeated body shape over the CBase_008fd255 / CMemberBase_00a01b10 stand-ins; re-authored on the
//   PDB classes in naming wave 32 (family aisg).
#define FABLE_RETAIL_CAISTATEGROUPBASE
#include "engine/CAIStateGroupBase.h"                  // CAIStateGroupBase : CBaseClass (0x14), ctor 0x008FD255

class CBaseObjectPointer;
#pragma pack(push, 1)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#pragma pack(pop)
class CThingCreatureBase;

class CAIStateGroup_MinionFollowThing : public CAIStateGroupBase {      // vtable 0x0128CC24
public:
    CIntelligentPointer<CThingCreatureBase> PFollowee;                     // +0x14
    CAIStateGroup_MinionFollowThing();
};

CAIStateGroup_MinionFollowThing::CAIStateGroup_MinionFollowThing()
{
}
