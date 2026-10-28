// CVertexGroupBlock::CVertexGroupBlock() -- default constructor of the 0x18-byte vertex group block: an empty group vector and an empty group -> index map
// Leaf: ??0CVertexGroupBlock@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD9030, 0x3A bytes, thiscall, returns this.
// Identity: Ego_r.pdb ??0CVertexGroupBlock@@QAE@XZ; retail layout as the landed CVertexGroupBlock::IsSubsetOf 0x00AD8C30
// (Groups CArray<unsigned long> = STLport vector +0x00, GroupToIndexMap map<unsigned long, long> +0x0C with a 0x18-byte
// node). Callers: three in the 0x00AA5xxx/0x00AA6xxx mesh code and CVertexGroupBlockOptimiser::AddBlocks 0x00AD9F10.
// Catalog "Vector_Construct_Empty" is descriptive.
// Callee: malloc @ 0x00BFEA0E (the map's header node).
// Shape: STLport `vector() : _Vector_base(allocator_type())` (three zero words) then the map's
// `_Rb_tree(_Compare(), allocator_type())` (header proxy zeroed, allocated, count zeroed, red colour, no parent,
// left/right at the header, re-read per store); one shared zero register.
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
template <class T> class _Vector_base {
public:
    T*                                       _M_start;          // +0
    T*                                       _M_finish;         // +4
    _STLP_alloc_proxy<T*, T, allocator<T> >  _M_end_of_storage; // +8
    _Vector_base(const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0) { }
};
template <class T> class vector : public _Vector_base<T> {
public:
    vector() : _Vector_base<T>(allocator<T>()) { }
};
} // namespace std

class CVertexGroupBlock {
public:
    std::vector<unsigned long>    Groups;                  // +0x00
    std::map<unsigned long, long> GroupToIndexMap;         // +0x0C
    CVertexGroupBlock();
};

CVertexGroupBlock::CVertexGroupBlock()
{
}