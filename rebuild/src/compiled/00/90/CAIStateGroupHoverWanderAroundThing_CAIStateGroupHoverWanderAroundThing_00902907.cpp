// CAIStateGroup_HoverWanderAroundThing::CAIStateGroup_HoverWanderAroundThing() -- base CAIStateGroup_HoverMovementBase(), own vtable, then the PThing member (+0x1c)
// Leaf: ??0CAIStateGroup_HoverWanderAroundThing@@QAE@XZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00902907, 36 bytes, thiscall, `ret`.
// Identity: stores own vtable 0x0128D1BC (RTTI .?AVCAIStateGroup_HoverWanderAroundThing@@, base CAIStateGroup_HoverMovementBase : CAIStateGroup_EnemyCreatureBase : CAIStateGroupBase : CBaseClass); member vtable 0x0125C29C = RTTI .?AV?$CIntelligentPointer@$$CBVCThingGameObject@@@@; Ego_r.pdb / FableWin.pdb
//   ??0CAIStateGroup_HoverWanderAroundThing@@QAE@XZ.
// Callees: CAIStateGroup_HoverMovementBase::CAIStateGroup_HoverMovementBase 0x008FF720 (??0CAIStateGroup_HoverMovementBase@@QAE@XZ), CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10 (??0CBaseIntelligentPointer@@QAE@XZ, Ego_r.pdb / FableWin.pdb).
// Layout: engine/CAIStateGroupBase.h's retail branch (CAIStateGroupBase : CBaseClass, 0x14); Ego_r.pdb CAIStateGroup_HoverWanderAroundThing::PThing +0x1c; the members before it
//   are left unmodelled. CBaseIntelligentPointer / CIntelligentPointer<T>: the chunk 703 model (0x00900B41).
// Shape: user default ctor, empty; CIntelligentPointer's ctor inlines to the base call + vtable store after this class's own vtable store.
// History: generated in a chunk batch for a repeated body shape over the CBase_008ff720 / CMemberBase_00a01b10 stand-ins; re-authored on the
//   PDB classes in naming wave 32 (family aisg).
#define FABLE_RETAIL_CAISTATEGROUPBASE
#include "engine/CAIStateGroupBase.h"                  // CAIStateGroupBase : CBaseClass (0x14), ctor 0x008FD255

class CBaseObjectPointer;
#pragma pack(push, 1)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#pragma pack(pop)
class CThingGameObject;

class CAIStateGroup_EnemyCreatureBase : public CAIStateGroupBase {                    // RTTI CAIStateGroup_EnemyCreatureBase : CAIStateGroupBase
public:
    CAIStateGroup_EnemyCreatureBase();                         // ??0CAIStateGroup_EnemyCreatureBase@@QAE@XZ (Ego_r.pdb / FableWin.pdb)
    unsigned char _unmodeled14[0x1c - 0x14];       // Ego_r.pdb: PTargetThing +0x14
};

class CAIStateGroup_HoverMovementBase : public CAIStateGroup_EnemyCreatureBase {                    // RTTI CAIStateGroup_HoverMovementBase : CAIStateGroup_EnemyCreatureBase
public:                                  // ctor FableWin.pdb only (??0CAIStateGroup_HoverMovementBase@@QAE@XZ): inlined into this row
};

class CAIStateGroup_HoverWanderAroundThing : public CAIStateGroup_HoverMovementBase {      // vtable 0x0128D1BC
public:
    CIntelligentPointer<const CThingGameObject> PThing;                     // +0x1c
    CAIStateGroup_HoverWanderAroundThing();
};

CAIStateGroup_HoverWanderAroundThing::CAIStateGroup_HoverWanderAroundThing()
{
}
