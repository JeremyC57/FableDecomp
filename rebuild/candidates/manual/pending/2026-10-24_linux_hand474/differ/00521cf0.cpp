// CThingManager::LoadMainPlayerCreatureState(CCharString& state) -- loads a saved hero state: builds the hero-state script, loads its entities (only the hero thing name) on the hero's current level, moves the loaded creature onto the hero's position, activates the loaded things and copies the loaded creature's hero components over to the hero
// Grade: DIFFER(448v446) -- frame-slot/register choice only: retail gives the hero the dead `state` argument slot and the loaded creature [esp+0x10] (spilled, ebp then holds the physics vtable), ours swaps the two slots and keeps the physics vtable (not the object) in [esp+0x18]; everything else (set/vector bodies, the edi-cached vector start) matches
// Leaf: ?LoadMainPlayerCreatureState@CThingManager@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00521CF0, 446 bytes (0x1BE + int3 padding to 448), thiscall `ret 4`.
// Identity: Ego_r.pdb ?LoadMainPlayerCreatureState@CThingManager@@QAEXAAVCCharString@@@Z (one CCharString& argument);
//   the catalog's "CThingManager_LoadMorphFromScript" is descriptive.
// Callees: CHeroStateScript(CCharString&) 0x0083E1F0 / GetText 0x0083E1B0 (a CCharString-only object: its destructor is
//   ~CCharString 0x0099EAE0), CCharString(const CCharString&) 0x0099EC30 (a copy of the hero thing name 0x013BAB08),
//   set<CCharString>::insert (_Rb_tree::insert_unique 0x004AC380) / _Rb_tree::_M_erase 0x004ABA70, malloc 0x00BFEA0E /
//   free 0x00BFEA14 (the set header node, 0x14 bytes, and the vector storage), CPlayerManager::GetMainPlayer 0x00449970,
//   CPlayer::GetHeroCreature 0x00487DC0, CThing::GetPosition 0x004C73D0, the world map's level lookup (slot 16, +0x40),
//   CThingManager::LoadEntitiesFromScript 0x00520D00, the physics component's SetPosition (slot 30, +0x78),
//   CThingManager::ActivateThings 0x0051E2F0, CThingManager::CopyHeroToHero 0x0051F0E0; the loading flag 0x013B8A88.
// Layout: retail CThingManager (my landed constructor 0x00523540): WorldMap +0x28, PlayerManager +0x30. CThing: the
//   thing-type byte +0x10 (2: a creature), PhysicsTC +0x60 (as the landed CThingGameObject rows).
// Shape: STLport set<CCharString> and vector<CThing*> written out (header node allocate + empty-initialise, clear +
//   deallocate). The vector's start stays in edi from the creature search to its destructor: the three thing-manager
//   callees that get the vector are same-TU bodies (CopyHeroToHero is the real 0x0051F0E0 body; LoadEntitiesFromScript
//   and ActivateThings are stand-ins that write / only read it), so VC7.1 knows the last two calls leave it alone.
extern "C" void* __cdecl malloc(unsigned int size);                                   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                            // 0x00BFEA14

inline void* operator new(unsigned int, void* p) { return p; }

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);                                             // 0x0099EC30
    ~CCharString();                                                                    // 0x0099EAE0
};

class CHeroStateScript {                             // the hero-state script built from the saved state
public:
    CCharString Text;                                                                  // +0x00
    CHeroStateScript(CCharString& state);                                              // 0x0083E1F0
    CCharString& GetText();                                                            // 0x0083E1B0
};

extern CCharString g_HeroThingName;                                                   // 0x013BAB08
extern bool        g_LoadingHeroState;                                                // 0x013B8A88

namespace std {

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const;
};

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                                                                // +0x00
    _Base_ptr _M_parent;                                                               // +0x04
    _Base_ptr _M_left;                                                                 // +0x08
    _Base_ptr _M_right;                                                                // +0x0c
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                                             // +0x10
};

template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() : _M_node(0) { }
};

template <class _Value, class _Alloc> class _Rb_tree_base {
public:
    typedef allocator<_Rb_tree_node<_Value> > _Node_alloc;
    _STLP_alloc_proxy<_Rb_tree_node<_Value>*, _Rb_tree_node<_Value>, _Node_alloc> _M_header;   // +0x00
    _Rb_tree_base(const _Alloc&) : _M_header(_Node_alloc(), (_Rb_tree_node<_Value>*)0)
    {
        _M_header._M_data = _M_header.allocate(1);
    }
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
};

template <class _Key, class _Value, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_iterator<_Value> iterator;
    unsigned int _M_node_count;                                                        // +0x04
    _Compare     _M_key_compare;                                                       // +0x08
    _Rb_tree(const _Compare& __comp, const _Alloc& __a)
        : _Rb_tree_base<_Value, _Alloc>(__a), _M_node_count(0), _M_key_compare(__comp)
    {
        _M_empty_initialize();
    }
    ~_Rb_tree() { clear(); }
    _Rb_tree_node_base*& _M_root() const { return this->_M_header._M_data->_M_parent; }
    _Rb_tree_node_base*& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    _Rb_tree_node_base*& _M_rightmost() const { return this->_M_header._M_data->_M_right; }
    void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = false;
        _M_root() = 0;
        _M_leftmost() = this->_M_header._M_data;
        _M_rightmost() = this->_M_header._M_data;
    }
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
    pair<iterator, bool> insert_unique(const _Value& __v);                             // 0x004AC380
    void _M_erase(_Link_type __x);                                                     // 0x004ABA70
};

template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set {
public:
    typedef _Rb_tree<_Key, _Key, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    set() : _M_t(_Compare(), _Alloc()) { }
    pair<iterator, bool> insert(const _Key& __x) { return _M_t.insert_unique(__x); }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                                                     // +0x00
    _Tp* _M_finish;                                                                    // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;                            // +0x08
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector() { }
    iterator begin() const { return this->_M_start; }
    iterator end() const { return this->_M_finish; }
};

}

struct C3DVector {
    float X, Y, Z;
};

class CTCPhysicsBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void SetPosition(const C3DVector& position);                               // slot 30 (+0x78)
};

struct CTCBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void CopyStateFrom(CTCBase* source);     // slot 12 (+0x30)
};
struct CTCHeroType3 : CTCBase {                      // interface type 3
    void ResetState();                               // 0x0071C260
};

struct CPresenceBits {                               // std::bitset<274> (nine words)
    unsigned long _M_w[9];
    bool operator[](unsigned int pos) const { return (_M_w[pos / 32] & (1UL << (pos % 32))) != 0; }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const long* type);            // 0x0040F020 (and its identical copy 0x004365B0)
};

class CThing {
public:
    unsigned char   _pad_0x00[0x10];
    unsigned char   ThingType;                                                         // +0x10 (2: a creature)
    unsigned char   _pad_0x11[0x20 - 0x11];
    CPresenceBits   m_present;                                                         // +0x20
    CInterfaceMap   m_interfaces;                                                      // +0x44
    unsigned char   _pad_0x4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                                                         // +0x60
    const C3DVector& GetPosition();                                                    // 0x004C73D0
    void CopyTCState(CTCBase* source, bool flag);                                      // 0x004C9F60
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present[wanted]) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* found = map->LowerBound(&type);
            CInterfaceMap::SEntry* it;
            if (found != map->last && !(wanted < found->type))
                it = found;
            else
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};

class CPlayer {
public:
    CThing* GetHeroCreature();                                                         // 0x00487DC0
};
class CPlayerManager {
public:
    CPlayer* GetMainPlayer();                                                          // 0x00449970
};
class CWorldMap {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual long GetLevelAt(const C3DVector& position);                                // slot 16 (+0x40)
};

class CASuspendableProcess;
long  LoadEntityInto(const CCharString& script, long index);                           // stand-in helper
void  ActivateThing(CThing* thing);                                                   // stand-in helper

class CThingManager {
public:
    unsigned char   _pad_0x00[0x28];
    CWorldMap*      WorldMap;                                                          // +0x28
    unsigned char   _pad_0x2c[0x30 - 0x2c];
    CPlayerManager* PlayerManager;                                                     // +0x30
    void LoadEntitiesFromScript(const CCharString& script, long mode, long level, const std::set<CCharString>& names,
                                CASuspendableProcess* process, long flags, std::vector<CThing*>& things);   // 0x00520D00
    void ActivateThings(const std::vector<CThing*>& things);                           // 0x0051E2F0
    void CopyHeroToHero(CThing& from, CThing& to, std::vector<CThing*>& things);       // 0x0051F0E0
    void LoadMainPlayerCreatureState(CCharString& state);
};

__declspec(noinline) void CThingManager::LoadEntitiesFromScript(const CCharString& script, long mode, long level,
    const std::set<CCharString>&, CASuspendableProcess*, long, std::vector<CThing*>& things)
{                                                   // 0x00520D00 (same-TU stand-in: writes the vector)
    things._M_finish = things._M_start + LoadEntityInto(script, mode + level);
}

__declspec(noinline) void CThingManager::ActivateThings(const std::vector<CThing*>& things)
{                                                   // 0x0051E2F0 (same-TU stand-in: only reads the vector)
    for (std::vector<CThing*>::iterator it = things.begin(); it != things.end(); ++it)
        ActivateThing(*it);
}

__declspec(noinline) void CThingManager::CopyHeroToHero(CThing& from, CThing& to, std::vector<CThing*>&)
{                                                   // 0x0051F0E0 (the real body, landed separately)
    CTCHeroType3* sourceType3;
    if (from.GetInterface(&sourceType3, 3)) {
        CTCHeroType3* targetType3;
        to.GetInterface(&targetType3, 3);
        targetType3->ResetState();
        to.CopyTCState(sourceType3, true);
    }
    CTCBase* source;
    if (from.GetInterface(&source, 4))
        to.CopyTCState(source, true);
    if (from.GetInterface(&source, 0x5e))
        to.CopyTCState(source, true);
    if (from.GetInterface(&source, 0x5c))
        to.CopyTCState(source, true);
    if (from.GetInterface(&source, 0x68))
        to.CopyTCState(source, true);
    if (from.GetInterface(&source, 0x6f)) {
        CTCBase* target;
        to.GetInterface(&target, 0x6f);
        target->CopyStateFrom(source);
    }
    if (from.GetInterface(&source, 0x14)) {
        CTCBase* target;
        to.GetInterface(&target, 0x14);
        target->CopyStateFrom(source);
    }
}

void CThingManager::LoadMainPlayerCreatureState(CCharString& state)
{
    CHeroStateScript script(state);
    const CCharString& text = script.GetText();
    std::set<CCharString> names;
    {
        CCharString heroName(g_HeroThingName);
        names.insert(heroName);
    }
    std::vector<CThing*> things;
    C3DVector heroPosition = PlayerManager->GetMainPlayer()->GetHeroCreature()->GetPosition();
    long level = WorldMap->GetLevelAt(heroPosition);
    g_LoadingHeroState = true;
    LoadEntitiesFromScript(text, 2, level, names, 0, 0, things);
    CThing* loaded = 0;
    CThing* hero = PlayerManager->GetMainPlayer()->GetHeroCreature();
    for (std::vector<CThing*>::iterator it = things.begin(); it != things.end(); ++it) {
        CThing* thing = *it;
        if (thing->ThingType == 2) {
            loaded = thing;
            break;
        }
    }
    loaded->PhysicsTC->SetPosition(hero->GetPosition());
    ActivateThings(things);
    CopyHeroToHero(*loaded, *hero, things);
    g_LoadingHeroState = false;
}
