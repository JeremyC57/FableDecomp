// CCreatureAction_ShowTrophy::`scalar deleting destructor'(unsigned int) -- the compiler-generated deleting destructor: the implicit ~CCreatureAction_ShowTrophy expanded (~CBaseIntelligentPointer on PEmitter +0xB5 and PTrophyBeingShown +0xA8, then the out-of-line ~CCreatureActionBase 0x00693EF0), then operator delete when bit 0 of the flags is set
// Leaf: ??_GCCreatureAction_ShowTrophy@@UAEPAXI@Z
// Retail: Fable.exe @ 0x006DF7C0. Identity: slot 0 of vtable 0x0125F7B4, RTTI .?AVCCreatureAction_ShowTrophy@@ (the catalog name
//   agrees); it follows the class's constructor 0x006DF730 in its TU.
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (Ego_r.pdb / FableWin.pdb ??1CBaseIntelligentPointer@@UAE@XZ),
//   CCreatureActionBase::~CCreatureActionBase 0x00693EF0, operator delete 0x00BFE9BC.
// Layout: Ego_r.pdb CCreatureAction_ShowTrophy : CCreatureActionBase (retail 0xA8 bytes): PTrophyBeingShown +0xA8
//   (CIntelligentPointer<CThing>), ShowingTrophyDeedHandle +0xB0, PostedDeed +0xB4, PEmitter +0xB5 (CIntelligentPointer<CThing>,
//   byte-packed). No engine header models the class, so it is declared here over the shared engine/CCreatureActionBase.h and
//   engine/CIntelligentPointer.h (retail branches); the constructors are the Ego_r.pdb spellings. The class declares no destructor.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "engine/CIntelligentPointer.h"
#include "engine/CCreatureActionBase.h"
class CThing;
class CThingCreatureBase;
class CCreatureAction_ShowTrophy : public CCreatureActionBase {          // vtable 0x0125F7B4
public:
    CCreatureAction_ShowTrophy(CThingCreatureBase& creature, CThing& trophy, const CCharString& animation, long deed);   // 0x006DF730
    virtual void PerformAction();                                       // Ego_r.pdb ?PerformAction@CCreatureAction_ShowTrophy@@UAEXXZ
    virtual void FinishAction();
    virtual CCreatureActionBase* Clone() const;                         // Ego_r.pdb ?Clone@..@@UBEPAVCCreatureActionBase@@XZ
    CIntelligentPointer<CThing> PTrophyBeingShown;                      // +0xA8
    unsigned long ShowingTrophyDeedHandle;                              // +0xB0
    bool PostedDeed;                                                    // +0xB4
    CIntelligentPointer<CThing> PEmitter;                               // +0xB5
};

// Emission harness (not a retail function): an inline copy construction emits the vftable and its deleting destructor.
CCreatureActionBase* EmitShowTrophyDeletingDestructor(const CCreatureAction_ShowTrophy& action)
{
    return new CCreatureAction_ShowTrophy(action);
}
