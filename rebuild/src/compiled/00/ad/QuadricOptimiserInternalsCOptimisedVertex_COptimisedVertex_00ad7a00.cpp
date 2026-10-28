// QuadricOptimiserInternals::COptimisedVertex::COptimisedVertex() -- default constructor of the 0xB8-byte optimiser vertex: empty face list, Exists = true, empty group and pair-contraction maps
// Leaf: ??0COptimisedVertex@QuadricOptimiserInternals@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD7A00, 0xB7 bytes, thiscall, returns this.
// Identity: Ego_r.pdb ??0COptimisedVertex@QuadricOptimiserInternals@@QAE@XZ; the members line up with the landed
// ~COptimisedVertex 0x00AD7400: FaceList's set<unsigned long> +0x84 (0x14-byte node), Exists +0x90, Groups
// map<unsigned long, float> +0x9C (0x18-byte node), PairContractions map<unsigned long, CContractionReference>
// +0xA8 (0x20-byte node). Callers 0x00ACDC53 and 0x00AD883D (vector<COptimisedVertex>::resize's default value).
// Catalog "COptimisedPrimitive_ConstructorBody" is descriptive.
// Callees: malloc @ 0x00BFEA0E (three header nodes: STLport allocator<_Node>::allocate(1)).
// Shape: the vectors/quadric before +0x84 have no constructor code (PODs); the three STLport tree default constructors
// inline in member order (header proxy zeroed, allocated, count zeroed, red colour, no parent, left/right at the
// header -- re-read per store) with `Exists(true)` as an initialiser between FaceList and Groups; UniqueIndex,
// OutputIndex and Cost stay uninitialised. One shared zero register (ebx), one `add esp,0Ch`.
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
    private:
    void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = false;          // red
        this->_M_header._M_data->_M_parent = 0;
        this->_M_header._M_data->_M_left = this->_M_header._M_data;
        this->_M_header._M_data->_M_right = this->_M_header._M_data;
    }
    public:
};
template <class K, class C = less<K>, class A = allocator<K> > class set {
public:
    typedef _Rb_tree<K, K, _Identity<K>, C, A> _Rep_type;
    _Rep_type _M_t;
    set() : _M_t(C(), A()) { }
};
template <class T1, class T2> class pair {
public:
    T1 first;
    T2 second;
};
template <class P> struct _Select1st { };
template <class K, class T, class C = less<K>, class A = allocator<pair<const K, T> > > class map {
public:
    typedef pair<const K, T> value_type;
    typedef _Rb_tree<K, value_type, _Select1st<value_type>, C, A> _Rep_type;
    _Rep_type _M_t;
    map() : _M_t(C(), A()) { }
};
} // namespace std

namespace QuadricOptimiserInternals {

class CContractionReference {                               // 12 bytes: CCountedPointer + SubIndex
public:
    void*         ContractionData;
    void*         ContractionInfo;
    unsigned long SubIndex;
};

class CIndexList {
public:
    std::set<unsigned long> Indices;                       // +0x00 (12 bytes in retail)
};

class COptimisedVertex {
public:
    unsigned char Position_to_Cost[0x84];                   // +0x00 Position, Normal, TextureCoord, PositionError, Cost (PODs)
    CIndexList    FaceList;                                 // +0x84
    bool          Exists;                                   // +0x90
    unsigned long UniqueIndex;                              // +0x94
    unsigned long OutputIndex;                              // +0x98
    std::map<unsigned long, float>                 Groups;            // +0x9C
    std::map<unsigned long, CContractionReference> PairContractions;  // +0xA8
    COptimisedVertex();
};

COptimisedVertex::COptimisedVertex()
    : Exists(true)
{
}

} // namespace QuadricOptimiserInternals