// CTCInventoryTrade::RecurseRefreshEntriesAmountsAndAvailabilitiesPC(NUISystem::CComponent* component, CListMap& lists) -- PC trade screen: as the console RefreshEntriesAmountsAndAvailabilities (0x0059DB82) but it recurses only into sold-out type-7 children and always hides the PC_MENU_ENTRY_ARROW (resetting its action) when nothing was counted
// Leaf: RecurseRefreshEntriesAmountsAndAvailabilitiesPC
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059E6BD, 656 bytes, `ret 0Ch` (hidden pair result). Frame-pointer size TU: optimize("y",off) + ("s",on).
// FableWin.pdb ?RecurseRefreshEntriesAmountsAndAvailabilitiesPC@CTCInventoryTrade@@AAE?AU?$pair@KK@std@@PAVCComponent@NUISystem@@AAV?$map@...@Z;
// called by the landed RefreshEntriesAmountsAndAvailabilitiesPC 0x0059FAFE.
// Callees: as 0x0059DB82 (same component slots, map find 0x005B1A43, CCharString, CreateComponent, counted pointer, CAction).
// Shape: the console twin's model; the found-flag is declared before the children reference, which moves its zero store above the
// GetChildren call as in retail.
#pragma optimize("y", off)
#pragma optimize("s", on)
namespace std {
template <class A, class B> struct pair {
    A first;
    B second;
    pair(const A& a, const B& b) : first(a), second(b) { }
};
}
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
namespace NUISystem {
class CComponent;
struct CActionParamList {                       // STLport list<CActionParam*> (one node pointer)
    void* _M_node;
    ~CActionParamList();                        // 0x0042ABCA
};
namespace N_0042bed4 { struct CAction {                                // retail 8 bytes: ActionType +0, UIActionParams +4
    long             ActionType;
    CActionParamList UIActionParams;
    CAction();                                  // 0x0042BED4
    CAction(const CAction& other);              // 0x0042CD84
}; } using N_0042bed4::CAction;
struct CComponentPointer {                      // CCountedPointer<NUISystem::CComponent>
    CComponent* m_pObject;
    void*       m_pCount;
    CComponentPointer(CComponent* object);      // 0x00429C15
    ~CComponentPointer();                       // 0x004291DE
};
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
struct SListMapNode {                           // STLport _Rb_tree_node<pair<CComponent* const, CList*> >
    long          _M_color;
    SListMapNode* _M_parent;
    SListMapNode* _M_left;
    SListMapNode* _M_right;
    NUISystem::CComponent* Key;                 // +0x10
    NUISystem::CList*      List;                // +0x14
};
struct CListMap {                               // std::map<NUISystem::CComponent*, NUISystem::CList*> (12 bytes)
    SListMapNode* _M_header;
    unsigned long _M_node_count;
    long          _M_key_compare;
    SListMapNode* find(NUISystem::CComponent* const& key);   // 0x005B1A43
    SListMapNode* end() const { return _M_header; }
};
#pragma auto_inline(off)
__declspec(noinline) SListMapNode* CListMap::find(NUISystem::CComponent* const& key)
{
    SListMapNode* y = _M_header;
    SListMapNode* x = _M_header->_M_parent;
    while (x != 0) {
        if (!(x->Key < key)) { y = x; x = x->_M_left; }
        else x = x->_M_right;
    }
    if (y == _M_header || key < y->Key) y = _M_header;
    return y;
}
#pragma auto_inline(on)

class CTCInventoryTrade { public:
    std::pair<unsigned long, unsigned long> RecurseRefreshEntriesAmountsAndAvailabilitiesPC(NUISystem::CComponent* component, CListMap& lists);
};

std::pair<unsigned long, unsigned long> CTCInventoryTrade::RecurseRefreshEntriesAmountsAndAvailabilitiesPC(NUISystem::CComponent* component, CListMap& lists)
{
    std::pair<unsigned long, unsigned long> result(0, 0);
    bool hasList = false;
    const NUISystem::CChildVector& children = component->GetChildren();
    for (unsigned long i = 0; i < children.size(); ++i) {
        if (children[i]->GetType() == 8) {
            SListMapNode* found = lists.find(component);
            if (found != lists.end()) {
                NUISystem::CList* list = found->List;
                for (unsigned long j = 0; j < list->GetChildren().size(); ++j) {
                    if (list->GetChildren()[j]->GetType() == 7 && *list->GetChildren()[j]->GetAmountLeft() == 0) {
                        ++result.first;
                        ++result.second;
                    }
                }
            } else {
                for (unsigned long j = 0; j < children[i]->GetChildren().size(); ++j) {
                    if (children[i]->GetChildren()[j]->GetType() == 7 && *children[i]->GetChildren()[j]->GetAmountLeft() == 0) {
                        std::pair<unsigned long, unsigned long> sub = RecurseRefreshEntriesAmountsAndAvailabilitiesPC(children[i]->GetChildren()[j].m_pObject, lists);
                        result.first += sub.first + 1;
                        result.second += sub.second;
                    }
                }
            }
            hasList = true;
        }
    }
    if (hasList && result.first != 0) {
        if (!component->FindChild("PC_MENU_ENTRY_ARROW")) {
            component->ShowArrow();
            NUISystem::CComponentPointer arrow(NUISystem::CManager::GetInstance()->CreateComponent("PC_MENU_ENTRY_ARROW", false));
            component->AddChildAtFront(arrow);
        }
    } else {
        component->HideArrow();
        NUISystem::CComponent* arrow = component->FindChild("PC_MENU_ENTRY_ARROW");
        if (arrow) {
            NUISystem::CAction action;
            action.ActionType = 1;
            arrow->SetAction(action);
        }
    }
    return result;
}