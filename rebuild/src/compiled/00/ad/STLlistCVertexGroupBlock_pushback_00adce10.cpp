// std::list<CVertexGroupBlock>::push_back(const CVertexGroupBlock& x) -- append a copy of a block to the optimiser's block list
// Leaf: ?push_back@?$list@VCVertexGroupBlock
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADCE10, 0x45 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity: 0x20-byte list node {next, prev, CVertexGroupBlock}; the element copy is CVertexGroupBlock's implicit copy
// constructor -- vector<unsigned long>(const vector&) @ 0x00535470 then the tree copy 0x00A96E40 on +0xC: Ego_r.pdb
// list<CVertexGroupBlock>::push_back (CVertexGroupBlockOptimiser::Blocks). Catalog "List_EmplaceBack_Pair" is
// descriptive.
// Callees: malloc @ 0x00BFEA0E (allocate(1)), vector<unsigned long>::vector(const vector&) @ 0x00535470,
// _Rb_tree<unsigned long, pair<const unsigned long, long>, ...>::_Rb_tree(const _Rb_tree&) @ 0x00A96E40.
// Shape: STLport 4.x `push_back(x) { insert(end(), x); }` with insert and _M_create_node inline (placement-new null test
// around the element copy, then tmp->next = n, tmp->prev = p, p->next = tmp, n->prev = tmp). Unlike the range insert
// 0x00ADCC80, this TU calls the vector copy constructor out of line.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E

inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

namespace std {

struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
_ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class T> class allocator {                        // STLport allocator<T>: empty
public:
    allocator() { }
    allocator(const allocator&) { }
    T* allocate(unsigned int n) { return n != 0 ? static_cast<T*>(malloc(n * sizeof(T))) : 0; }
};

template <class T> class _STLP_alloc_proxy : public allocator<T> {
public:
    T* _M_data;
    _STLP_alloc_proxy(const allocator<T>& a, T* p) : allocator<T>(a), _M_data(p) { }
};

template <class T> class _Vector_base {
public:
    T*                    _M_start;                         // +0
    T*                    _M_finish;                        // +4
    _STLP_alloc_proxy<T>  _M_end_of_storage;                // +8
    _Vector_base(unsigned int n, const allocator<T>& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0)
    {
        _M_start = _M_end_of_storage.allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
    }
};

template <class T> class vector : public _Vector_base<T> {  // used here for a POD element only (__true_type copy)
public:
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    allocator<T> get_allocator() const { return allocator<T>(); }
    vector(const vector& x);                                // 0x00535470 (vector<unsigned long>, out of line here)
};

template <class T> struct less {
    bool operator()(const T& a, const T& b) const { return a < b; }
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
struct _Rb_tree_node_base;

template <class K, class V, class Compare> class _Rb_tree {
public:
    _Rb_tree_node_base* _M_header;                          // +0x00 (alloc proxy)
    unsigned int        _M_node_count;                      // +0x04
    Compare             _M_key_compare;                     // +0x08
    _Rb_tree(const _Rb_tree& __x);                          // 0x00A96E40
};

template <class K, class T, class Compare = less<K> > class map {
public:
    typedef pair<const K, T> value_type;
    _Rb_tree<K, value_type, Compare> _M_t;
    map(const map& __x) : _M_t(__x._M_t) { }
};

} // namespace std

class CVertexGroupBlock {                                   // Ego_r.pdb, 0x18 bytes in retail
public:
    std::vector<unsigned long>    Groups;                  // +0x00
    std::map<unsigned long, long> GroupToIndexMap;         // +0x0C
};

namespace std {

struct _List_node_base {
    _List_node_base* _M_next;                               // +0x0
    _List_node_base* _M_prev;                               // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                            // +0x8
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp, class _Ref> struct _List_iterator : public _List_iterator_base {
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const _List_iterator& __x) : _List_iterator_base(__x._M_node) { }
    _Ref operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
};

template <class _Tp> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node> _M_node;                       // sentinel
};

template <class _Tp> class list : public _List_base<_Tp> {
public:
    typedef _List_node<_Tp>                    _Node;
    typedef _List_iterator<_Tp, _Tp&>          iterator;
    typedef _List_iterator<_Tp, const _Tp&>    const_iterator;

    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = this->_M_node.allocate(1);
        _Construct(&__p->_M_data, __x);
        return __p;
    }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return __tmp;
    }
    iterator end() { return iterator(this->_M_node._M_data); }
    void push_back(const _Tp& __x) { insert(end(), __x); }
};

template void list<CVertexGroupBlock>::push_back(const CVertexGroupBlock&);
} // namespace std