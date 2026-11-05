// CCreatureAction_ShowItemToThing::~CCreatureAction_ShowItemToThing() -- compiler-generated destructor: PItemBeingShown, then PReceivingThing (both ~CBaseIntelligentPointer), then ~CCreatureActionBase by tail jump
// Leaf: ??1CCreatureAction_ShowItemToThing@@UAE@XZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) @ 0x007ED750, 33 bytes: `push esi ; mov esi,ecx ; lea ecx,[esi+0B0h] ; call 0x00A01C10 ;
//   lea ecx,[esi+0A8h] ; call 0x00A01C10 ; mov ecx,esi ; pop esi ; jmp 0x00693EF0`. No caller (every use is inlined; the
//   linker keeps unreferenced COMDATs).
// Identity (proven): it sits in CCreatureAction_ShowItemToThing's TU, right before that class's constructor 0x007ED780
//   (landed, stores vtable 0x012714C4) and ahead of its scalar deleting destructor 0x007ED830 (vtable slot 0), and the
//   only +0xA8/+0xB0 two-pointer action in that band (GiveMoneyToThing 0x007ED4E0 and PickUpJug 0x00968040 destroy one
//   pointer at +0xA8). CTCCarriedActionUseShow's use slot 0x007ED870 builds the action (call 0x007ED780) and inlines this
//   exact sequence (+0xB0, +0xA8, 0x00693EF0 at 0x007ED914..0x007ED929). FableWin.pdb names
//   ??1CCreatureAction_ShowItemToThing@@UAE@XZ (compiler-generated: no vtable store). The catalog label
//   `_Dest_val<...CSymbolMap::CSymbolInfo...>` is propagated across five byte-identical action destructors.
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (the members' implicit ~CIntelligentPointer<CThing>
//   inlines to it), CCreatureActionBase::~CCreatureActionBase 0x00693EF0.
// Layout: Ego_r.pdb CCreatureAction_ShowItemToThing : CCreatureActionBase has PReceivingThing / PItemBeingShown
//   (CIntelligentPointer<CThing>) at +0xB4 / +0xBC; retail CCreatureActionBase is 0xA8 (12-byte STLport vectors, map 0xC),
//   so they sit at +0xA8 / +0xB0 (the landed constructor 0x007ED780). engine/CCreatureActionBase.h carries the PDB size,
//   so the base is modelled locally at its retail size.
// Emission: an implicit destructor is emitted where a translation unit destroys the object by name; the TU harness below
//   destroys a local action by name (retail's user 0x007ED870 builds one, hands it to SetCurrentAction and destroys it). It is not a retail function.
class CBaseObject;
class CBaseObjectPointer;
class CThing;
class CThingCreatureBase;

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CCreatureActionBase {                                 // retail sizeof 0xA8
public:
    virtual ~CCreatureActionBase();                         // 0x00693EF0
private:
    unsigned char _body[0xa8 - 4];
};

class CCreatureAction_ShowItemToThing : public CCreatureActionBase {   // vtable 0x012714C4
public:
    CCreatureAction_ShowItemToThing(CThingCreatureBase& creature, CThing& receivingThing, CThing& itemBeingShown);   // 0x007ED780
    CIntelligentPointer<CThing> PReceivingThing;            // +0xA8
    CIntelligentPointer<CThing> PItemBeingShown;            // +0xB0
};

// TU harness (not a retail function): a local action destroyed by name.
void TUHarness_ShowItemToThing(CThingCreatureBase& creature, CThing& receivingThing, CThing& itemBeingShown)
{
    CCreatureAction_ShowItemToThing action(creature, receivingThing, itemBeingShown);
}