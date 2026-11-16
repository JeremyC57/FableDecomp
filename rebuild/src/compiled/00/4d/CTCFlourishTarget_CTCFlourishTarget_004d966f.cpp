// CTCFlourishTarget::CTCFlourishTarget(CThing* owner) -- thing-component base, the attacker intelligent pointer built, both movement-cull flags set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D966F, length 50 bytes. Callees: CTCBase::CTCBase(CThing&) @ 0x00686800
// (manifest label "CEngineSubPrimitive2DClockSprite", propagated), CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10.
// Label correction: the manifest calls this a CTCGossip constructor; the vtable it stores, 0x0123EF34, is
// CTCFlourishTarget's (rtti-vtables.tsv), and its only caller is the factory @ 0x004DB49D (operator new(0x18)).
// Member vtable: CIntelligentPointer<CThing const> 0x01237F1C.
// Layout from Ego_r.pdb (CTCFlourishTarget, 24 bytes): the CTCBase part +0x00..+0x0b, PAttacker
// (CIntelligentPointer<CThing const>) +0x0c, CullNegativeXAxisMovement +0x14, CullAllYAxisMovement +0x15 (bools).
// Shape: size-optimised module; vtable store, member pointer constructor, then the two bool initialisers (= true)
// in declaration order.
// The engine header CTCBase.h is a flattened POD (`void* __vftable`), unusable as a polymorphic base: local model. [superseded 2026-11-16: the header's FABLE_RETAIL_CTCBASE branch is polymorphic; see the Ported note]
// Leaf: ??0CTCFlourishTarget@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase (which bound ??0CTCBase@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as the PDBs spell it (??0CTCFlourishTarget@@QAE@AAVCThing@@@Z).
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


class CTCFlourishTarget : public CTCBase { public:
    CIntelligentPointer<const CThing> PAttacker;          // +0x0c
    bool CullNegativeXAxisMovement;                     // +0x14
    bool CullAllYAxisMovement;                          // +0x15
    CTCFlourishTarget(CThing& owner);
};

CTCFlourishTarget::CTCFlourishTarget(CThing& owner)
    : CTCBase(owner), PAttacker(), CullNegativeXAxisMovement(true), CullAllYAxisMovement(true)
{
}