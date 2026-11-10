// CCreatureAction_BriarRoseConcentrateInto::~CCreatureAction_BriarRoseConcentrateInto() -- the compiler-generated destructor: ~CBaseIntelligentPointer on PEnemy (+0x115), then the out-of-line ~CCombatActionBase as the tail jump; no vtable store
// Leaf: ??1CCreatureAction_BriarRoseConcentrateInto@@UAE@XZ
// Retail: Fable.exe @ 0x008E8C20. No caller (an unreferenced COMDAT the linker kept).
// Identity: it follows the class's constructor 0x008E8BB0 and IsToFaceTargetAtStart 0x008E8C10 (vtable 0x01287904, CIntelligentPointer<CThingCreatureBase> vtable 0x0123E7D8 at +0x115); only this class destroys a pointer at +0x115;
//   that vtable's slot 0 (the scalar deleting destructor) expands the same sequence. Ego_r.pdb CCreatureAction_BriarRoseConcentrateInto : CCombatActionBase has
//   Imposter (bool), PEnemy (CIntelligentPointer<CThingCreatureBase>) right after the base (retail CCombatActionBase is 0x114 bytes);
//   FableWin.pdb ??1CCreatureAction_BriarRoseConcentrateInto@@UAE@XZ (compiler-generated: no vtable store).
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (Ego_r.pdb / FableWin.pdb ??1CBaseIntelligentPointer@@UAE@XZ),
//   CCombatActionBase::~CCombatActionBase 0x0062E900 (??1CCombatActionBase@@UAE@XZ, tail jump).
// Model: the shared engine/CIntelligentPointer.h and engine/CCreatureActionBase.h / engine/CCombatActionBase.h (retail branches).
//   engine/CCreatureAction_BriarRoseConcentrateInto.h is the generated flattened layout; its hdr overlay opt-in FABLE_RETAIL_CCREATUREACTION_BRIARROSECONCENTRATEINTO is the retail class.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "engine/CIntelligentPointer.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
class CThing;
class CThingCreatureBase;
#define FABLE_RETAIL_CCREATUREACTION_BRIARROSECONCENTRATEINTO
#include "engine/CCreatureAction_BriarRoseConcentrateInto.h"

// Emission harness (not a retail function): the class declares no destructor (retail's stores no vtable); a qualified call
// makes VC7.1 emit the implicit destructor's COMDAT.
void DestroyBriarRoseConcentrateInto(CCreatureAction_BriarRoseConcentrateInto* action)
{
    action->CCreatureAction_BriarRoseConcentrateInto::~CCreatureAction_BriarRoseConcentrateInto();
}
