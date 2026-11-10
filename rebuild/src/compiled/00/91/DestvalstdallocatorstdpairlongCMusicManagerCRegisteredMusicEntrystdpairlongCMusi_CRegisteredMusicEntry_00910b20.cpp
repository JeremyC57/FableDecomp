// CCreatureAction_DeclarePlayerACriminal::~CCreatureAction_DeclarePlayerACriminal() -- the compiler-generated destructor: ~CBaseIntelligentPointer on PCriminal (+0xA8), then the out-of-line ~CCreatureActionBase as the tail jump; no vtable store
// Leaf: ??1CCreatureAction_DeclarePlayerACriminal@@UAE@XZ
// Retail: Fable.exe @ 0x00910B20. No caller (an unreferenced COMDAT the linker kept).
// Identity: the catalog names it; the class's constructors 0x00910C30 / 0x00911020 store vtable 0x0128F624 and CIntelligentPointer<CThingPlayerCreature> 0x01231C4C at +0xA8;
//   that vtable's slot 0 (the scalar deleting destructor) expands the same sequence. Ego_r.pdb CCreatureAction_DeclarePlayerACriminal : CCreatureActionBase has
//   PCriminal (CIntelligentPointer<CThingPlayerCreature>) right after the base (retail CCreatureActionBase is 0xA8 bytes);
//   FableWin.pdb ??1CCreatureAction_DeclarePlayerACriminal@@UAE@XZ (compiler-generated: no vtable store).
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (Ego_r.pdb / FableWin.pdb ??1CBaseIntelligentPointer@@UAE@XZ),
//   CCreatureActionBase::~CCreatureActionBase 0x00693EF0 (??1CCreatureActionBase@@UAE@XZ, tail jump).
// Model: the shared engine/CIntelligentPointer.h and engine/CCreatureActionBase.h (retail branches).
//   No engine header models CCreatureAction_DeclarePlayerACriminal, so it is declared here.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "engine/CIntelligentPointer.h"
#include "engine/CCreatureActionBase.h"
class CThing;
class CThingCreatureBase;
class CThingPlayerCreature;

class CCreatureAction_DeclarePlayerACriminal : public CCreatureActionBase {                      // vtable 0x0128F624
public:
    CCreatureAction_DeclarePlayerACriminal(CThingCreatureBase& creature, CThingPlayerCreature& criminal);   // 0x00910C30 (Ego_r.pdb / FableWin.pdb ??0CCreatureAction_DeclarePlayerACriminal@@QAE@...)
    CIntelligentPointer<CThingPlayerCreature> PCriminal;        // +0xA8
};

// Emission harness (not a retail function): the class declares no destructor (retail's stores no vtable); a qualified call
// makes VC7.1 emit the implicit destructor's COMDAT.
void DestroyDeclarePlayerACriminal(CCreatureAction_DeclarePlayerACriminal* action)
{
    action->CCreatureAction_DeclarePlayerACriminal::~CCreatureAction_DeclarePlayerACriminal();
}
