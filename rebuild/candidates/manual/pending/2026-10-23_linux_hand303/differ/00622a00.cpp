// CTCInventoryQuests::OpenInventory() -- opens the quest screen
// Grade: DIFFER(877v877) -- ours caches 0 in ebx for the whole function (push ebx / cmp x,ebx / mov [..],bl); retail uses immediates and zeroes edi/ebp locally -- zero-register allocation
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
struct CThing;
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
namespace NUISystem {
struct CList;
struct CComponent {
#define V(n) virtual void v##n();
    V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19)
    V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29) V(30) V(31) V(32) V(33) V(34) V(35) V(36) V(37) V(38) V(39)
    V(40) V(41) V(42)
    virtual void Reset();                               // slot 43 (+0xac)
    V(44) V(45) V(46) V(47)
#undef V
    virtual void ChangeState(long state);               // slot 48 (+0xc0)
};
}
struct CTCInventoryQuests;
struct CFrontEndManager {
#define V(n) virtual void v##n();
    V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16)
    virtual void SetQuestInventory(CTCInventoryQuests* inventory);   // slot 17 (+0x44)
    V(18) V(19) V(20) V(21) V(22) V(23) V(24) V(25)
    virtual void SetPlayerThing(CThing* thing);         // slot 26 (+0x68), name unproven
#undef V
    NUISystem::CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
    static CFrontEndManager* __fastcall GetInstance();  // 0x0041E5F2
};
struct __true_type { __true_type() { } };
struct CComponentVector {
    NUISystem::CComponent** _M_start;
    NUISystem::CComponent** _M_finish;
    NUISystem::CComponent** _M_end_of_storage;
    void _M_insert_overflow(NUISystem::CComponent** position, NUISystem::CComponent* const& x, const __true_type&, unsigned long fill_len, bool atend);   // 0x00485DBC
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
struct CThingVector {
    CThing** _M_start;
    CThing** _M_finish;
    CThing** _M_end_of_storage;
    CThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CThingVector() { if (_M_start) free(_M_start); }
    void _M_insert_overflow(CThing** position, CThing* const& x, const __true_type&, unsigned long fill_len, bool atend);   // 0x00436AC0
    void push_back(CThing* const& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                *_M_finish = x;
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
#pragma auto_inline(off)
void CComponentVector::_M_insert_overflow(NUISystem::CComponent** position, NUISystem::CComponent* const& x, const __true_type&, unsigned long fill_len, bool atend)
{
    unsigned long oldSize = _M_finish - _M_start;
    unsigned long len = oldSize + (oldSize < fill_len ? fill_len : oldSize);
    NUISystem::CComponent** newStart = len ? (NUISystem::CComponent**)malloc(len * sizeof(void*)) : 0;
    NUISystem::CComponent** newFinish = newStart;
    for (NUISystem::CComponent** p = _M_start; p != position; ++p, ++newFinish)
        *newFinish = *p;
    for (unsigned long i = 0; i < fill_len; ++i, ++newFinish)
        *newFinish = x;
    if (!atend)
        for (NUISystem::CComponent** p = position; p != _M_finish; ++p, ++newFinish)
            *newFinish = *p;
    if (_M_start)
        free(_M_start);
    _M_start = newStart;
    _M_finish = newFinish;
    _M_end_of_storage = newStart + len;
}
void CThingVector::_M_insert_overflow(CThing** position, CThing* const& x, const __true_type&, unsigned long fill_len, bool atend)
{
    unsigned long oldSize = _M_finish - _M_start;
    unsigned long len = oldSize + (oldSize < fill_len ? fill_len : oldSize);
    CThing** newStart = len ? (CThing**)malloc(len * sizeof(void*)) : 0;
    CThing** newFinish = newStart;
    for (CThing** p = _M_start; p != position; ++p, ++newFinish)
        *newFinish = *p;
    for (unsigned long i = 0; i < fill_len; ++i, ++newFinish)
        *newFinish = x;
    if (!atend)
        for (CThing** p = position; p != _M_finish; ++p, ++newFinish)
            *newFinish = *p;
    if (_M_start)
        free(_M_start);
    _M_start = newStart;
    _M_finish = newFinish;
    _M_end_of_storage = newStart + len;
}
#pragma auto_inline(on)
struct CIntelligentPointer {
    virtual ~CIntelligentPointer();
    void* m_pItem;
    CThing* GetPItem() const;                           // 0x00A01B50
};
struct CIntelligentPointerVector {
    CIntelligentPointer* _M_start;
    CIntelligentPointer* _M_finish;
    CIntelligentPointer* _M_end_of_storage;
    CIntelligentPointerVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CIntelligentPointerVector()
    {
        for (CIntelligentPointer* p = _M_start; p != _M_finish; ++p)
            p->~CIntelligentPointer();
        if (_M_start)
            free(_M_start);
    }
};
struct CTCQuestCards {
    void GetQuestCards(CIntelligentPointerVector& out); // 0x0077DBC0
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CThing {
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                    // +0x20
    CInterfaceMap  m_interfaces;                        // +0x44
    unsigned char  _pad4c[0x91 - 0x4c];
    unsigned char  Flags91;                             // +0x91
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
struct CHudOwner {
#define V(n) virtual void v##n();
    V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19)
    V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29) V(30) V(31) V(32) V(33) V(34) V(35) V(36) V(37) V(38) V(39)
    V(40) V(41) V(42) V(43) V(44) V(45) V(46) V(47) V(48) V(49) V(50) V(51) V(52) V(53) V(54) V(55) V(56) V(57) V(58) V(59)
    V(60) V(61) V(62) V(63) V(64) V(65) V(66) V(67) V(68) V(69) V(70) V(71) V(72) V(73) V(74) V(75)
#undef V
    virtual void SetEnabled(bool enabled);              // slot 76 (+0x130)
};
struct CWorldPart { unsigned char _pad[0x2c]; CHudOwner* Hud; };   // +0x2c
struct CPlayer { CThing* GetHeroCreature();             // 0x00487DC0
};
struct CPlayerManager { CPlayer* GetMainPlayer();       // 0x00449970
};
struct CMainGameComponent {
    unsigned char _pad00[0x1c];
    CPlayerManager* PlayerManager;                      // +0x1c
    unsigned char _pad20[0x28 - 0x20];
    CWorldPart* World;                                  // +0x28
};
extern CMainGameComponent* g_pMainGameComponent;      // 0x013B86A0
struct CSoundSystem {
#define V(n) virtual void v##n();
    V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19)
    V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29) V(30) V(31) V(32) V(33) V(34) V(35) V(36) V(37) V(38) V(39)
    V(40) V(41) V(42) V(43) V(44) V(45)
#undef V
    virtual void RefreshCategory(long category, float a, float b);   // slot 46 (+0xB8)
};
extern CSoundSystem* g_pSoundSystem;                 // 0x013B8394
void __fastcall OpenMenus();                          // 0x0041D122
namespace NHeroInformationScreens {
struct CBase {
    virtual void v00(); virtual void v01();
    virtual long GetScreenType();                    // slot 2 (+0x08)
    virtual bool IsActive();                         // slot 3 (+0x0C)
};
struct CManager {
    void SetScreen(long screen);                     // 0x0066E799 (name unproven)
    CBase* GetCurrentScreen();                       // 0x0066E7BD
};
}
struct CPlayerGui {
    NHeroInformationScreens::CManager* GetPHeroInfoScreensManager() const;   // 0x00437CD0
};
extern CPlayerGui* g_pPlayerGui;                     // 0x013B8790
struct CQuestTree {                                  // STLport _Rb_tree: header, count, compare
    struct Node { long color; Node* parent; Node* left; Node* right; };
    Node* _M_header;
    unsigned long _M_node_count;
    long _M_key_compare;
    void _M_erase(Node* x);                          // 0x00623560
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_header->parent);
            _M_header->left = _M_header;
            _M_header->parent = 0;
            _M_header->right = _M_header;
            _M_node_count = 0;
        }
    }
};
struct CPointerVector {
    void** _M_start;
    void** _M_finish;
    void** _M_end_of_storage;
    void** erase(void** first, void** last);         // 0x004AC0F0
    void clear() { erase(_M_start, _M_finish); }
};
struct CTCInventoryQuests {
#define V(n) virtual void v##n();
    V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19)
    V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29) V(30) V(31) V(32) V(33) V(34) V(35) V(36) V(37) V(38) V(39)
    V(40) V(41) V(42) V(43) V(44) V(45) V(46) V(47) V(48)
#undef V
    virtual bool IsInventoryScreen();                // slot 49 (+0xC4)
    CThing*          ParentThing;                    // +0x04
    unsigned char    _pad08[0x0c - 0x08];
    CComponentVector Components;                     // +0x0c
    unsigned char    _pad18[0x130 - 0x18];
    NHeroInformationScreens::CBase Screen;           // +0x130
    unsigned char    _pad134[0x14c - 0x134];
    bool             CategoryFlag;                   // +0x14c
    unsigned char    _pad14d[0x150 - 0x14d];
    long             NumberOfCards;                  // +0x150
    unsigned char    _pad154[0x157 - 0x154];
    bool             ScreenWasActive;                // +0x157
    unsigned char    _pad158[0x168 - 0x158];
    long             PreviousScreen;                 // +0x168
    CQuestTree       QuestTree;                      // +0x16c
    CPointerVector   QuestEntries;                   // +0x178
    void InitialiseQuestCardList();                  // 0x00621A20
    long GetSelectedIndex();                         // 0x005BC4B2
    void SetSelectedIndex(long index);               // 0x005BC4B6
    void BaseOpenInventory();                        // 0x005BD4EA (CTCInventoryBase::OpenInventory)
    CThing* GetAssociatedThing();                    // 0x005BC4C4
    void ConstructQuestList(const CThingVector& things, NUISystem::CList* list, long type, long type2);   // 0x0061B610
    void OpenInventory();
};
void CTCInventoryQuests::OpenInventory()
{
    CFrontEndManager::GetInstance()->SetPlayerThing(g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetHeroCreature());
    PreviousScreen = -1;
    if (!Screen.IsActive()) {
        if (g_pSoundSystem) {
            g_pSoundSystem->RefreshCategory(0x10, 0.0f, 500.0f);
            g_pSoundSystem->RefreshCategory(0x40, 0.0f, 500.0f);
            g_pSoundSystem->RefreshCategory(0x80, 0.0f, 500.0f);
        }
        CFrontEndManager::GetInstance()->SetPlayerThing(ParentThing);
        g_pMainGameComponent->World->Hud->SetEnabled(false);
        OpenMenus();
        PreviousScreen = g_pPlayerGui->GetPHeroInfoScreensManager()->GetCurrentScreen()->GetScreenType();
        ScreenWasActive = false;
    } else {
        CPlayerGui* gui = g_pPlayerGui;
        gui->GetPHeroInfoScreensManager()->SetScreen(Screen.GetScreenType());
        ScreenWasActive = true;
    }
    CategoryFlag = IsInventoryScreen();
    InitialiseQuestCardList();
    if (NumberOfCards > 0) {
        long last = NumberOfCards - 1;
        if (GetSelectedIndex() > last)
            SetSelectedIndex(last);
    }
    BaseOpenInventory();
    if (Screen.IsActive())
        return;
    CFrontEndManager::GetInstance()->SetQuestInventory(this);
    CFrontEndManager* frontEnd = CFrontEndManager::GetInstance();
    NUISystem::CComponent* component = frontEnd->CreateComponent("UI_QUESTS_SELECTION_MENU", false);
    CThingVector things;
    CThing* thing = GetAssociatedThing();
    if (thing && !(thing->Flags91 & 1)) {
        CTCQuestCards* cards = 0;
        if (thing->HasTC(0x6a))
            cards = thing->GetTC<CTCQuestCards>(0x6a);
        CIntelligentPointerVector items;
        cards->GetQuestCards(items);
        for (CIntelligentPointer* it = items._M_start; it != items._M_finish; ++it) {
            if (it->GetPItem() && !(it->GetPItem()->Flags91 & 1)) {
                it->GetPItem();
                things.push_back(it->GetPItem());
            }
        }
    }
    QuestTree.clear();
    QuestEntries.clear();
    ConstructQuestList(things, (NUISystem::CList*)component, 0, 0);
    component->Reset();
    component->ChangeState(5);
    Components.push_back(component);
}
