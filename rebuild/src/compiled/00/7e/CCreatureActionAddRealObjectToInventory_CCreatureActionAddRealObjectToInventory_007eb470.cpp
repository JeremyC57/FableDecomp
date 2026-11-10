// CCreatureAction_AddRealObjectToInventory::~CCreatureAction_AddRealObjectToInventory() -- the compiler-generated destructor: ~CBaseIntelligentPointer on PReceivedObject (+0xA8), then the out-of-line ~CCreatureActionBase as the tail jump; no vtable store
// Leaf: ??1CCreatureAction_AddRealObjectToInventory@@UAE@XZ
// Retail: Fable.exe @ 0x007EB470. No caller (an unreferenced COMDAT the linker kept).
// Identity: it follows the class's Clone 0x007EB3F0 and copy constructor 0x007EB420 (vtable 0x01270D5C, CIntelligentPointer<CThingObject> vtable 0x0125A858 at +0xA8);
//   that vtable's slot 0 (the scalar deleting destructor) expands the same sequence. Ego_r.pdb CCreatureAction_AddRealObjectToInventory : CCreatureActionBase has
//   PReceivedObject (CIntelligentPointer<CThingObject>) right after the base (retail CCreatureActionBase is 0xA8 bytes);
//   FableWin.pdb ??1CCreatureAction_AddRealObjectToInventory@@UAE@XZ (compiler-generated: no vtable store).
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (Ego_r.pdb / FableWin.pdb ??1CBaseIntelligentPointer@@UAE@XZ),
//   CCreatureActionBase::~CCreatureActionBase 0x00693EF0 (??1CCreatureActionBase@@UAE@XZ, tail jump).
// Model: the shared engine/CIntelligentPointer.h and engine/CCreatureActionBase.h (retail branches).
//   No engine header models CCreatureAction_AddRealObjectToInventory, so it is declared here.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "engine/CIntelligentPointer.h"
#include "engine/CCreatureActionBase.h"
class CThing;
class CThingCreatureBase;
class CThingObject;

class CCreatureAction_AddRealObjectToInventory : public CCreatureActionBase {                      // vtable 0x01270D5C
public:
    CCreatureAction_AddRealObjectToInventory(CThingCreatureBase& creature, CThingObject& object);   // 0x007EB2D0 (Ego_r.pdb / FableWin.pdb ??0CCreatureAction_AddRealObjectToInventory@@QAE@...)
    CIntelligentPointer<CThingObject> PReceivedObject;          // +0xA8
};

// Emission harness (not a retail function): the class declares no destructor (retail's stores no vtable); a qualified call
// makes VC7.1 emit the implicit destructor's COMDAT.
void DestroyAddRealObjectToInventory(CCreatureAction_AddRealObjectToInventory* action)
{
    action->CCreatureAction_AddRealObjectToInventory::~CCreatureAction_AddRealObjectToInventory();
}
