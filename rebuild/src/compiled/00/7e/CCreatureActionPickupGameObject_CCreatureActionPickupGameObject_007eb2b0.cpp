// CCreatureAction_PickupGameObject::~CCreatureAction_PickupGameObject() -- the compiler-generated destructor: ~CBaseIntelligentPointer on PObject (+0xA8), then the out-of-line ~CCreatureActionBase as the tail jump; no vtable store
// Leaf: ??1CCreatureAction_PickupGameObject@@UAE@XZ
// Retail: Fable.exe @ 0x007EB2B0. No caller (an unreferenced COMDAT the linker kept).
// Identity: it follows the class's Clone 0x007EB230 and copy constructor 0x007EB260 (vtable 0x01270C3C, CIntelligentPointer<CThing> vtable 0x01238C6C at +0xA8);
//   that vtable's slot 0 (the scalar deleting destructor) expands the same sequence. Ego_r.pdb CCreatureAction_PickupGameObject : CCreatureActionBase has
//   PObject (CIntelligentPointer<CThing>) right after the base (retail CCreatureActionBase is 0xA8 bytes);
//   FableWin.pdb ??1CCreatureAction_PickupGameObject@@UAE@XZ (compiler-generated: no vtable store).
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (Ego_r.pdb / FableWin.pdb ??1CBaseIntelligentPointer@@UAE@XZ),
//   CCreatureActionBase::~CCreatureActionBase 0x00693EF0 (??1CCreatureActionBase@@UAE@XZ, tail jump).
// Model: the shared engine/CIntelligentPointer.h and engine/CCreatureActionBase.h (retail branches).
//   engine/CCreatureAction_PickupGameObject.h is the generated flattened layout; its hdr overlay opt-in FABLE_RETAIL_CCREATUREACTION_PICKUPGAMEOBJECT is the retail class.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "engine/CIntelligentPointer.h"
#include "engine/CCreatureActionBase.h"
class CThing;
class CThingCreatureBase;
#define FABLE_RETAIL_CCREATUREACTION_PICKUPGAMEOBJECT
#include "engine/CCreatureAction_PickupGameObject.h"

// Emission harness (not a retail function): the class declares no destructor (retail's stores no vtable); a qualified call
// makes VC7.1 emit the implicit destructor's COMDAT.
void DestroyPickupGameObject(CCreatureAction_PickupGameObject* action)
{
    action->CCreatureAction_PickupGameObject::~CCreatureAction_PickupGameObject();
}
