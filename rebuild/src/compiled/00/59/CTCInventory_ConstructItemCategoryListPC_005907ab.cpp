// CTCInventory::ConstructItemCategoryListPC(CWideString& title, NUISystem::CList* list, std::vector<std::pair<long,long> >& items, NUISystem::CComponent* owner, const CCharString& templateName, const CCharString& descriptionSymbol, bool checkUsable) -- PC inventory category: a branch entry captioned "title x <total count>" with a subtitle, the category description (shown in the owner's viewport) and an optional template component, all registered with the owner's frame; an empty category clears the entry, otherwise the items are merged per def into a map, a sub-list titled "<title>: <name>" gets the merged count, and an items-actions list (the "can't use" variant, with its not-usable entry cleared and the icon action reset, when the hero's carried thing is not alive) gets the same title before ContructItemListsPC fills them
// Leaf: ConstructItemCategoryListPC@CTCInventory
// Retail: Fable.exe (TLC, VC7.1) @ 0x005907AB, 1415 bytes, `ret 1Ch`. Frame-pointer size TU: optimize("y",off) + ("s",on).
//   FableWin.pdb ?ConstructItemCategoryListPC@CTCInventory@@QAEXAAVCWideString@@PAVCList@NUISystem@@AAV?$vector@...@ABVCCharString@@4_N@Z
//   (the catalog label "CTCInventoryAbilities::ConstructPCExpressionCategoryList" is propagated).
// Callees: CFrontEndManager::GetInstance 0x0041E5F2, CManager::CreateComponent 0x0041DB1D / CreateMenuEntryComponent 0x00428494,
//   CCountedPointer<CComponent>(T*) 0x00429C15 / Reset 0x00429C6F / dtor 0x004291DE, CTCInventory::CreateMenuDescPC 0x00582673
//   (CCountedPointer by hidden pointer), GetXString 0x005CC772, IntToWideString 0x00405F00, CWideString copy 0x0099B720 /
//   += 0x0099B8D0 / += (const wchar*) 0x0099B940 / ~ 0x0099B510, CCharString 0x0099EBF0 / ~ 0x0099EAE0 / != 0x0099E960,
//   map<long,long> ctor 0x004DF86C / find 0x00592F92 / insert 0x004E539F / dtor 0x004DD2B1, std::bitset::test 0x00410DE0 +
//   const CVectorMap::LowerBound 0x004365B0 (the parent thing's interface 8), IS_THING_ALIVE 0x0040DCE0 (`__fastcall`,
//   CBaseObject* const&), NUISystem::CAction() 0x0042BED4 / copy 0x0042CD84 / param-list dtor 0x0042ABCA,
//   CTCInventory::ContructItemListsPC 0x00590198. The global name 0x013B8B4C is CCharString("ITEMS TITLE").
// Shape: the CTCInventoryAbilities::ConstructPCExpressionCategoryList 0x005F45B5 family (this chunk); the total count is a
//   plain index loop VC strength-reduces; the merge loop index is signed against the pointer difference (`jl`); the
//   actions-list name is a ternary on a `usable` flag (VC threads the flag test, laying the normal-name arm out of line)
//   built as a named CCharString; the merged entry goes in through `insert(make_pair(key, count))` (the count is loaded
//   into a register before the key store; a direct pair constructor stores it at once).
// Layout: retail CTCInventory ParentThing +0x04; engine/CTCInventory.h is the PDB layout, so `this` is a local model.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    bool operator!=(const char* text) const;            // 0x0099E960
};
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600
class CWideString { public:
    void* PStringData;
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    const CWideString& operator+=(const CWideString& other);        // 0x0099B8D0
    const CWideString& operator+=(const unsigned short* text);      // 0x0099B940
};
CWideString __fastcall GetXString();                    // 0x005CC772
CWideString __fastcall IntToWideString(long value);     // 0x00405F00
struct CDataBank {
    CWideString GetTextBySymbol(const CCharString& symbol);   // 0x009C95E0
};
struct CGameSubsystem {
    unsigned char _pad00[0x14];
    CDataBank*    TextBank;                             // +0x14
};
extern CCharString g_ItemsTitleName;                  // 0x013B8B4C ("ITEMS TITLE")
template<class T> class CCountedPointer { public:
    T*    m_pObject;
    void* m_pCount;
    CCountedPointer() : m_pObject(0), m_pCount(0) {}
    CCountedPointer(T* object);                         // 0x00429C15
    void Reset(T* object);                              // 0x00429C6F
    ~CCountedPointer();                                 // 0x004291DE
    T* operator->() const { return m_pObject; }
};
namespace NUISystem {
struct CActionParamList {                               // STLport list<CActionParam*> (one node pointer)
    void* _M_node;
    ~CActionParamList();                                // 0x0042ABCA
};
class CAction { public:                                        // console action record, 8 bytes
    long             ActionType;
    CActionParamList UIActionParams;
    CAction();                                          // 0x0042BED4
    CAction(const CAction& other);                      // 0x0042CD84
};
class CComponent { public:
    virtual void v000(); virtual void v001(); virtual void v002();
    virtual CComponent* FindChild(CCharString name);                       // slot 3 (+0x0C)
    virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
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
    virtual void AddChildAtEnd(const CCountedPointer<CComponent>& child);  // slot 59 (+0xEC)
    virtual void v060(); virtual void v061();
    virtual void SetAction(CAction action);                                    // slot 62 (+0xF8)
    virtual void v063(); virtual void v064();
    virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082();
    virtual void SetName(const CCharString& name);                         // slot 83 (+0x14C)
    virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088();
    virtual void v089(); virtual void v090(); virtual void v091();
    virtual void Clear();                                                  // slot 92 (+0x170)
    virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097();
    virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102();
    virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112();
    virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117();
    virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122();
    virtual void v123();
    virtual void AddFrameMember(CComponent* member);                       // slot 124 (+0x1F0)
    virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129();
    virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134();
    virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139();
    virtual void v140(); virtual void v141();
    virtual const CWideString& GetText();                                  // slot 142 (+0x238)
    virtual void v143();
    virtual void SetText(const CWideString& text, bool a, bool b);         // slot 144 (+0x240)
    virtual void SetHeader(const CCountedPointer<CComponent>& header);     // slot 145 (+0x244)
    virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150();
    virtual void v151();
    virtual void AddEntry(const CCountedPointer<CComponent>& entry);       // slot 152 (+0x260)
    virtual void v153(); virtual void v154(); virtual void v155();
    virtual void SetItemCount(long count);                                 // slot 156 (+0x270)
    virtual void v157();
    virtual void SetSelectedIndex(long index);                             // slot 158 (+0x278)
};
class CViewport { public:                                      // the description viewport (its slot 142 takes the content)
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
    virtual void v140(); virtual void v141();
    virtual void SetContent(CComponent* content);                          // slot 142 (+0x238)
};
class CList : public CComponent { public:};
class CManager { public:
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
    CComponent* CreateMenuEntryComponent(const CWideString& text, float a, float b, const CCharString& definition,
                                         float c, CCharString d, float e, CCharString button);   // 0x00428494
};
class CFrontEndManager : public CManager { public:
    static CFrontEndManager* __fastcall GetInstance();  // 0x0041E5F2
};
}
using namespace NUISystem;
namespace std {
template <class A, class B> struct pair {
    A first;
    B second;
    pair(const A& a, const B& b) : first(a), second(b) {}
    template <class U, class V> pair(const pair<U, V>& p) : first(p.first), second(p.second) {}
};
template <class A, class B> inline pair<A, B> make_pair(const A& a, const B& b) { return pair<A, B>(a, b); }
template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) { return _M_start[n]; }
    bool empty() const { return _M_start == _M_finish; }
};
}
typedef std::vector<std::pair<long, long> > CItemVector;
struct CPresenceBits { bool test(long kind) const; };  // 0x00410DE0
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
class CBaseObject;
bool __fastcall IS_THING_ALIVE(CBaseObject* const& thing);   // 0x0040DCE0
struct CTCCarryingSlot {                                // the parent thing's interface 8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual CBaseObject* GetHeldThing();                // slot 33 (+0x84)
};
class CThing { public:
    unsigned char _pad00[0x20];
    CPresenceBits m_present;                            // +0x20
    unsigned char _pad21[0x44 - 0x21];
    CInterfaceMap m_interfaces;                         // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long kind) const
    {
        if (m_present.test(kind)) {
            long key = kind;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&key);
            *out = (T*)((it == map->last || kind < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CLongMapNode {
    long          _M_color;
    CLongMapNode* _M_parent;
    CLongMapNode* _M_left;
    CLongMapNode* _M_right;
    long          first;                                // +0x10
    long          second;                               // +0x14
};
struct CLongLongMap {                                   // std::map<long, long> (12 bytes)
    CLongMapNode* _M_header;
    unsigned long _M_node_count;
    long          _M_key_compare;
    typedef std::pair<const long, long> value_type;
    struct insert_result {
        CLongMapNode* first;
        bool          second;
        insert_result(CLongMapNode* a, bool b) : first(a), second(b) {}
    };
    CLongLongMap();                                     // 0x004DF86C
    ~CLongLongMap();                                    // 0x004DD2B1
    CLongMapNode* find(const long& key);                // 0x00592F92
    insert_result insert(const value_type& value);      // 0x004E539F
    unsigned long size() const { return _M_node_count; }
};
class CTCInventory { public:
    void*   __vftable;                                  // +0x00
    CThing* ParentThing;                                // +0x04
    CCountedPointer<CComponent> CreateMenuDescPC(const CCharString& descriptionSymbol);   // 0x00582673
    void ContructItemListsPC(CLongLongMap& items, CList* subList, CComponent* owner, CCountedPointer<CComponent>& actionsList);   // 0x00590198
    void ConstructItemCategoryListPC(CWideString& title, CList* list, CItemVector& items, CComponent* owner,
                                     const CCharString& templateName, const CCharString& descriptionSymbol, bool checkUsable);
};
void CTCInventory::ConstructItemCategoryListPC(CWideString& title, CList* list, CItemVector& items, CComponent* owner,
                                               const CCharString& templateName, const CCharString& descriptionSymbol, bool checkUsable)
{
    CFrontEndManager* fe = CFrontEndManager::GetInstance();
    CViewport* viewport = (CViewport*)owner->FindChild("PC_DESCRIPTION_VIEWPORT");
    CComponent* frame = owner->FindChild("PC_UI_FRAME");
    CCountedPointer<CComponent> subtitle;
    CWideString caption(title);
    if (!items.empty()) {
        caption += GetXString();
        long total = 0;
        for (unsigned long i = 0; i < items.size(); ++i)
            total += items[i].second;
        caption += IntToWideString(total);
    }
    subtitle.Reset(fe->CreateComponent("PC_SUBTITLE", false));
    subtitle->SetText(caption, true, true);
    CCountedPointer<CComponent> entry(fe->CreateMenuEntryComponent(caption, 0.0f, 1.0f, "PC_MENU_ENTRY_BRANCH_TEMPLATE",
                                                                   0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
    entry->AddChildAtEnd(subtitle);
    if (frame)
        frame->AddFrameMember(subtitle.m_pObject);
    list->AddEntry(entry);
    CCountedPointer<CComponent> description = CreateMenuDescPC(descriptionSymbol);
    entry->AddChildAtEnd(description);
    if (frame)
        frame->AddFrameMember(description.m_pObject);
    viewport->SetContent(description.m_pObject);
    if (templateName != "") {
        CCountedPointer<CComponent> extra(fe->CreateComponent(templateName, false));
        entry->AddChildAtEnd(extra);
        if (frame)
            frame->AddFrameMember(extra.m_pObject);
    }
    if (items.empty()) {
        entry->Clear();
    } else {
        CLongLongMap merged;
        for (long i = 0; i < items._M_finish - items._M_start; ++i) {
            std::pair<long, long>* item = &items[i];
            long key = item->first;
            CLongMapNode* found = merged.find(key);
            if (found == merged._M_header)
                merged.insert(std::make_pair(key, item->second));
            else
                found->second += item->second;
        }
        CCountedPointer<CComponent> subList(fe->CreateComponent("PC_SUB_LIST", false));
        CComponent* sub = subList.m_pObject;
        entry->AddChildAtEnd(subList);
        sub->SetItemCount(merged.size());
        CCountedPointer<CComponent> titleComp(fe->CreateComponent("PC_ITEM_TITLE", false));
        CComponent* titleObject = titleComp.m_pObject;
        CWideString titleText(titleObject->GetText());
        titleText += L":";
        titleText += title;
        titleObject->SetText(titleText, true, true);
        titleComp->SetName(g_ItemsTitleName);
        sub->SetHeader(titleComp);
        if (frame)
            frame->AddFrameMember(titleComp.m_pObject);
        bool usable = true;
        CTCCarryingSlot* carrying;
        if (checkUsable && ParentThing->GetInterface(&carrying, 8) && !IS_THING_ALIVE(carrying->GetHeldThing()))
            usable = false;
        CCharString actionsListName(usable ? "PC_UI_LIST_ITEMS_ACTIONS" : "PC_UI_LIST_ITEMS_ACTIONS_CANT_USE");
        CCountedPointer<CComponent> actions(CFrontEndManager::GetInstance()->CreateComponent(actionsListName, false));
        CComponent* actionsList = actions.m_pObject;
        actionsList->SetSelectedIndex(0);
        if (!usable) {
            CComponent* notUsable = actionsList->FindChild("PC_UI_MENU_ENTRY_ITEM_ACTION_NOT_USE");
            notUsable->Clear();
            CComponent* icon = notUsable->FindChild("UI_ACTION_ICON");
            if (icon) {
                CAction reset;
                reset.ActionType = 1;
                icon->SetAction(reset);
            }
        }
        CCountedPointer<CComponent> actionsTitle(fe->CreateComponent("PC_ITEM_TITLE", false));
        actionsTitle->SetText(titleText, true, true);
        actionsTitle->SetName(g_ItemsTitleName);
        actionsList->SetHeader(actionsTitle);
        if (frame)
            frame->AddFrameMember(actionsTitle.m_pObject);
        ContructItemListsPC(merged, (CList*)sub, owner, actions);
    }
}