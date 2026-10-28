// CTCInventoryQuests::OpenInventory() -- open the quest screen: hand the UI manager the hero; when this is not an inventory screen quieten the sound groups, hand it the parent thing, disable 3D scene rendering, open the menus and remember the current hero-info screen type (ActiveTestQuestMode off), else select this screen type (ActiveTestQuestMode on); store the guild validity, initialise the quest card list, clamp the selected category to the last card, run the base OpenInventory; then (not an inventory screen) register as the quest inventory, create the UI_QUESTS_SELECTION_MENU list, gather the building thing's live quest cards, rebuild the quest list and push the list component
// Grade: DIFFER(880v877) -- PDB-exact model (SetPlayerThing/SetQuestInventory slots, CTCInventoryBase/CBase layout, real STLport vector with forceinline push_back); residue: the two local vectors' frame homes are swapped and retail keeps quests._M_finish in ebp across the gather loop (xor edi,edi / stores of edi,ebp,edi) where ours stores ebp three times and reloads -- lane gap: local STL container fields enregistered
// Leaf: ?OpenInventory@CTCInventoryQuests@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00622A00, 877 bytes; CTCInventoryQuests vtable 0x012585B4 slot 30.
extern "C" void __cdecl free(void* block);                                      // 0x00BFEA14

inline void* operator new(unsigned int, void* __p) { return __p; }

namespace std {

struct __true_type { __true_type() { } };
struct __false_type { };

template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; typedef __false_type is_POD_type; };
template <class _Tp> struct __type_traits<_Tp*> { typedef __true_type has_trivial_destructor; typedef __true_type is_POD_type; };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _Tp>
inline void _Destroy(_Tp* __first, _Tp* __last)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _T1, class _T2> inline void _Copy_Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;                                                      // +0x00
    _Tp* _M_finish;                                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;             // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp value_type;
    typedef value_type* pointer;
    typedef value_type* iterator;
    typedef unsigned int size_type;
    typedef typename __type_traits<_Tp>::is_POD_type _IsPODType;
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    __forceinline void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Copy_Construct(this->_M_finish, __x);
            ++this->_M_finish;
        } else
            _M_insert_overflow(this->_M_finish, __x, _IsPODType(), 1UL, true);
    }
    iterator erase(iterator __first, iterator __last);
    void clear() { erase(begin(), end()); }
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, size_type __fill_len,
                            bool __atend = false);
};

template <class _Arg1, class _Arg2, class _Result> struct binary_function { };
template <class _Tp> struct less : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Pair> struct _Select1st { };

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool _M_color;
    _Base_ptr _M_parent;                                                // +0x04
    _Base_ptr _M_left;                                                  // +0x08
    _Base_ptr _M_right;                                                 // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
template <class _Tp, class _Alloc> class _Rb_tree_base {
public:
    _STLP_alloc_proxy<_Rb_tree_node<_Tp>*, _Rb_tree_node<_Tp>, _Alloc> _M_header;   // +0x00
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
protected:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value>* _Link_type;
    unsigned int _M_node_count;                                         // +0x04
    _Compare _M_key_compare;                                            // +0x08
    _Base_ptr& _M_root() const { return this->_M_header._M_data->_M_parent; }
    _Base_ptr& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    _Base_ptr& _M_rightmost() const { return this->_M_header._M_data->_M_right; }
private:
    void _M_erase(_Link_type __x);
public:
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase((_Link_type)_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    _Rep_type _M_t;
    void clear() { _M_t.clear(); }
};

} // namespace std

class CCharString {
public:
    CCharString(const char* text, long length = -1);                   // 0x0099EBF0
    ~CCharString();                                                     // 0x0099EAE0
private:
    void* PStringData;
};

class CBaseObject {
public:
    virtual ~CBaseObject();
};
class CClass : public CBaseObject { };

enum ETCInterfaceType {
    TC_HERO_CENTRE = 0x6a                                               // descriptive enumerator (value: CTCHeroCentre slot 21)
};
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class C = CKeyPairCompareLess<K, V> >
class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    typedef value_type* iterator;
    value_type* _M_start;
    value_type* _M_finish;
    value_type* _M_end_of_storage;
    C Compare;                                                          // +0x0C
    bool Dirty;                                                         // +0x0D
    iterator LowerBound(const K& key);                                  // 0x0040F020
    iterator end() { return _M_finish; }
};

class CTCBase;
class CThing : public CClass {
public:
    unsigned char _pad_0x04[0x20 - 0x04];
    unsigned long UsingComponents[9];                                   // +0x20 bitset<274>
    CVectorMap<ETCInterfaceType, CTCBase*> Components;                  // +0x44
    unsigned char _pad_0x54[0x91 - 0x54];
    unsigned char Dead : 1;                                             // +0x91 bit 0
    bool IsDead() const { return Dead; }
    template <class T> __forceinline bool GetTC(T** out)
    {
        if (!(UsingComponents[T::InterfaceType / 32] & (1UL << (T::InterfaceType % 32))))
            return false;
        CVectorMap<ETCInterfaceType, CTCBase*>& components = Components;
        CVectorMap<ETCInterfaceType, CTCBase*>::iterator p = components.LowerBound((ETCInterfaceType)T::InterfaceType);
        if (p == components.end() || (ETCInterfaceType)T::InterfaceType < p->first)
            p = components.end();
        *out = (T*)p->second;
        return true;
    }
};
class CThingPhysical : public CThing { };
class CThingGameObject : public CThingPhysical { };
class CThingCreatureBase : public CThingGameObject { };
class CThingPlayerCreature : public CThingCreatureBase { };

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;                                      // 0x00A01B50
private:
    CBaseObject* PItem;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* operator->() const { return (T*)GetPItem(); }
    operator T*() const { return (T*)GetPItem(); }
};

class CBaseClass { };
class CBaseClassNonCopyable : public CBaseClass { };
class CTCBase : public CBaseClassNonCopyable {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                                // +0x04
    bool ToBeRemoved;                                                   // +0x08
    bool IsInGlobalUpdateTCs;                                           // +0x09
};
class CTCHeroCentre : public CTCBase {
public:
    enum { InterfaceType = TC_HERO_CENTRE };
    bool GetQuestCards(std::vector<CIntelligentPointer<CThing> >& quests) const;   // 0x0077DBC0
};

class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;               // 0x00487DC0
};
class CPlayerManager {
public:
    CPlayer& GetMainPlayer();                                           // 0x00449970
};
class CIEngine {
public:
#define S(n) virtual void Slot##n();
    S(00) S(01) S(02) S(03) S(04) S(05) S(06) S(07) S(08) S(09) S(10) S(11) S(12) S(13) S(14) S(15) S(16) S(17) S(18) S(19)
    S(20) S(21) S(22) S(23) S(24) S(25) S(26) S(27) S(28) S(29) S(30) S(31) S(32) S(33) S(34) S(35) S(36) S(37) S(38) S(39)
    S(40) S(41) S(42) S(43) S(44) S(45) S(46) S(47) S(48) S(49) S(50) S(51) S(52) S(53) S(54) S(55) S(56) S(57) S(58) S(59)
    S(60) S(61) S(62) S(63) S(64) S(65) S(66) S(67) S(68) S(69) S(70) S(71) S(72) S(73) S(74) S(75)
#undef S
    virtual void Enable3DSceneRendering(bool enable);                   // slot 76 (+0x130), CEngine 0x00B23F90
};
class CDisplayEngine {
public:
    unsigned char _pad_0x00[0x2c];
    CIEngine* Engine3D;                                                 // +0x2C
};
template <class T> class scoped_ptr {
public:
    T* operator->() const { return ptr; }
private:
    T* ptr;
};
class CMainGameComponent {
public:
    unsigned char _pad_0x00[0x1c];
    scoped_ptr<CPlayerManager> PPlayerManager;                          // +0x1C
    unsigned char _pad_0x20[0x28 - 0x20];
    scoped_ptr<CDisplayEngine> PDisplayEngine;                          // +0x28
};
extern CMainGameComponent* g_pMainGameComponent;                        // 0x013B86A0

class CSoundManagerLH {
public:
#define S(n) virtual void Slot##n();
    S(00) S(01) S(02) S(03) S(04) S(05) S(06) S(07) S(08) S(09) S(10) S(11) S(12) S(13) S(14) S(15) S(16) S(17) S(18) S(19)
    S(20) S(21) S(22) S(23) S(24) S(25) S(26) S(27) S(28) S(29) S(30) S(31) S(32) S(33) S(34) S(35) S(36) S(37) S(38) S(39)
    S(40) S(41) S(42) S(43) S(44) S(45)
#undef S
    virtual void SetGroupVolume(unsigned long groupMask, float volume, float fadeTime);   // slot 46 (+0xB8) 0x00A3E9D0 (descriptive)
};
extern CSoundManagerLH* g_pSoundSystem;                                 // 0x013B8394


namespace NHeroInformationScreens {
enum EScreenType { };
class CBase {
public:
    virtual void Slot00();                                              // slot 0 (CTCInventoryQuests: GetScreenName)
    virtual void Slot01();                                              // slot 1 (CTCInventoryQuests: GetPlayerMode)
    virtual EScreenType GetScreenType() const = 0;                      // slot 2 (+0x08)
    virtual bool IsInventoryScreen() const = 0;                         // slot 3 (+0x0C)
};
class CManager {
public:
    const CBase* GetCurrentScreen();                                    // 0x0066E7BD
    void SelectScreenOfType(EScreenType type);                          // 0x0066E799
};
}
class CPlayerGui {
public:
    NHeroInformationScreens::CManager* GetPHeroInfoScreensManager() const;   // 0x00437CD0
};
extern CPlayerGui* g_pPlayerGui;                                        // 0x013B8790

class CTCInventoryQuests;
namespace NUISystem {
class CComponent {
public:
#define S(n) virtual void Slot##n();
    S(00) S(01) S(02) S(03) S(04) S(05) S(06) S(07) S(08) S(09) S(10) S(11) S(12) S(13) S(14) S(15) S(16) S(17) S(18) S(19)
    S(20) S(21) S(22) S(23) S(24) S(25) S(26) S(27) S(28) S(29) S(30) S(31) S(32) S(33) S(34) S(35) S(36) S(37) S(38) S(39)
    S(40) S(41) S(42)
#undef S
    virtual void Initialise();                                          // slot 43 (+0xAC)
    virtual void Slot44(); virtual void Slot45(); virtual void Slot46(); virtual void Slot47();
    virtual void ChangeState(unsigned long state);                      // slot 48 (+0xC0)
};
class CChangingStateComponent : public CComponent { };
class CList : public CChangingStateComponent { };
class CManager {
public:
#define S(n) virtual void Slot##n();
    S(00) S(01) S(02) S(03) S(04) S(05) S(06) S(07) S(08) S(09) S(10) S(11) S(12) S(13) S(14)
#undef S
    virtual void SetWeaponsInventory(class CTCInventoryWeapons* inventory);           // slot 15 (+0x3C)
    virtual void SetMapInventory(class CTCInventoryMap* inventory);                   // slot 16 (+0x40)
    virtual void SetQuestInventory(CTCInventoryQuests* inventory);                    // slot 17 (+0x44)
    virtual void SetTCHeroExperienceInventory(class CTCHeroExperience* inventory);    // slot 18 (+0x48)
    virtual void SetAbilitiesInventory(class CTCInventoryAbilities* inventory);       // slot 19 (+0x4C)
    virtual void SetExperienceInventory(class CTCInventoryExperience* inventory);     // slot 20 (+0x50)
    virtual void SetInventory(class CTCInventory* inventory);                         // slot 21 (+0x54)
    virtual void SetInventoryClothing(class CTCInventoryClothing* inventory);         // slot 22 (+0x58)
    virtual void SetInventoryTrade(class CTCInventoryTrade* inventory);               // slot 23 (+0x5C)
    virtual void Slot24(); virtual void Slot25();
    virtual void SetPlayerThing(CThing* thing);                                       // slot 26 (+0x68)
    CComponent* CreateComponent(const CCharString& name, bool b);                      // 0x0041DB1D
    static CManager* __fastcall GetInstance();                                         // 0x0041E5F2
    static void __fastcall OnOpenMenus();                                              // 0x0041D122 (FableWin/Ego_r SIXXZ; open-menu counter)
};
}

enum QuestScreenType { };

namespace NInventory {
class CTCInventoryBase : public CTCBase {
public:
#define S(n) virtual void Slot##n();
    S(01) S(02) S(03) S(04) S(05) S(06) S(07) S(08) S(09) S(10) S(11) S(12) S(13) S(14) S(15) S(16) S(17) S(18) S(19)
    S(20) S(21) S(22) S(23) S(24) S(25) S(26) S(27) S(28) S(29)
#undef S
    virtual void OpenInventory();                                       // slot 30 (+0x78) 0x005BD4EA
#define S(n) virtual void Slot##n();
    S(31) S(32) S(33) S(34) S(35) S(36) S(37) S(38) S(39)
    S(40) S(41) S(42) S(43) S(44) S(45) S(46) S(47) S(48)
#undef S
    virtual bool GetInventoryGuildValidity();                           // slot 49 (+0xC4)
    long GetSelectedCategoryIndex() const;                              // 0x005BC4B2
    void SetSelectedCategoryIndex(long index);                          // 0x005BC4B6
    CThing* GetBuildingThing() const;                                   // 0x005BC4C4
    std::vector<NUISystem::CComponent*> Components;                     // +0x0C
    unsigned char _pad_0x18[0x130 - 0x18];
};
}

class CTCInventoryQuests : public NInventory::CTCInventoryBase, public NHeroInformationScreens::CBase {
public:
    virtual void OpenInventory();
    virtual bool GetInventoryGuildValidity();
    virtual NHeroInformationScreens::EScreenType GetScreenType() const;
    virtual bool IsInventoryScreen() const;
    void InitialiseQuestCardList();                                     // 0x00621A20
    void ConstructQuestList(const std::vector<CThing*>& quests, NUISystem::CList* list, QuestScreenType type,
                            NUISystem::CList* parentList);              // 0x0061B610
    unsigned char _pad_0x134[0x14c - 0x134];
    bool IsGuildValid;                                                  // +0x14C
    bool InventoryScreen;                                               // +0x14D
    long QuestCardCount;                                                // +0x150
    bool Teleport;                                                      // +0x154
    bool QuestCardActivatable;                                          // +0x155
    bool EnableQuestCardListInitialise;                                 // +0x156
    bool ActiveTestQuestMode;                                           // +0x157
    long TestQuestSelectedIndex;                                        // +0x158
    long TestQuestTopIndex;                                             // +0x15C
    bool TestQuestShowAll;                                              // +0x160
    long TestQuestListSize;                                             // +0x164
    NHeroInformationScreens::EScreenType PreviousScreenType;            // +0x168
    std::map<CThing*, NUISystem::CComponent*> AvailableQuests;          // +0x16C
    std::vector<CIntelligentPointer<CThing> > DisplayQuests;            // +0x178
};

void CTCInventoryQuests::OpenInventory()
{
    NUISystem::CManager::GetInstance()->SetPlayerThing(g_pMainGameComponent->PPlayerManager->GetMainPlayer().GetPControlledCreature());
    PreviousScreenType = (NHeroInformationScreens::EScreenType)-1;
    if (!IsInventoryScreen()) {
        if (g_pSoundSystem) {
            g_pSoundSystem->SetGroupVolume(0x10, 0.0f, 500.0f);
            g_pSoundSystem->SetGroupVolume(0x40, 0.0f, 500.0f);
            g_pSoundSystem->SetGroupVolume(0x80, 0.0f, 500.0f);
        }
        NUISystem::CManager::GetInstance()->SetPlayerThing(ParentThing);
        g_pMainGameComponent->PDisplayEngine->Engine3D->Enable3DSceneRendering(false);
        NUISystem::CManager::OnOpenMenus();
        PreviousScreenType = g_pPlayerGui->GetPHeroInfoScreensManager()->GetCurrentScreen()->GetScreenType();
        ActiveTestQuestMode = false;
    } else {
        CPlayerGui* gui = g_pPlayerGui;
        gui->GetPHeroInfoScreensManager()->SelectScreenOfType(GetScreenType());
        ActiveTestQuestMode = true;
    }
    IsGuildValid = GetInventoryGuildValidity();
    InitialiseQuestCardList();
    if (QuestCardCount > 0) {
        long last = QuestCardCount - 1;
        if (GetSelectedCategoryIndex() > last)
            SetSelectedCategoryIndex(last);
    }
    NInventory::CTCInventoryBase::OpenInventory();
    if (IsInventoryScreen())
        return;
    NUISystem::CManager::GetInstance()->SetQuestInventory(this);
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    NUISystem::CList* list = (NUISystem::CList*)manager->CreateComponent("UI_QUESTS_SELECTION_MENU", false);
    std::vector<CThing*> quests;
    CThing* building = GetBuildingThing();
    if (building && !building->IsDead()) {
        CTCHeroCentre* heroCentre = 0;
        building->GetTC(&heroCentre);
        std::vector<CIntelligentPointer<CThing> > questCards;
        heroCentre->GetQuestCards(questCards);
        for (std::vector<CIntelligentPointer<CThing> >::iterator it = questCards.begin(); it != questCards.end(); ++it) {
            if (*it)
                if (!(*it)->IsDead()) {
                    CThing* quest = *it;
                    quests.push_back(*it);
                }
        }
    }
    AvailableQuests.clear();
    DisplayQuests.clear();
    ConstructQuestList(quests, list, (QuestScreenType)0, 0);
    list->Initialise();
    list->ChangeState(5);
    Components.push_back(list);
}

