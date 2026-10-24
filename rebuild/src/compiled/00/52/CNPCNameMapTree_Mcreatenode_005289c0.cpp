// CNPCNameMapTree::_M_create_node(const value_type& x) -- STLport _Rb_tree::_M_create_node of CThingManager::NPCNameMap: malloc a 0x20-byte node and copy-construct the (UID, (name, display name)) pair into it
// Leaf: ?_M_create_node
//
// Retail 0x005289C0, 70 bytes, `ret 4`, node pointer in eax.  Callees: 0x00BFEA0E malloc (the node allocator),
//   0x0099EC30 CCharString::CCharString(const CCharString&), 0x0099B720 CWideString::CWideString(const CWideString&).
// Caller: the map's _M_insert 0x005293D0.
// Layout: Ego_r.pdb CThingManager::NPCNameMap +0xF4 = map<unsigned __int64, pair<CCharString, CWideString> >; node
//   { color +0, parent +4, left +8, right +0xC, value +0x10 } = 0x20 bytes: UID +0x10, CCharString +0x18, CWideString +0x1C.
// Shape: `tmp = allocate(1); _Construct(&tmp->_M_value_field, x); return tmp;` -- the TU has no /EHsc, so the
//   _STLP_TRY/_STLP_UNWIND deallocate is gone; the placement new keeps its null test (`test eax,eax / je`) on node+0x10.
//   `this` is never read (the allocator is stateless).
extern "C" void* __cdecl malloc(unsigned int size);
inline void* __cdecl operator new(unsigned int, void* p) throw() { return p; }

class CCharString {
public:
    const char* PString;
    CCharString(const CCharString& other);     // 0x0099EC30
};
class CWideString {
public:
    const unsigned short* PString;
    CWideString(const CWideString& other);     // 0x0099B720
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair<T1, T2>& o) : first(o.first), second(o.second) {}
};
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
}

struct _Rb_tree_node_base {
    char                _M_color;              // +0x0
    _Rb_tree_node_base* _M_parent;             // +0x4
    _Rb_tree_node_base* _M_left;               // +0x8
    _Rb_tree_node_base* _M_right;              // +0xC
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                          // +0x10
};

class CNPCNameMapTree {                        // STLport _Rb_tree<unsigned __int64, pair<const unsigned __int64, pair<CCharString, CWideString> >, ...>
public:
    typedef std::pair<const unsigned __int64, std::pair<CCharString, CWideString> > value_type;
    typedef _Rb_tree_node<value_type> _Node;
    _Node*        _M_header;                   // +0x0
    unsigned long _M_node_count;               // +0x4

    static _Node* allocate(unsigned int n) { return (_Node*)malloc(n * sizeof(_Node)); }
    _Node* _M_create_node(const value_type& x);
};

CNPCNameMapTree::_Node* CNPCNameMapTree::_M_create_node(const value_type& x)
{
    _Node* tmp = allocate(1);
    std::_Construct(&tmp->_M_value_field, x);
    return tmp;
}