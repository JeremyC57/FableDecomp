// CTCInventoryClothing::ConstructClothingList(const std::vector<std::pair<long,long> >& items, NUISystem::CViewport* viewport, EClothingSuitPart part, bool plainEntries) -- console clothing list for one suit part: build the clothing-actions list with its back / select prompts and "CLOTHING TITLE" header, then (for a non-empty item list) a sub-list with the same prompts and title and one menu entry per item wired with select (viewport, entry), focus, unfocus, equip and remove actions, each entry either cleared (plain) or given the actions list, and registered with AddClothingMenuEntry; returns the sub-list (null when there are no items)
// Leaf: ConstructClothingList@CTCInventoryClothing
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B70B8, 1561 bytes, `ret 10h`. Biased frame (lea ebp,[esp-0x68]): optimize("s",on).
// Callees: as ConstructClothingListPC 0x005B6A66 (this chunk) minus OpenInventory and the frame registrations.
// Shape: the sub-list pointer starts null (edi doubles as the `false` argument until the sub-list replaces it) and the
//   sub-list prompts / title live in the non-empty block, so the empty path skips their destructors.
// Layout: engine/CTCInventoryClothing.h is the Ego_r.pdb layout and no data member is read here, so `this` is a method-only
//   local model; CAction 0x10 (thing-param list<long> at +0x08), CActionParamLong 0x0C (vtable 0x012366F4, type 3).
#pragma optimize("s", on)
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
class CWideString { public:
    void* PStringData;
    CWideString();                                      // 0x0099AED0
    CWideString(const unsigned short* text);            // 0x0099B6B0
    ~CWideString();                                     // 0x0099B510
};
class CCPPointerInfo;
template<class T> class CCountedPointer { public:
    T*              m_pObject;
    CCPPointerInfo* m_pCount;
    CCountedPointer() : m_pObject(0), m_pCount(0) {}
    CCountedPointer(T* object);
    ~CCountedPointer();
    void Reset(T* object);
    void ShareData(T* object, CCPPointerInfo* count);   // 0x00429460 for CText
    template<class U> __forceinline CCountedPointer& operator=(const CCountedPointer<U>& other)
    {
        ShareData((T*)other.m_pObject, other.m_pCount);
        return *this;
    }
    T* operator->() const { return m_pObject; }
};
namespace NUISystem {
class CActionParam { public:
    virtual void v00();
    long Type;                                          // +0x04
    CActionParam(long type) : Type(type) {}
};
struct CActionParamLong : CActionParam {                // vtable 0x012366F4
    long Value;                                         // +0x08
    CActionParamLong(long value) : CActionParam(3), Value(value) {}
};
struct CActionParamList {
    void* _M_node;
    void push_back(CActionParam* const& value);         // 0x0042B646
};
struct CLongList {
    void* _M_node;
    void push_back(const long& value);                  // 0x0042B687
};
}
void InsertParamNode(void* node, NUISystem::CActionParam* value);
#pragma auto_inline(off)
__declspec(noinline) void NUISystem::CActionParamList::push_back(NUISystem::CActionParam* const& value)
{
    InsertParamNode(_M_node, value);
}
#pragma auto_inline(on)
namespace NUISystem {
class CAction { public:
    long             ActionType;                        // +0x00
    CActionParamList UIActionParams;                    // +0x04
    CLongList        ThingActionParams;                 // +0x08
    bool             Valid;                             // +0x0c
    CAction();                                          // 0x0042BE50
    void AddParam(CActionParam* param, CCountedPointer<CAction>& self);   // 0x0053FDE0
};
struct CComponentBase {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014();
    virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034();
    virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044();
    virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054();
    virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058();
    virtual void AddChildAtEnd(const CCountedPointer<class CComponent>& child);     // slot 59 (+0xEC)
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void v065(); virtual void v066();
    virtual void AddSelectedAction(const CCountedPointer<CAction>& action);         // slot 67 (+0x10C)
    virtual void AddSpecialAction(const CCountedPointer<CAction>& action);          // slot 68 (+0x110)
    virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082();
    virtual void SetName(const CCharString& name);                                  // slot 83 (+0x14C)
    virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088();
    virtual void v089(); virtual void v090(); virtual void v091();
    virtual void Clear();                                                           // slot 92 (+0x170)
    virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097();
    virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102();
    virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112();
    virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117();
    virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122();
    virtual void v123();
    virtual void AddFrameMember(class CComponent* member);                         // slot 124 (+0x1F0)
    virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129();
    virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134();
    virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139();
    virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143();
    virtual void SetText(const CWideString& text, bool a, bool b);                  // slot 144 (+0x240, CText)
    virtual void SetHeader(const CCountedPointer<class CComponent>& header);       // slot 145 (+0x244)
    virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149();
    virtual void SetUserValue(long value);                                          // slot 150 (+0x258)
    virtual void v151();
    virtual void AddFocusAction(const CCountedPointer<CAction>& action);            // slot 152 (+0x260)
    virtual void AddUnfocusAction(const CCountedPointer<CAction>& action);          // slot 153 (+0x264)
    virtual void v154(); virtual void v155();
    virtual void SetItemCount(long count);                                          // slot 156 (+0x270)
    unsigned char _pad04[0x18 - 0x04];
};
class CComponent : public CComponentBase, public CActionParam { public:};
class CText : public CComponent { public:};
class CViewport : public CComponent { public:};
class CList { public:                                          // a list component (separate view of the vtable: slot 152 adds an entry)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014();
    virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034();
    virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044();
    virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054();
    virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084();
    virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089();
    virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094();
    virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099();
    virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104();
    virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109();
    virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114();
    virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119();
    virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124();
    virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129();
    virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134();
    virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139();
    virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144();
    virtual void SetHeader(const CCountedPointer<CComponent>& header);       // slot 145 (+0x244)
    virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150();
    virtual void v151();
    virtual void AddEntry(const CCountedPointer<CComponent>& entry);         // slot 152 (+0x260)
    virtual void v153(); virtual void v154(); virtual void v155();
    virtual void SetItemCount(long count);                                   // slot 156 (+0x270)
};
class CManager { public:
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
    CComponent* CreateMenuEntryComponent(const CWideString& text, float a, float b, const CCharString& definition,
                                         float c, CCharString d, float e, CCharString button);   // 0x00428494
    static CManager* __fastcall GetInstance();                      // 0x0041E5F2 ?GetInstance@CManager@NUISystem@@SIPAV12@XZ
};
}
using namespace NUISystem;
namespace std {
template <class A, class B> struct pair { A first; B second; };
template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    bool empty() const { return _M_start == _M_finish; }
    const T& operator[](unsigned long n) const { return _M_start[n]; }
};
}
typedef std::vector<std::pair<long, long> > CItemVector;
enum EClothingSuitPart { CLOTHING_SUIT_PART_NONE };
class CTCInventoryClothing { public:
    void ConstructTitleString(CWideString& out);        // 0x005B6881
    void AddClothingMenuEntry(EClothingSuitPart part, CComponent* entry);   // 0x005B4771
    CComponent* ConstructClothingList(const CItemVector& items, CViewport* viewport, EClothingSuitPart part, bool plainEntries);
};
CComponent* CTCInventoryClothing::ConstructClothingList(const CItemVector& items, CViewport* viewport, EClothingSuitPart part,
                                                        bool plainEntries)
{
    CManager* fe = CManager::GetInstance();
    CCountedPointer<CComponent> actions(fe->CreateComponent("UI_LIST_CLOTHING_ACTIONS", false));
    CComponent* actionsList = actions.m_pObject;
    CCountedPointer<CComponent> back(fe->CreateComponent("UI_BACK_FOR_LIST", false));
    CCountedPointer<CComponent> select(fe->CreateComponent("UI_SELECT_FOR_LIST", false));
    actionsList->SetHeader(back);
    actionsList->SetHeader(select);
    CCountedPointer<CComponent> title(fe->CreateComponent("UI_TITLE_NEW", false));
    CCountedPointer<CText> titleText;
    title->SetName("CLOTHING TITLE");
    titleText = title;
    CWideString titleString;
    ConstructTitleString(titleString);
    titleText->SetText(titleString, true, true);
    actionsList->SetHeader(title);
    CList* subList = 0;
    if (!items.empty()) {
        subList = (CList*)fe->CreateComponent("UI_SUB_LIST", false);
        CCountedPointer<CComponent> subBack(fe->CreateComponent("UI_BACK_FOR_LIST", false));
        CCountedPointer<CComponent> subSelect(fe->CreateComponent("UI_SELECT_FOR_LIST", false));
        subList->SetHeader(subBack);
        subList->SetHeader(subSelect);
        CCountedPointer<CComponent> subTitle(fe->CreateComponent("UI_TITLE_NEW", false));
        subTitle->SetName("CLOTHING TITLE");
        titleText = subTitle;
        titleText->SetText(titleString, true, true);
        subList->SetHeader(subTitle);
        subList->SetItemCount(items.size());
        for (unsigned long i = 0; i < items.size(); ++i) {
            long defIndex = items[i].first;
            CCountedPointer<CComponent> entry(fe->CreateMenuEntryComponent(L"INVALID NAME", 0.0f, 1.0f,
                                                                           "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW", 0.0f, "",
                                                                           0.0f, "PC_BUTTON_BASTARD"));
            CComponent* comp = entry.m_pObject;
            CCountedPointer<CAction> selectAction(new CAction);
            selectAction->ActionType = 0x7d2;
            selectAction->UIActionParams.push_back(new CActionParamLong(defIndex));
            selectAction->AddParam(viewport, selectAction);
            selectAction->AddParam(entry.m_pObject, selectAction);
            entry->AddSelectedAction(selectAction);
            CCountedPointer<CAction> focus(new CAction);
            focus->ActionType = 0x7e1;
            focus->AddParam(entry.m_pObject, focus);
            comp->AddFocusAction(focus);
            CCountedPointer<CAction> unfocus(new CAction);
            unfocus->ActionType = 0x7d5;
            unfocus->UIActionParams.push_back(new CActionParamLong(defIndex));
            unfocus->AddParam(entry.m_pObject, unfocus);
            comp->AddUnfocusAction(unfocus);
            comp->SetUserValue(defIndex);
            CCountedPointer<CAction> equip(new CAction);
            equip->ActionType = 0x1f;
            equip->ThingActionParams.push_back(defIndex);
            entry->AddSelectedAction(equip);
            CCountedPointer<CAction> remove(new CAction);
            remove->ActionType = 0x27;
            entry->AddSpecialAction(remove);
            subList->AddEntry(entry);
            if (plainEntries)
                entry->Clear();
            else
                entry->AddChildAtEnd(actions);
            AddClothingMenuEntry(part, entry.m_pObject);
        }
    }
    return (CComponent*)subList;
}