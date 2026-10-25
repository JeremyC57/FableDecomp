// _STL::vector::resize(unsigned int new_size) -- STLport vector<set<unsigned long> >::resize(n): build an empty set, resize the vector with it as the fill value (or truncate), then destroy the temporary set
// Leaf: ?resize@?$vector@V?$set@KV?$less@K@_STL@@V?$allocator@K@2@@_STL@@V?$allocator@V?$set@KV?$less@K@_STL@@V?$allocator@K@2@@_STL@@@2@@_STL@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0051D240, 144 bytes, `ret 4` (__fastcall: this in ecx, new_size on the stack).
// Caller: 0x005073E2.
// Callees: malloc 0x00BFEA0E (the temporary set's 20-byte header node), vector::resize(n, const set&) 0x0051D030 (the
//   manifest's "vector<CUIMiniMapMarker>::resize" label there is propagated), _Rb_tree::_M_erase 0x004A9CB0 (the
//   manifest's "LTextTreeWalkThrough::BuildTreeArray" label is propagated), free 0x00BFEA14.
// Identity: STLport 4.x `void resize(size_type __new_size) { resize(__new_size, _Tp()); }` for _Tp = set<unsigned long>
//   (the 12-byte set: alloc-proxied header pointer, node count, empty less<>). The catalog's
//   "std::vector_Construct_FromTreeTraversal" is descriptive.
// Shape: the whole temporary set is inline: _Rb_tree_base's constructor (`push 14h ; call malloc`), _M_empty_initialize
//   (red header, null parent, left = right = header), then on the way out ~_Rb_tree's clear() (only when the count is
//   non-zero: `_M_erase(root)` and the header reset) and ~_Rb_tree_base's deallocate (`free` when the header is
//   non-null). The model follows the landed _Rb_tree of 0x00AD9120.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

namespace _STL {
template <class T> class allocator {
public:
    allocator() { }
    template <class U> allocator(const allocator<U>&) { }
    T* allocate(unsigned int n) { return (T*)malloc(n * sizeof(T)); }
    void deallocate(T* p, unsigned int) { if (p != 0) free(p); }
};
template <class T> class less {
public:
    bool operator()(const T& a, const T& b) const { return a < b; }
};
template <class T1, class T2> struct pair {             // STLport pair is a struct
    T1 first;
    T2 second;
    pair() { }
    pair(const T1& a, const T2& b) : first(a), second(b) { }
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
template <class V> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(const _Rb_tree_iterator& x) : _M_node(x._M_node) { }
};
template <class V, class A> class _Rb_tree_base {
public:
    typedef _Rb_tree_node<V> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
    _Rb_tree_base(const A& a) : _M_header(allocator<_Node>(a), (_Node*)0)
    {
        _M_header._M_data = _M_header.allocate(1);
    }
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
};
template <class T> struct _Identity { };
template <class K, class V, class KeyOfValue, class Compare, class A> class _Rb_tree : public _Rb_tree_base<V, A> {
public:
    typedef _Rb_tree_node<V> _Node;
    typedef _Rb_tree_iterator<V> iterator;
    unsigned int _M_node_count;
    Compare      _M_key_compare;
    _Rb_tree(const Compare& comp, const A& a) : _Rb_tree_base<V, A>(a), _M_node_count(0), _M_key_compare(comp)
    {
        _M_empty_initialize();
    }
    ~_Rb_tree() { clear(); }
    void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = false;          // red
        this->_M_header._M_data->_M_parent = 0;
        this->_M_header._M_data->_M_left = this->_M_header._M_data;
        this->_M_header._M_data->_M_right = this->_M_header._M_data;
    }
    void _M_erase(_Node* x);                                // 0x004A9CB0
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase((_Node*)this->_M_header._M_data->_M_parent);
            this->_M_header._M_data->_M_left = this->_M_header._M_data;
            this->_M_header._M_data->_M_parent = 0;
            this->_M_header._M_data->_M_right = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
    unsigned int size() const { return _M_node_count; }

};
template <class K, class C = less<K>, class A = allocator<K> > class set {
public:
    typedef _Rb_tree<K, K, _Identity<K>, C, A> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    set() : _M_t(C(), A()) { }
    unsigned int size() const { return _M_t.size(); }

};

template <class _Tp> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _Tp* _M_end_of_storage;                                 // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp> {
public:
    void resize(unsigned int __new_size, const _Tp& __x);   // 0x0051D030
    void resize(unsigned int __new_size) { resize(__new_size, _Tp()); }
};

template void vector<set<unsigned long> >::resize(unsigned int);
} // namespace _STL