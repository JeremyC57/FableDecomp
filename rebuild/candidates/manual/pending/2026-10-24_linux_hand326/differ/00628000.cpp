// CTCQuickAccessMenu::Activate() -- opens the quick-access menu: clears the inventory's quick-access maps, builds the root list with an "Items" and an "Expressions" branch (each titled with its count) and, when either exists, shows it, registers it and puts the hero into the quick-access mode; otherwise the empty list is deleted
// Grade: DIFFER(1323v1315) -- callee-saved register permutation: retail keeps `this` in esi until it is spilled, the front-end manager in ebp and the items list in edi; ours keeps `this` in ebp, the manager in edi and the items list in ebp, which also moves the numExpressions / expressions-list frame slots (the CList* local, converted for push_back, fixed the frame size and put the root list in esi as retail)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00628000, 1315 bytes, plain `ret`. FableWin.pdb ?Activate@CTCQuickAccessMenu@@QAEXXZ.
// Callees: GFGetTime 0x009E1BC0, CVectorMap LowerBound 0x0040F020 (inline GetTC, type 0x11), the inventory maps' _M_erase
//   0x00592BCE, NUISystem::CFrontEndManager::GetInstance 0x0041E5F2 (+ its slots 29 / 21), operator new 0x00BFEA1A /
//   delete 0x00BFE9BC, NUISystem::CAction() 0x0042BE50, CCountedPointer<CAction>(CAction*) 0x0042AA29,
//   CCountedPointer<CComponent>(CComponent*) 0x00429C15 (both releases inline), CManager::CreateComponent 0x0041DB1D /
//   CreateMenuEntryComponent 0x00428494, GetItemsListUI 0x006276F0, GetExpressionsListUI 0x00627BE0,
//   CDataBank::GetTextBySymbol 0x009C95E0, GetXString 0x005CC772, IntToWideString 0x00405F00 (both `__fastcall`, hidden
//   result in ecx), CWideString::operator+= 0x0099B8D0 / ~ 0x0099B510, CCharString 0x0099EBF0 / ~ 0x0099EAE0,
//   vector<CComponent*>::_M_insert_overflow 0x00485DBC, CTCBase::GetPlayerManager 0x00686D40,
//   CPlayerManager::GetMainPlayer 0x00449970, CPlayer::AddMode 0x006345C0,
//   CPlayerGui::SetSpecialAbilityButtonsAsDisplaying 0x00437580 (g_pPlayerGui 0x013B8790); component slots 43, 48
//   (ChangeState), 59 (AddChild), 68 (+0x110, the entry's action), 152 (AddItem), 156 (visible entries) and slot 0
//   (deleting destructor).
// Shape: the action and the two branch entries are counted pointers with out-of-line constructors and inline releases;
//   the branch titles are named CWideString locals built straight from GetTextBySymbol; the root list goes into
//   Components through the inline STLport push_back; the map clears are the inline STLport _Rb_tree::clear().
// Layout: CTCQuickAccessMenu (retail 12-byte vector): Active +0x0C, Components +0x10, LastRenderTime +0x20,
//   FirstRenderFrame +0x28; the inventory's quick-access maps +0x1FC / +0x208 (12-byte STLport maps).
struct CTCBase;

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                     // 0x0099EAE0
};
struct CWideString {
    void* PStringData;
    CWideString& operator+=(const CWideString& other);  // 0x0099B8D0
    ~CWideString();                                     // 0x0099B510
};
CWideString __fastcall GetXString();                    // 0x005CC772
CWideString __fastcall IntToWideString(long value);     // 0x00405F00
struct CDataBank { CWideString GetTextBySymbol(const CCharString& symbol); };   // 0x009C95E0
struct CMainGameComponent { unsigned char _pad[0x14]; CDataBank* TextBank; };   // +0x14
extern CMainGameComponent* g_pMainGameComponent;       // 0x013B86A0
double __fastcall GFGetTime();                          // 0x009E1BC0
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
void __cdecl operator delete(void* block);              // 0x00BFE9BC

struct CCPPointerInfo {                                 // CCountedPointer count block
    long References;                                    // +0
    void (__fastcall* Destroy)(void* object);           // +4
    void* Object;                                       // +8
};
template <class T> struct CCountedPointer {
    T*              Data;                               // +0
    CCPPointerInfo* Info;                               // +4
    CCountedPointer(T* data);                           // 0x00429C15 (CComponent) / 0x0042AA29 (CAction)
    void Release()
    {
        if (Info) {
            --Info->References;
            if (Info->References == 0) {
                Info->Destroy(Info->Object);
                operator delete(Info);
            }
        }
    }
    ~CCountedPointer() { Release(); Data = 0; Info = 0; }
    T* operator->() const { return Data; }
};

struct __true_type { __true_type() {} };
struct CTCInventory;
namespace NUISystem {
struct CAction {                                        // 0x10 bytes
    unsigned long ActionType;                           // +0x00
    unsigned char _pad04[0x0c];
    CAction();                                          // 0x0042BE50
};
struct CComponent {
    virtual ~CComponent();                              // slot 0 (deleting destructor)
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42();
    virtual void RefreshLayout();                       // slot 43 (+0xAC), name descriptive
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void ChangeState(long state);               // slot 48 (+0xC0)
    virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58();
    virtual void AddChild(const CCountedPointer<CComponent>& child);     // slot 59 (+0xEC)
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64();
    virtual void v65(); virtual void v66(); virtual void v67();
    virtual void SetAction(const CCountedPointer<CAction>& action);      // slot 68 (+0x110), name descriptive
    virtual void v69(); virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74();
    virtual void v75(); virtual void v76(); virtual void v77(); virtual void v78(); virtual void v79();
    virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83(); virtual void v84();
    virtual void v85(); virtual void v86(); virtual void v87(); virtual void v88(); virtual void v89();
    virtual void v90(); virtual void v91(); virtual void v92(); virtual void v93(); virtual void v94();
    virtual void v95(); virtual void v96(); virtual void v97(); virtual void v98(); virtual void v99();
    virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104();
    virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109();
    virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114();
    virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119();
    virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124();
    virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129();
    virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134();
    virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139();
    virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144();
    virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149();
    virtual void v150(); virtual void v151();
    virtual void AddItem(const CCountedPointer<CComponent>& item);       // slot 152 (+0x260)
    virtual void v153(); virtual void v154(); virtual void v155();
    virtual void SetNumberOfVisibleEntries(long count); // slot 156 (+0x270), name descriptive
};
struct CList : CComponent { };
struct CManager {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20();
    virtual void SetInventory(CTCInventory* inventory); // slot 21 (+0x54), name descriptive
    virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26();
    virtual void v27(); virtual void v28();
    virtual void ResetInput();                          // slot 29 (+0x74), name descriptive
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
    CComponent* CreateMenuEntryComponent(const CWideString& text, float a, float b, const CCharString& definition,
                                         float c, CCharString d, float e, CCharString f);   // 0x00428494
};
struct CFrontEndManager : CManager {
    static CFrontEndManager* __fastcall GetInstance();  // 0x0041E5F2
};
}

struct SRbNode {
    long     _M_color;
    SRbNode* _M_parent;
    SRbNode* _M_left;
    SRbNode* _M_right;
};
struct CQuickAccessMap {                                // STLport map (12 bytes)
    SRbNode*      _M_header;
    unsigned long _M_node_count;
    unsigned char _M_key_compare;
    void _M_erase(SRbNode* node);                       // 0x00592BCE
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_header->_M_parent);
            _M_header->_M_left = _M_header;
            _M_header->_M_parent = 0;
            _M_header->_M_right = _M_header;
            _M_node_count = 0;
        }
    }
};
struct CTCInventory {                                   // interface 0x11
    unsigned char   _pad00[0x1fc];
    CQuickAccessMap QuickAccessItems;                   // +0x1FC
    CQuickAccessMap QuickAccessExpressions;             // +0x208
};

struct CComponentVector {                               // STLport vector<NUISystem::CComponent*> (12 bytes)
    NUISystem::CComponent** _M_start;
    NUISystem::CComponent** _M_finish;
    NUISystem::CComponent** _M_end_of_storage;
    void _M_insert_overflow(NUISystem::CComponent** position, NUISystem::CComponent* const& x, const __true_type&,
                            unsigned long fillLength, bool atEnd);   // 0x00485DBC
    void push_back(NUISystem::CComponent* const& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                *_M_finish = x;
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};

struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                     // +0x20 std::bitset of thing-component interface types
    CInterfaceMap m_interfaces;                         // +0x44
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline T* GetTC(long type, T*)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};

struct CPlayer { void AddMode(long mode, long parameter); };         // 0x006345C0
struct CPlayerManager { CPlayer* GetMainPlayer(); };                // 0x00449970
struct CPlayerGui { void SetSpecialAbilityButtonsAsDisplaying(bool a, bool b); };   // 0x00437580
extern CPlayerGui* g_pPlayerGui;                       // 0x013B8790

struct CTCQuickAccessMenu {
    void*            __vftable;                         // +0x00
    CThing*          ParentThing;                       // +0x04 (CTCBase)
    bool             ToBeRemoved;                       // +0x08
    bool             IsInGlobalUpdateTCs;               // +0x09
    unsigned char    _pad0a[2];
    bool             Active;                            // +0x0C
    CComponentVector Components;                        // +0x10
    bool             Quitting;                          // +0x1C
    double           LastRenderTime;                    // +0x20
    bool             FirstRenderFrame;                  // +0x28
    CPlayerManager* GetPlayerManager();                 // 0x00686D40 (CTCBase)
    NUISystem::CComponent* GetItemsListUI(long& numItems, NUISystem::CComponent* parent);             // 0x006276F0
    NUISystem::CComponent* GetExpressionsListUI(long& numExpressions, NUISystem::CComponent* parent); // 0x00627BE0
    void Activate();
};

void CTCQuickAccessMenu::Activate()
{
    if (Active)
        return;
    LastRenderTime = GFGetTime();
    CTCInventory* inventory = 0;
    if (ParentThing->HasTC(0x11))
        inventory = ParentThing->GetTC(0x11, (CTCInventory*)0);
    inventory->QuickAccessItems.clear();
    inventory->QuickAccessExpressions.clear();
    NUISystem::CFrontEndManager::GetInstance()->ResetInput();
    NUISystem::CFrontEndManager::GetInstance()->SetInventory(inventory);
    CCountedPointer<NUISystem::CAction> action(new NUISystem::CAction);
    action->ActionType = 0x4f;
    NUISystem::CFrontEndManager* manager = NUISystem::CFrontEndManager::GetInstance();
    NUISystem::CList* list = static_cast<NUISystem::CList*>(manager->CreateComponent("UI_LIST_QUICK_ACCESS_MENU", false));
    long numItems = 0;
    long numExpressions = 0;
    NUISystem::CComponent* itemsList = GetItemsListUI(numItems, list);
    NUISystem::CComponent* expressionsList = GetExpressionsListUI(numExpressions, list);
    long branches = 0;
    if (itemsList)
        branches = 1;
    if (expressionsList)
        ++branches;
    if (branches > 0)
        list->SetNumberOfVisibleEntries(branches);
    if (itemsList) {
        CWideString title = g_pMainGameComponent->TextBank->GetTextBySymbol("TEXT_GUI_MENU_ITEMS");
        if (numItems > 0) {
            title += GetXString();
            title += IntToWideString(numItems);
        }
        CCountedPointer<NUISystem::CComponent> entry(manager->CreateMenuEntryComponent(title, 0.0f, 1.0f,
            "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_FOR_QUICK_ACCESS_ROOT", 0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
        CCountedPointer<NUISystem::CComponent> branch(itemsList);
        entry->SetAction(action);
        entry->AddChild(branch);
        list->AddItem(entry);
    }
    if (expressionsList) {
        CWideString title = g_pMainGameComponent->TextBank->GetTextBySymbol("TEXT_GUI_MENU_EXPRESSIONS");
        if (numExpressions > 0) {
            title += GetXString();
            title += IntToWideString(numExpressions);
        }
        CCountedPointer<NUISystem::CComponent> entry(manager->CreateMenuEntryComponent(title, 25.0f, 2.0f / 3.0f,
            "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_FOR_QUICK_ACCESS_ROOT", 0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
        CCountedPointer<NUISystem::CComponent> branch(expressionsList);
        entry->SetAction(action);
        entry->AddChild(branch);
        list->AddItem(entry);
    }
    if (itemsList || expressionsList) {
        list->RefreshLayout();
        list->ChangeState(5);
        Components.push_back(list);
        Active = true;
        FirstRenderFrame = true;
        GetPlayerManager()->GetMainPlayer()->AddMode(0x24, 0);
        g_pPlayerGui->SetSpecialAbilityButtonsAsDisplaying(true, true);
    } else
        delete list;
}
