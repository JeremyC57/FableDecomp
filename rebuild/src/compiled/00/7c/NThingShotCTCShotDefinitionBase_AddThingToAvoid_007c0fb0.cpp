// NThingShot::CTCShotDefinitionBase::AddThingToAvoid(const CThing&) -- appends the thing to the shot's hit-things list (HitThingsList), so the shot no longer collides with it
// Leaf: ?AddThingToAvoid@CTCShotDefinitionBase@NThingShot@@QAEXABVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007C0FB0, 66 bytes (+ int3 padding), thiscall, `ret 4`.
// Name (NF78, q10947): Ego_r.pdb 0x007BFAF0 and FableWin.pdb 0x026FCCD0 ?AddThingToAvoid@CTCShotDefinitionBase@NThingShot@@QAEXABVCThing@@@Z;
//   Ego_d.pdb gives it one parameter `thing` and one local `ip` (an intelligent pointer); the only vector<CIntelligentPointer<const CThing> >
//   member of the class is HitThingsList (+0x14); the landed caller CCreatureAction_DragonRearUpFull::PerformAction 0x008F3630 binds
//   this spelling. Was the descriptive CTCShotDefinitionBase::AddHitThing(const CThing*) on a local class model.
// Port (NF78, round 7): the class from engine/NThingShot_CTCShotDefinitionBase.h (FABLE_RETAIL_CTCSHOTDEFINITIONBASE_ADDTHINGTOAVOID, NF74);
//   the local CThing / CTCBase / std::vector / CTCShotDefinitionBase models are gone.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10, CBaseIntelligentPointer::SetPItem(CBaseObject*) 0x00A01B90,
//   std::vector<CIntelligentPointer<const CThing> >::push_back 0x007398B0 (out of line in this TU), CBaseIntelligentPointer::
//   ~CBaseIntelligentPointer() 0x00A01C10. Vtable: CIntelligentPointer<const CThing> 0x01237F1C.
// Shape: ip's inline (T*) constructor (base ctor, own vtable, SetPItem) and its implicit destructor (a direct call of the base
//   destructor, no vtable reset); no EH frame (the module is built without /GX).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_ADDTHINGTOAVOID
#include "fable_compiler.h"
#include "engine/CThing.h"
#include "engine/NThingShot_CTCShotDefinitionBase.h"

void NThingShot::CTCShotDefinitionBase::AddThingToAvoid(const CThing& thing)
{
    CIntelligentPointer<const CThing> ip(&thing);
    HitThingsList.push_back(ip);
}
