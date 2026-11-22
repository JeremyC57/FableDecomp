// CTCInventoryTrade::RecurseRefreshEntriesAmountsAndAvailabilitiesPC(NUISystem::CComponent* component, std::map<NUISystem::CComponent*, NUISystem::CList*>& lists) -- PC trade screen: as the console RefreshEntriesAmountsAndAvailabilities (0x0059DB82) but it recurses only into sold-out type-7 children and always hides the PC_MENU_ENTRY_ARROW (resetting its action) when nothing was counted
// Leaf: RecurseRefreshEntriesAmountsAndAvailabilitiesPC
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059E6BD, 656 bytes, `ret 0Ch` (hidden pair result). Frame-pointer size TU: optimize("y",off) + ("s",on).
// FableWin.pdb ?RecurseRefreshEntriesAmountsAndAvailabilitiesPC@CTCInventoryTrade@@AAE?AU?$pair@KK@std@@PAVCComponent@NUISystem@@AAV?$map@...@Z;
// called by the landed RefreshEntriesAmountsAndAvailabilitiesPC 0x0059FAFE.
// Callees: as 0x0059DB82 (same component slots, map find 0x005B1A43, CCharString, CreateComponent, counted pointer, CAction).
// Name fix (agentNF 2026-11-10): the local CComponentPointer struct is the shared CCountedPointer<NUISystem::CComponent> (as the
//   coordinator did for the console twin 0x0059DB82): ctor 0x00429C15, release 0x004291DE = ReleaseData (FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA;
//   the destructor is the 5-byte jmp COMDAT 0x00429C53).
// Shape: the console twin's model; the found-flag is declared before the children reference, which moves its zero store above the
// GetChildren call as in retail.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
// NF50 (round 7): the local std::pair / CListMap / SListMapNode stand-ins are the shared stlport/fable_map.h
// map<NUISystem::CComponent*, NUISystem::CList*> (FableWin.pdb parameter type), with STLport's _M_find member template called out of line
// (FABLE_RETAIL_STLP_TREE_MFIND_NOINLINE), so the call @ 0x005B1A43 binds
// ??$_M_find@PAVCComponent@NUISystem@@@?$_Rb_tree@PAVCComponent@NUISystem@@U?$pair@QAVCComponent@NUISystem@@PAVCList@2@@std@@U?$_Select1st@U?$pair@QAVCComponent@NUISystem@@PAVCList@2@@std@@@4@U?$less@PAVCComponent@NUISystem@@@4@V?$allocator@U?$pair@QAVCComponent@NUISystem@@PAVCList@2@@std@@@4@@std@@ABEPAU?$_Rb_tree_node@U?$pair@QAVCComponent@NUISystem@@PAVCList@2@@std@@@1@ABQAVCComponent@NUISystem@@@Z
// (nf44 entry 53). The row's own leaf is now the PDB symbol: the real map parameter (AAV?$map@..., was AAUCListMap@@) and the PDB
// access (AAE private, as engine/CTCInventoryTrade.h and the landed callers bind it; was QAE).
#define FABLE_RETAIL_STLP_TREE_MFIND_NOINLINE
// NF241 (2026-11-22, the list<CComponent*> family pass): the out-of-line list destructor call binds _List_base<CComponent*>'s
//   destructor 0x0042ABCA (was FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE, binding list's own name there; retail 0x0042B6D2 is list's
//   size-optimised `~list() {}`, a 5-byte `jmp 0x0042ABCA`). Same bytes.
#define FABLE_RETAIL_STLP_LIST_BASE_DTOR_VISIBLE_NOINLINE
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
    std::pair<unsigned long, unsigned long> RecurseRefreshEntriesAmountsAndAvailabilitiesPC(NUISystem::CComponent* component, std::map<NUISystem::CComponent*, NUISystem::CList*>& lists);
};

std::pair<unsigned long, unsigned long> CTCInventoryTrade::RecurseRefreshEntriesAmountsAndAvailabilitiesPC(NUISystem::CComponent* component, std::map<NUISystem::CComponent*, NUISystem::CList*>& lists)
{
    std::pair<unsigned long, unsigned long> result(0, 0);
    bool hasList = false;
    const std::vector<CCountedPointer<NUISystem::CComponent> >& children = component->GetChildren();
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
                    if (children[i]->GetChildren()[j]->GetType() == NUISystem::UI_TYPE_MENU_ENTRY && children[i]->GetChildren()[j]->GetDeletion().Method == NUISystem::CDeletion::UI_DO_NOT_DELETE) {
                        std::pair<unsigned long, unsigned long> sub = RecurseRefreshEntriesAmountsAndAvailabilitiesPC(children[i]->GetChildren()[j].Data, lists);
                        result.first += sub.first + 1;
                        result.second += sub.second;
                    }
                }
            }
            hasList = true;
        }
    }
    if (hasList && result.first != 0) {
        if (!component->FindChildNonRecursive("PC_MENU_ENTRY_ARROW")) {
            component->MakeAvailableDynamically();
            CCountedPointer<NUISystem::CComponent> arrow(NUISystem::CManager::GetInstance()->CreateComponent("PC_MENU_ENTRY_ARROW", false));
            component->AddChildDynamically(arrow);
        }
    } else {
        component->MakeUnavailableDynamically();
        NUISystem::CComponent* arrow = component->FindChildNonRecursive("PC_MENU_ENTRY_ARROW");
        if (arrow) {
            NUISystem::CDeletion action;
            action.Method = NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
            arrow->SetDeletion(action);
        }
    }
    return result;
}