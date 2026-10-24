// CFaction::CFaction(const CCharString& name) -- a named faction with empty allied and enemy faction-name sets
// Leaf: ??0CFaction@@QAE@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BA3B0, 98 bytes (+ int3 padding), `ret 4` (thiscall: name; returns this).
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30, malloc @ 0x00BFEA0E (each set's 0x14-byte header
//   node: four link words + a CCharString value).
// Identity: Ego_r.pdb ??0CFaction@@QAE@ABVCCharString@@@Z.
// Layout: CFaction (Ego_r.pdb, no engine header) Name CCharString +0, AlliedFactions set<CCharString> +4, EnemyFactions
//   +0x10 -- the retail STLport set is 12 bytes (_M_header +0, _M_node_count +4, the empty less<> +8), as the landed
//   SetAlliedToFaction 0x006B94E0 / IsAlliedWithFaction 0x006B9460.
// Shape: `Name(name)` then two default STLport sets: `_Rb_tree_base` zeroes the proxy word and allocates the header,
//   `_Rb_tree` zeroes the count after the call and `_M_empty_initialize()` writes red colour, null root, leftmost and
//   rightmost = header, re-reading the header pointer each time (the landed CBulletTimeManager ctor 0x004C60F0 model);
//   the two malloc pushes are popped together (`add esp,8`).
// Label correction: the catalog `CFaction_InitInternalLists` is descriptive.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E

class CCharString {
public:
    void* PStringData;                                  // +0x00
    CCharString(const CCharString& other);              // 0x0099EC30
};

namespace std {
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

template <class K, class C = less<K>, class A = allocator<K> > class set {
public:
    _Rb_tree<K, K, C, A> _M_t;
    __forceinline set() : _M_t(C()) {}
};
}

class CFaction {
public:
    CCharString                Name;                    // +0x00
    std::set<CCharString>      AlliedFactions;          // +0x04
    std::set<CCharString>      EnemyFactions;           // +0x10
    CFaction(const CCharString& name);
};

CFaction::CFaction(const CCharString& name)
    : Name(name)
{
}