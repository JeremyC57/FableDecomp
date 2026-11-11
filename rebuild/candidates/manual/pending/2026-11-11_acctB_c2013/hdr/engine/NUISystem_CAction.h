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
    // Opt-in (chunk 2013, rows 0x00626110 / 0x00626750 CTCQuickAccessMenu::ContructItemLists, a speed TU): the constructor's body
    // (FableWin.pdb ??0CAction@NUISystem@@QAE@XZ; out of line at 0x0042BE50 = `and [esi],0`, the two STLport list constructors,
    // `mov byte [esi+0Ch],1`, the landed row's mem-initialisers) visible, so the speed TU expands it after `new(10h)` as retail
    // does (0x0062651D..0x00626546: ActionType 0, each list's null store then its malloc'ed self-linked head, Valid 1).
    CAction() : ActionType(UI_ACTION_TYPE_NONE), Valid(true) { }        // 0x0042BE50
#else
    CAction();                                                              // 0x0042BE50
#endif
    void AddParam(CActionParam* param);                                     // 0x0053FDB0
    void AddParam(CActionParam* param, CCountedPointer<CAction>& self);     // 0x0053FDE0
#ifdef FABLE_RETAIL_CACTION_ISVALID
    // chunk 886 (row 0x0041E6D3 NUISystem::CManager::Action): FableWin.pdb ?IsValid@CAction@NUISystem@@QAE_NXZ (non-const), inline
    // in retail: the dispatcher's entry test reads it through a byte register (0x0041E6EE `mov al,[eax+0Ch]; test al,al`), the
    // inline bool-getter shape, where a direct `!action->Valid` test compiles to `cmp byte ptr [eax+0Ch],0`.
    bool IsValid() { return Valid; }
#endif
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
