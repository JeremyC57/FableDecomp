// std::list<CVertexGroupBlock>::operator=(const list& x) -- copy-assign the optimiser's block list: assign over the existing blocks pairwise, then erase the surplus or append copies of the rest
// Leaf: ??4?$list@VCVertexGroupBlock
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADD020, 0xAB bytes, thiscall, `ret 4`, returns this.
// Identity: list nodes {next, prev, CVertexGroupBlock}; element assignment is CVertexGroupBlock's implicit operator=
// (vector<unsigned long>::operator= 0x0045BC09 on +8, the tree operator= 0x0045C79E on +0x14); the tail helpers are
// this chunk's list::erase(first, last) 0x00ADC4E0 and _M_insert_dispatch 0x00ADCC80: Ego_r.pdb
// list<CVertexGroupBlock> (CVertexGroupBlockOptimiser::Blocks). Catalog "List_CopyAssign" is descriptive.
// Callees: vector<unsigned long>::operator= @ 0x0045BC09, _Rb_tree<unsigned long, pair<const unsigned long, long>,
// ...>::operator= @ 0x0045C79E, list::erase(iterator, iterator) @ 0x00ADC4E0, list::_M_insert_dispatch @ 0x00ADCC80.
// Shape: STLport 4.x `if (this != &x) { first1 = begin(); last1 = end(); first2 = x.begin(); last2 = x.end();
// while (first1 != last1 && first2 != last2) *first1++ = *first2++; if (first2 == last2) erase(first1, last1); else
// insert(last1, first2, last2); } return *this;` -- `this` and `last2` live in the frame words, `last1` in the dead `x`
// slot (which also takes the erase result and the insert's __false_type tag; the insert callee's same-TU body keeps
// that tag homing).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6

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

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
        : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) + ((const char*)__last - (const char*)__first);
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
    vector(const vector& x)
        : _Vector_base<T>(x.size(), x.get_allocator())
    {
        this->_M_finish = (T*)__copy_trivial(x._M_start, x._M_finish, this->_M_start);
    }
    vector& operator=(const vector& x);                     // 0x0045BC09 (vector<unsigned long>)
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
    _Rb_tree& operator=(const _Rb_tree& __x);               // 0x0045C79E
};

template <class K, class T, class Compare = less<K> > class map {
public:
    typedef pair<const K, T> value_type;
    _Rb_tree<K, value_type, Compare> _M_t;
    map(const map& __x) : _M_t(__x._M_t) { }
    map& operator=(const map& __x) { _M_t = __x._M_t; return *this; }
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

// STLport 4.x: the one converting constructor from `iterator` is the user copy constructor of `iterator` (built in the
// argument slot) but leaves `const_iterator` with an implicit copy (pushed straight from its register).
template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; };
template <class _Tp> struct _Const_traits { typedef const _Tp& reference; };
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef typename _Traits::reference _Ref;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }
    _Ref operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
    _List_iterator operator++(int)
    {
        _List_iterator __tmp = *this;
        this->_M_incr();
        return __tmp;
    }
};

template <class _Tp> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node> _M_node;                       // sentinel
};

template <class _Tp> class list : public _List_base<_Tp> {
public:
    typedef _List_node<_Tp>                    _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> >    const_iterator;

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
    template <class _InputIter>
    void _M_insert_dispatch(iterator __pos, _InputIter __first, _InputIter __last, const __false_type&);   // 0x00ADCC80
    template <class _InputIter>
    void insert(iterator __pos, _InputIter __first, _InputIter __last)
    {
        _M_insert_dispatch(__pos, __first, __last, __false_type());
    }
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
    const_iterator begin() const { return const_iterator(this->_M_node._M_data->_M_next); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
    iterator erase(iterator __first, iterator __last);                                                     // 0x00ADC4E0
    list& operator=(const list& __x);
};

#pragma auto_inline(off)
// Same-TU stand-in for the out-of-line range insert (retail 0x00ADCC80): it never touches its tag, which lets VC7.1
// home the caller's empty tag on the `__last1` slot as retail does.
template <class _Tp>
template <class _InputIter>
__declspec(noinline) void list<_Tp>::_M_insert_dispatch(iterator __pos, _InputIter __first, _InputIter __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        insert(__pos, *__first);
}
#pragma auto_inline(on)

template <class _Tp>
list<_Tp>& list<_Tp>::operator=(const list<_Tp>& __x)
{
    if (this != &__x) {
        iterator __first1 = begin();
        iterator __last1 = end();
        const_iterator __first2 = __x.begin();
        const_iterator __last2 = __x.end();
        while (__first1 != __last1 && __first2 != __last2)
            *__first1++ = *__first2++;
        if (__first2 == __last2)
            erase(__first1, __last1);
        else
            insert(__last1, __first2, __last2);
    }
    return *this;
}

template list<CVertexGroupBlock>& list<CVertexGroupBlock>::operator=(const list<CVertexGroupBlock>&);
} // namespace std