// CCreatureAction_BriarRoseInvulnerableTeleport::~CCreatureAction_BriarRoseInvulnerableTeleport() -- the compiler-generated destructor: ~CBaseIntelligentPointer on PEnemy (+0x114), then the out-of-line ~CCombatActionBase as the tail jump; no vtable store
// Leaf: ??1CCreatureAction_BriarRoseInvulnerableTeleport@@UAE@XZ
// Retail: Fable.exe @ 0x008E8CB0. No caller (an unreferenced COMDAT the linker kept).
// Identity: it follows the class's constructor 0x008E8C50 (vtable 0x01287A4C, CIntelligentPointer<CThingCreatureBase> vtable 0x0123E7D8 at +0x114);
//   that vtable's slot 0 (the scalar deleting destructor) expands the same sequence. Ego_r.pdb CCreatureAction_BriarRoseInvulnerableTeleport : CCombatActionBase has
//   PEnemy (CIntelligentPointer<CThingCreatureBase>) right after the base (retail CCombatActionBase is 0x114 bytes);
//   FableWin.pdb ??1CCreatureAction_BriarRoseInvulnerableTeleport@@UAE@XZ (compiler-generated: no vtable store).
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (Ego_r.pdb / FableWin.pdb ??1CBaseIntelligentPointer@@UAE@XZ),
//   CCombatActionBase::~CCombatActionBase 0x0062E900 (??1CCombatActionBase@@UAE@XZ, tail jump).
// Model: the shared engine/CIntelligentPointer.h and engine/CCreatureActionBase.h / engine/CCombatActionBase.h (retail branches).
//   No engine header models CCreatureAction_BriarRoseInvulnerableTeleport, so it is declared here.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "engine/CIntelligentPointer.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
class CThing;
class CThingCreatureBase;

class CCreatureAction_BriarRoseInvulnerableTeleport : public CCombatActionBase {                      // vtable 0x01287A4C
public:
    CCreatureAction_BriarRoseInvulnerableTeleport(CThingCreatureBase& creature, CThingCreatureBase& enemy);   // 0x008E8C50 (Ego_r.pdb / FableWin.pdb ??0CCreatureAction_BriarRoseInvulnerableTeleport@@QAE@...)
    CIntelligentPointer<CThingCreatureBase> PEnemy;             // +0x114
};

// Emission harness (not a retail function): the class declares no destructor (retail's stores no vtable); a qualified call
// makes VC7.1 emit the implicit destructor's COMDAT.
void DestroyBriarRoseInvulnerableTeleport(CCreatureAction_BriarRoseInvulnerableTeleport* action)
{
    action->CCreatureAction_BriarRoseInvulnerableTeleport::~CCreatureAction_BriarRoseInvulnerableTeleport();
}
