// QuadricOptimiserInternals::CUniqueVertex::CUniqueVertex() -- default constructor of the 0x1C-byte unique-vertex record: two empty STLport set<unsigned long> index lists
// Leaf: ??0CUniqueVertex@QuadricOptimiserInternals
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD4C40, 0x56 bytes (+ int3 padding), thiscall, returns this.
// Identity: Ego_r.pdb QuadricOptimiserInternals::CUniqueVertex { bool Exists +0; CIndexList VertexIndices +4;
// CIndexList FaceList } with CIndexList { set<unsigned long> Indices } -- retail STLport sets are 12 bytes, so
// FaceList sits at +0x10 (PDB +0x14) and the record is 0x1C. Its destructor is 0x00AD4A70 (landed under the
// descriptive name CTwoLongSets; COptimisedPrimitive's destructor 0x00AD84D0 identifies it as ~CUniqueVertex),
// its copy constructor 0x00AD5C70 (Exists byte + two set copies 0x00AD3A10). The only caller (0x00AD8310) builds
// a temporary in its frame, hands it to 0x00AD8090 and destroys it. Catalog "Intrusive_List_Pair_Construct" is
// descriptive.
// Callees: malloc @ 0x00BFEA0E twice (STLport allocator<_Node>::allocate(1)); 0x14-byte node: colour +0,
// parent +4, left +8, right +0xC, value +0x10.
// Shape: Exists is left uninitialised; both STLport `set() : _M_t(_Compare(), allocator_type())` constructors
// inline: `_Rb_tree_base(a)` zeroes the header proxy before the allocation and assigns it after,
// `_M_node_count(0)`, then `_M_empty_initialize()` (red colour byte, no parent, left/right at the header)
// re-reading the header member at each store; one shared zero register (ebx) and a single `add esp,8`.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E

namespace std {
template <class T> class allocator {
public:
    allocator() { }
    template <class U> allocator(const allocator<U>&) { }
    T* allocate(unsigned int n) { return (T*)malloc(n * sizeof(T)); }
};
template <class T> class less {
public:
    bool operator()(const T& a, const T& b) const { return a < b; }
};
template <class V, class Tp, class A> class _STLP_alloc_proxy : public A {
public:
    V _M_data;
    _STLP_alloc_proxy(const A& a, V p) : A(a), _M_data(p) { }
};
struct _Rb_tree_node_base {
    bool                _M_color;                           // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0C
};
template <class V> struct _Rb_tree_node : public _Rb_tree_node_base {
    V _M_value_field;                                       // +0x10
};
template <class V, class A> class _Rb_tree_base {
public:
    typedef _Rb_tree_node<V> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
    _Rb_tree_base(const A& a) : _M_header(allocator<_Node>(a), (_Node*)0)
    {
        _M_header._M_data = _M_header.allocate(1);
    }
};
template <class T> struct _Identity { };
template <class K, class V, class KeyOfValue, class Compare, class A> class _Rb_tree : public _Rb_tree_base<V, A> {
public:
    unsigned int _M_node_count;
    Compare      _M_key_compare;
    _Rb_tree(const Compare& comp, const A& a) : _Rb_tree_base<V, A>(a), _M_node_count(0), _M_key_compare(comp)
    {
        _M_empty_initialize();
    }
    void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = false;          // red
        this->_M_header._M_data->_M_parent = 0;
        this->_M_header._M_data->_M_left = this->_M_header._M_data;
        this->_M_header._M_data->_M_right = this->_M_header._M_data;
    }
};
template <class K, class C = less<K>, class A = allocator<K> > class set {
public:
    typedef _Rb_tree<K, K, _Identity<K>, C, A> _Rep_type;
    _Rep_type _M_t;
    set() : _M_t(C(), A()) { }
};
} // namespace std

namespace QuadricOptimiserInternals {

class CIndexList {
public:
    std::set<unsigned long> Indices;                       // +0x00 (12 bytes in retail)
};

class CUniqueVertex {
public:
    bool       Exists;                                      // +0x00 (not initialised by the constructor)
    CIndexList VertexIndices;                               // +0x04
    CIndexList FaceList;                                    // +0x10
    CUniqueVertex();
};

CUniqueVertex::CUniqueVertex()
{
}

} // namespace QuadricOptimiserInternals