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
//   engine/NUISystem_CDeletion.h), not a list of CActionParam*. The row-local N_0042bed4::CAction stand-in itself is left (NOTES).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
namespace NUISystem {
class CComponent;
namespace N_0042bed4 { struct CAction {                                // retail 8 bytes: ActionType +0, UIActionParams +4
    long             ActionType;
    std::list<CComponent*> UIActionParams;
    CAction();                                  // 0x0042BED4
    CAction(const CAction& other);              // 0x0042CD84
}; } using N_0042bed4::CAction;
typedef CCountedPointer<CComponent> CComponentPointer;   // (T*) ctor 0x00429C15 (called), ~CCountedPointer 0x004291DE
class CManager { public:
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
    static CManager* __fastcall GetInstance();                      // 0x0041E5F2 ?GetInstance@CManager@NUISystem@@SIPAV12@XZ
};
struct SChildPointer {                          // CCountedPointer<NUISystem::CComponent>
    CComponent* m_pObject;
    void*       m_pCount;
    CComponent* operator->() const { return m_pObject; }
};
struct CChildVector {                           // vector<CCountedPointer<CComponent> > (12 bytes)
    SChildPointer* _M_start;
    SChildPointer* _M_finish;
    SChildPointer* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    const SChildPointer& operator[](unsigned long i) const { return _M_start[i]; }
};
class CComponent { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07();
    virtual CComponent* FindChild(CCharString name);              // slot 8 (+0x20)
    virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
    virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26();
    virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32();
    virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38();
    virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49(); virtual void v50();
    virtual void v51(); virtual void v52();
    virtual const CChildVector& GetChildren();                  // slot 53 (+0xD4)
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void AddChildAtFront(const CComponentPointer& child); // slot 60 (+0xF0) (name descriptive)
    virtual void v61();
    virtual void SetAction(CAction action);                      // slot 62 (+0xF8) (name descriptive)
    virtual const long* GetAmountLeft();                         // slot 63 (+0xFC)
    virtual void v64();
    virtual long GetType();                                      // slot 65 (+0x104)
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76(); virtual void v77();
    virtual void v78(); virtual void v79(); virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83();
    virtual void v84(); virtual void v85(); virtual void v86(); virtual void v87(); virtual void v88(); virtual void v89();
    virtual void v90(); virtual void v91(); virtual void v92();
    virtual void HideArrow();                                    // slot 93 (+0x174) (name descriptive)
    virtual void v94();
    virtual void ShowArrow();                                    // slot 95 (+0x17C) (name descriptive)
};
class CList : public CComponent { public:
};
}

class CTCInventoryTrade { private:              // Ego_r.pdb / FableWin.pdb: private members (AAE), as engine/CTCInventoryTrade.h
    std::pair<unsigned long, unsigned long> RefreshEntriesAmountsAndAvailabilities(NUISystem::CComponent* component, std::map<NUISystem::CComponent*, NUISystem::CList*>& lists);
};

std::pair<unsigned long, unsigned long> CTCInventoryTrade::RefreshEntriesAmountsAndAvailabilities(NUISystem::CComponent* component, std::map<NUISystem::CComponent*, NUISystem::CList*>& lists)
{
    std::pair<unsigned long, unsigned long> result(0, 0);
    const NUISystem::CChildVector& children = component->GetChildren();
    bool hasList = false;
    for (unsigned long i = 0; i < children.size(); ++i) {
        if (children[i]->GetType() == 8) {
            std::map<NUISystem::CComponent*, NUISystem::CList*>::iterator found = lists.find(component);
            if (found != lists.end()) {
                NUISystem::CList* list = found->second;
                for (unsigned long j = 0; j < list->GetChildren().size(); ++j) {
                    if (list->GetChildren()[j]->GetType() == 7 && *list->GetChildren()[j]->GetAmountLeft() == 0) {
                        ++result.first;
                        ++result.second;
                    }
                }
            } else {
                for (unsigned long j = 0; j < children[i]->GetChildren().size(); ++j) {
                    if (children[i]->GetChildren()[j]->GetType() == 7) {
                        std::pair<unsigned long, unsigned long> sub = RefreshEntriesAmountsAndAvailabilities(children[i]->GetChildren()[j].m_pObject, lists);
                        result.first += sub.first + 1;
                        result.second += sub.second;
                    }
                }
            }
            hasList = true;
        }
    }
    if (hasList && result.first != 0) {
        if (!component->FindChild("UI_SELECTION_ARROW_RIGHT")) {
            component->ShowArrow();
            NUISystem::CComponentPointer arrow(NUISystem::CManager::GetInstance()->CreateComponent("UI_SELECTION_ARROW_RIGHT", false));
            component->AddChildAtFront(arrow);
        }
    } else if (component->FindChild("UI_SELECTION_ARROW_RIGHT")) {
        component->HideArrow();
        NUISystem::CComponent* arrow = component->FindChild("UI_SELECTION_ARROW_RIGHT");
        if (arrow) {
            NUISystem::CAction action;
            action.ActionType = 1;
            arrow->SetAction(action);
        }
    }
    return result;
}