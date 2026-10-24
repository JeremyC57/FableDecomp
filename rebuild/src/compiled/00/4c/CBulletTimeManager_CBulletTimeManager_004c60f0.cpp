// CBulletTimeManager::CBulletTimeManager(CWorld& world, CPlayerManager& playerManager) -- construct the bullet-time manager: an empty STLport map of bullet-time entries (0x1c-byte header node), no override, the default multiplier on, no strike tick, the world and player manager remembered
// Leaf: ??0CBulletTimeManager@@QAE@AAVCWorld@@AAVCPlayerManager@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C60F0, length 80 bytes (71 + int3 padding), `ret 8` (thiscall: world, playerManager).
// Identity: FableWin.pdb / Ego_r.pdb `??0CBulletTimeManager@@QAE@AAVCWorld@@AAVCPlayerManager@@@Z`; the single caller
// 0x004A73AB is in CWorld (the PBulletTimeManager scoped_ptr at CWorld +0x68, per the landed PrepareForLoad 0x004C6080).
// The catalog "LTextBinTree_ctor" is a propagated label.
// Callees: malloc @ 0x00BFEA0E (the map's header node: 0x10 links + void* key + 8-byte CBulletTimeInfo = 0x1c).
// Layout: Ego_r.pdb CBulletTimeManager with the 12-byte retail STLport map (as the landed PrepareForLoad 0x004C6080):
// BulletTimeInfoMap +0 (header pointer, count +4, compare +8), Override +0x0C, UseDefaultMultiplier +0x0D,
// LastStrikeActionTick +0x10, World +0x14, PlayerManager +0x18. The engine header puts the map at 16 bytes (Ego_r's
// Dinkumware size), so a local model is used.
// Shape: STLport 4.x `_Rb_tree(const _Compare&)` over `_Rb_tree_base`: the alloc proxy's data zeroed and the header
// allocated in the base constructor, the count zeroed by the derived init list (after the call, as retail),
// then `_M_empty_initialize()` (red colour, null root, leftmost/rightmost = header, each re-reading the header pointer);
// the members follow in declaration order with the zero register shared.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};

struct _Rb_tree_node_base {
    bool                _M_color;                       // +0x00
    _Rb_tree_node_base* _M_parent;                      // +0x04
    _Rb_tree_node_base* _M_left;                        // +0x08
    _Rb_tree_node_base* _M_right;                       // +0x0c
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                   // +0x10
};

template <class T> struct allocator {
    __forceinline T* allocate(unsigned int n) { return static_cast<T*>(malloc(n * sizeof(T))); }
};
template <class T> struct less {};

template <class T, class A> struct _STLP_alloc_proxy : A {
    T _M_data;
    __forceinline _STLP_alloc_proxy(const A&, T p) : _M_data(p) {}
};

template <class V, class A> struct _Rb_tree_base {
    typedef _Rb_tree_node<V> _Node;
    _STLP_alloc_proxy<_Rb_tree_node_base*, allocator<_Node> > _M_header;   // +0x00
    __forceinline _Rb_tree_base(const A&) : _M_header(allocator<_Node>(), 0) { _M_header._M_data = _M_header.allocate(1); }
};

template <class K, class V, class C, class A> struct _Rb_tree : _Rb_tree_base<V, A> {
    unsigned int _M_node_count;                                             // +0x04
    C            _M_key_compare;                                            // +0x08
    __forceinline _Rb_tree(const C& comp) : _Rb_tree_base<V, A>(A()), _M_node_count(0), _M_key_compare(comp)
    {
        _M_empty_initialize();
    }
    __forceinline void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = false;      // _S_rb_tree_red
        this->_M_header._M_data->_M_parent = 0;
        this->_M_header._M_data->_M_left = this->_M_header._M_data;
        this->_M_header._M_data->_M_right = this->_M_header._M_data;
    }
};

template <class K, class T, class C = less<K>, class A = allocator<pair<K const, T> > > class map {
public:
    typedef pair<K const, T> value_type;
    _Rb_tree<K, value_type, C, A> _M_t;
    __forceinline map() : _M_t(C()) {}
};
}

class CWorld;
class CPlayerManager;

class CBulletTimeManager {
public:
    class CBulletTimeInfo {                             // Ego_r.pdb, 8 bytes
    public:
        float Multiplier;                               // +0x0
        float SoundPitch;                               // +0x4
    };
    CBulletTimeManager(CWorld& world, CPlayerManager& playerManager);
    std::map<void*, CBulletTimeInfo> BulletTimeInfoMap; // +0x00
    bool            Override;                           // +0x0c
    bool            UseDefaultMultiplier;               // +0x0d
    long            LastStrikeActionTick;               // +0x10
    CWorld*         World;                              // +0x14
    CPlayerManager* PlayerManager;                      // +0x18
};

CBulletTimeManager::CBulletTimeManager(CWorld& world, CPlayerManager& playerManager)
    : Override(false), UseDefaultMultiplier(true), LastStrikeActionTick(0), World(&world), PlayerManager(&playerManager)
{
}