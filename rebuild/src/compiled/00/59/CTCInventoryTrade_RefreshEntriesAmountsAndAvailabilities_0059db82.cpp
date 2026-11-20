// CTCInventoryTrade::RefreshEntriesAmountsAndAvailabilities(NUISystem::CComponent* component, std::map<NUISystem::CComponent*, NUISystem::CList*>& lists) -- console trade screen: for each list child (type 8) of a category component, counts the sold-out entries of its mapped list (both counts) or, with no mapped list, recurses into its type-7 children adding one plus their counts; then shows the right selection arrow (UI_SELECTION_ARROW_RIGHT) when something was counted, else hides it and resets the arrow's action
// Leaf: RefreshEntriesAmountsAndAvailabilities@CTCInventoryTrade
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059DB82, 642 bytes, `ret 0Ch` (hidden pair result). Frame-pointer size TU: optimize("y",off) + ("s",on).
// Label: the catalog's "RecurseRefreshEntriesAmountsAndAvailabilitiesPC" is propagated; this body calls itself and uses the console
// arrow, so it is the console RefreshEntriesAmountsAndAvailabilities (FableWin.pdb ?RefreshEntriesAmountsAndAvailabilities@CTCInventoryTrade
// @@AAE?AU?$pair@KK@std@@PAVCComponent@NUISystem@@AAV?$map@...@Z); the PC recursion is 0x0059E6BD.
// Callees: CComponent slots 8 (+0x20, FindChild(CCharString) by value), 53 (+0xD4 children), 60 (+0xF0), 62 (+0xF8, a CAction by
// value), 63 (+0xFC amount left), 65 (+0x104 type), 93 (+0x174) and 95 (+0x17C) (names of 60/62/93/95 descriptive); the map find
// @ 0x005B1A43 (same-TU STLport body, the landed 0x0059FAFE lever), CCharString @ 0x0099EBF0 / ~ @ 0x0099EAE0, CFrontEndManager::
// GetInstance @ 0x0041E5F2 + CreateComponent @ 0x0041DB1D, CCountedPointer<CComponent> @ 0x00429C15 / ~ @ 0x004291DE,
// NUISystem::CAction() @ 0x0042BED4 / copy @ 0x0042CD84 / its list dtor @ 0x0042ABCA.
// Shape: the landed RefreshEntriesAmountsAndAvailabilitiesPC (0x0059FAFE) model; the find key is the outer component (retail passes
// the component argument slot), the list children are re-fetched through the virtual on every access, the CreateComponent name is
// an implicit `const char*` conversion (its temporary lands in the dead `lists` slot), FindChild's argument is built in its slot.
// Name fix (agentNF 2026-11-10): 0x004291DE is CCountedPointer<NUISystem::CComponent>::ReleaseData (the destructor is the 5-byte
//   `jmp` COMDAT 0x00429C53; Ego_r.pdb ??1 is 5 bytes): FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (engine/CCountedPointer.h).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#include "engine/CCountedPointer.h"
// NF50 (round 7): the local std::pair / CListMap / SListMapNode stand-ins are the shared stlport/fable_map.h
// map<NUISystem::CComponent*, NUISystem::CList*> (FableWin.pdb parameter type), with STLport's _M_find member template called out of line
// (FABLE_RETAIL_STLP_TREE_MFIND_NOINLINE), so the call @ 0x005B1A43 binds
// ??$_M_find@PAVCComponent@NUISystem@@@?$_Rb_tree@PAVCComponent@NUISystem@@U?$pair@QAVCComponent@NUISystem@@PAVCList@2@@std@@U?$_Select1st@U?$pair@QAVCComponent@NUISystem@@PAVCList@2@@std@@@4@U?$less@PAVCComponent@NUISystem@@@4@V?$allocator@U?$pair@QAVCComponent@NUISystem@@PAVCList@2@@std@@@4@@std@@ABEPAU?$_Rb_tree_node@U?$pair@QAVCComponent@NUISystem@@PAVCList@2@@std@@@1@ABQAVCComponent@NUISystem@@@Z
// (nf44 entry 53). The row's own leaf is now the PDB symbol: the real map parameter (AAV?$map@..., was AAUCListMap@@) and the PDB
// access (AAE private, as engine/CTCInventoryTrade.h and the landed callers bind it; was QAE).
#define FABLE_RETAIL_STLP_TREE_MFIND_NOINLINE
#define FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE
#include "stlport/fable_map.h"
// NF156 (2026-11-20, q4776): the local CActionParamList stand-in replaced by the shared STLport std::list<NUISystem::CComponent*>
//   (stlport/fable_containers.h; ~list 0x0042ABCA out of line, FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE). The 8-byte object at
//   0x0042BED4 / 0x0042CD84 is NUISystem::CDeletion (catalog + FableWin.pdb ??0CDeletion@NUISystem@@QAE@XZ / ..@ABV01@@Z; the landed
//   0x0041E6D3 / 0x00545B10 bind them), so the member is CDeletion::AssociatedParents, list<CComponent*> (Ego_r.pdb member type,
//   engine/NUISystem_CDeletion.h), not a list of CActionParam*. The row-local N_0042bed4::CAction stand-in is gone since NF179 (below).
// NF179 (PRIORITY q11945 / q11946): the row-local NUISystem::CComponent model (vfunc slot placeholders), its CChildVector /
//   SChildPointer views, the N_0042bed4::CAction stand-in and the local CCharString / CManager / CList models are replaced by the
//   shared headers: engine/NUISystem_CComponent.h (slot 8 FindChildNonRecursive, 53 GetChildren -> vector<CCountedPointer<CComponent> >&,
//   60 AddChildDynamically, 62 SetDeletion, 63 GetDeletion, 65 GetType, 93 MakeUnavailableDynamically, 95 MakeAvailableDynamically:
//   the header's slot list = the local model's +0x20 / +0xD4 / +0xF0 / +0xF8 / +0xFC / +0x104 / +0x174 / +0x17C offsets),
//   engine/NUISystem_CDeletion.h (FABLE_RETAIL_CDELETION: the out-of-line ctors 0x0042BED4 / 0x0042CD84 now bind
//   ??0CDeletion@NUISystem@@QAE@XZ / ??0CDeletion@NUISystem@@QAE@ABV01@@Z, was ??0CAction@N_0042bed4@NUISystem@@...),
//   engine/NUISystem_CList.h, engine/NUISystem_CManager.h and fable_string.h (FABLE_RETAIL_CCHARSTRING). The amount test
//   `*GetAmountLeft() == 0` is GetDeletion().Method == UI_DO_NOT_DELETE (first member of the returned CDeletion&).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDELETION
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CDeletion.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CManager.h"

class CTCInventoryTrade { private:              // Ego_r.pdb / FableWin.pdb: private members (AAE), as engine/CTCInventoryTrade.h
    std::pair<unsigned long, unsigned long> RefreshEntriesAmountsAndAvailabilities(NUISystem::CComponent* component, std::map<NUISystem::CComponent*, NUISystem::CList*>& lists);
};

std::pair<unsigned long, unsigned long> CTCInventoryTrade::RefreshEntriesAmountsAndAvailabilities(NUISystem::CComponent* component, std::map<NUISystem::CComponent*, NUISystem::CList*>& lists)
{
    std::pair<unsigned long, unsigned long> result(0, 0);
    const std::vector<CCountedPointer<NUISystem::CComponent> >& children = component->GetChildren();
    bool hasList = false;
    for (unsigned long i = 0; i < children.size(); ++i) {
        if (children[i]->GetType() == NUISystem::UI_TYPE_LIST) {
            std::map<NUISystem::CComponent*, NUISystem::CList*>::iterator found = lists.find(component);
            if (found != lists.end()) {
                NUISystem::CList* list = found->second;
                for (unsigned long j = 0; j < list->GetChildren().size(); ++j) {
                    if (list->GetChildren()[j]->GetType() == NUISystem::UI_TYPE_MENU_ENTRY && list->GetChildren()[j]->GetDeletion().Method == NUISystem::CDeletion::UI_DO_NOT_DELETE) {
                        ++result.first;
                        ++result.second;
                    }
                }
            } else {
                for (unsigned long j = 0; j < children[i]->GetChildren().size(); ++j) {
                    if (children[i]->GetChildren()[j]->GetType() == NUISystem::UI_TYPE_MENU_ENTRY) {
                        std::pair<unsigned long, unsigned long> sub = RefreshEntriesAmountsAndAvailabilities(children[i]->GetChildren()[j].Data, lists);
                        result.first += sub.first + 1;
                        result.second += sub.second;
                    }
                }
            }
            hasList = true;
        }
    }
    if (hasList && result.first != 0) {
        if (!component->FindChildNonRecursive("UI_SELECTION_ARROW_RIGHT")) {
            component->MakeAvailableDynamically();
            CCountedPointer<NUISystem::CComponent> arrow(NUISystem::CManager::GetInstance()->CreateComponent("UI_SELECTION_ARROW_RIGHT", false));
            component->AddChildDynamically(arrow);
        }
    } else if (component->FindChildNonRecursive("UI_SELECTION_ARROW_RIGHT")) {
        component->MakeUnavailableDynamically();
        NUISystem::CComponent* arrow = component->FindChildNonRecursive("UI_SELECTION_ARROW_RIGHT");
        if (arrow) {
            NUISystem::CDeletion action;
            action.Method = NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
            arrow->SetDeletion(action);
        }
    }
    return result;
}