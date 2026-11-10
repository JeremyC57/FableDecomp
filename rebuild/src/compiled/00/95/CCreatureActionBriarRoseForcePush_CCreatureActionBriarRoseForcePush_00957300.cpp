// CCreatureAction_BriarRoseForcePush::~CCreatureAction_BriarRoseForcePush() -- the compiler-generated destructor: ~CBaseIntelligentPointer on PEnemy (+0x114), then the out-of-line ~CCombatActionBase as the tail jump; no vtable store
// Leaf: ??1CCreatureAction_BriarRoseForcePush@@UAE@XZ
// Retail: Fable.exe @ 0x00957300. No caller (an unreferenced COMDAT the linker kept).
// Identity: it follows the class's constructor 0x009572A0 (vtable 0x01294CD4, CIntelligentPointer<CThingCreatureBase> vtable 0x0123E7D8 at +0x114);
//   that vtable's slot 0 (the scalar deleting destructor) expands the same sequence. Ego_r.pdb CCreatureAction_BriarRoseForcePush : CCombatActionBase has
//   PEnemy (CIntelligentPointer<CThingCreatureBase>) right after the base (retail CCombatActionBase is 0x114 bytes);
//   FableWin.pdb ??1CCreatureAction_BriarRoseForcePush@@UAE@XZ (compiler-generated: no vtable store).
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (Ego_r.pdb / FableWin.pdb ??1CBaseIntelligentPointer@@UAE@XZ),
//   CCombatActionBase::~CCombatActionBase 0x0062E900 (??1CCombatActionBase@@UAE@XZ, tail jump).
// Model: the shared engine/CIntelligentPointer.h and engine/CCreatureActionBase.h / engine/CCombatActionBase.h (retail branches).
//   No engine header models CCreatureAction_BriarRoseForcePush, so it is declared here.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "engine/CIntelligentPointer.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
class CThing;
class CThingCreatureBase;

class CCreatureAction_BriarRoseForcePush : public CCombatActionBase {                      // vtable 0x01294CD4
public:
    CCreatureAction_BriarRoseForcePush(CThingCreatureBase& creature, CThingCreatureBase& enemy);   // 0x009572A0 (Ego_r.pdb / FableWin.pdb ??0CCreatureAction_BriarRoseForcePush@@QAE@...)
    CIntelligentPointer<CThingCreatureBase> PEnemy;             // +0x114
};

// Emission harness (not a retail function): the class declares no destructor (retail's stores no vtable); a qualified call
// makes VC7.1 emit the implicit destructor's COMDAT.
void DestroyBriarRoseForcePush(CCreatureAction_BriarRoseForcePush* action)
{
    action->CCreatureAction_BriarRoseForcePush::~CCreatureAction_BriarRoseForcePush();
}
