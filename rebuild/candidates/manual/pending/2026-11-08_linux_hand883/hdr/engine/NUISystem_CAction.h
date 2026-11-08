#ifndef FABLE_ENGINE_NUISYSTEM_CACTION_H
#define FABLE_ENGINE_NUISYSTEM_CACTION_H
// NUISystem::CAction (new shared header, chunk 867, row 0x005F59C7 CTCInventoryAbilities::ConstructPCExpressionsList): the UI
// action record a component runs when it is selected / dropped / ... Ego_r.pdb layout (struct_layouts_egor.tsv, 32 bytes with the
// 12-byte debug STLport lists): EActionType ActionType +0, list<CActionParam*> UIActionParams +4, list<long> ThingActionParams,
// bool Valid; with retail's 4-byte STLport 4.6 list (FABLE_RETAIL_STLP_LIST, one node pointer) the record is 0x10 bytes
// (`push 10h ; call operator new` before every ??0CAction@NUISystem@@QAE@XZ 0x0042BE50 call in the PC inventory rows, and the
// landed local models of the CTCInventory* rows: ThingActionParams +0x08, Valid +0x0C). Methods, FableWin.pdb spellings, out of
// line in retail: ??0CAction@NUISystem@@QAE@XZ 0x0042BE50, ?AddParam@CAction@NUISystem@@QAEXPAVCActionParam@2@@Z 0x0053FDB0 and
// ?AddParam@CAction@NUISystem@@QAEXPAVCActionParam@2@AAV?$CCountedPointer@VCAction@NUISystem@@@@@Z 0x0053FDE0 (the component
// parameter is the CComponent's CActionParam base at +0x18, so a CComponent* converts in place: `add eax,18h`, null-guarded
// when the pointer may be null). The parameter list is filled directly: `UIActionParams.push_back(new CActionParamULONG(n))`
// is retail's `call ?push_back@?$list@PAVCActionParam@NUISystem@@...` 0x0042B646 with the new pointer homed in a temporary.
// Define FABLE_RETAIL_STLP_LIST with it (the retail list layout).
#include "rebuild_abi.h"
#include "engine/NUISystem_EActionType.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
namespace NUISystem {
class CAction {
public:
#ifdef FABLE_RETAIL_CACTION_INLINE_CTOR
    // Retail opt-in (chunk 883, row 0x006096E0 CTCInventoryMap::ConstructLogbookListPC): the ctor body is inline, as in the
    // landed 0x0042BE50 COMDAT (ActionType = 0, the two empty lists, Valid = true). A speed TU expands it after `new CAction`
    // (0x006097BE: `mov [ebx],0 ; mov [ebx+4],0 ; call malloc(0Ch)` ... `mov byte ptr [ebx+0Ch],1`) until the caller's inline
    // budget runs out, then calls the COMDAT (0x0060A97F in the same row).
    CAction() : ActionType(UI_ACTION_TYPE_NONE), Valid(true) { }            // 0x0042BE50
#else
    CAction();                                                              // 0x0042BE50
#endif
    void AddParam(CActionParam* param);                                     // 0x0053FDB0
    void AddParam(CActionParam* param, CCountedPointer<CAction>& self);     // 0x0053FDE0
    EActionType ActionType;                                                 // +0x00
    std::list<CActionParam*> UIActionParams;                                // +0x04
    std::list<long> ThingActionParams;                                      // +0x08
    bool Valid;                                                             // +0x0c
};
}
#ifdef FABLE_RETAIL_STLP_LIST
FABLE_STATIC_ASSERT(sizeof(NUISystem::CAction) == 0x10);
#endif
#endif // FABLE_ENGINE_NUISYSTEM_CACTION_H
