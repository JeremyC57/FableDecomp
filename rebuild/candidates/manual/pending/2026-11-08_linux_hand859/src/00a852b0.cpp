// CVectorMap::Insert(const unsigned __int64& key, const std::list<C2DLineF>& value) -- append a (key, line list) entry to the navigation layer's switchable line-list map (an unsorted pair vector) and mark it dirty
// Leaf: ?Insert@?$CVectorMap@_KV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@V?$CKeyPairCompareLess@_KV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@@@@@QAEXAB_KABV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A852B0, 141 bytes (+ int3 padding), thiscall `ret 8`.
// Identity: Ego_r struct_layouts CNavigationLayer::SwitchableLineLists = CVectorMap<unsigned __int64, list<C2DLineF> > (+32);
//   CVectorMap<K, V, C> = STLport vector<pair<K, V> > (12 bytes) + empty Compare +0x0C + bool Dirty +0x0D (GOTCHAS, 0x00A34950).
//   The catalog label "Vector_PushBack_ControlTreeNode_With_Sort" is descriptive (no tree node, no sort: Insert only appends and
//   sets Dirty; Sort() happens later). Ego_r.pdb is a Dinkumware build, so the STLport spellings follow the landed TU siblings.
// Callees: malloc 0x00BFEA0E (the temporary's 0x18-byte list sentinel), list<C2DLineF>::_M_insert_dispatch 0x005152C0 (landed:
//   `??$_M_insert_dispatch@U?$_List_iterator@VC2DLineF@@U?$_Const_traits@...`), the pair copy constructor 0x00A825D0 (landed
//   `??0?$pair@...`), vector<pair<...> >::_M_insert_overflow 0x00A84C20 (landed `?_M_insert_overflow@`), list<C2DLineF>::~list
//   0x005127A0 (landed `??1?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@QAE@XZ`).
// Shape (chunk 859): the stock STLport 4.6 text (`push_back(value_type(key, value)); Dirty = true;`, the pair ctor, the list copy
//   constructor `insert(begin(), x.begin(), x.end())`, the _List_base ctor / dtor, the proxy's `_M_data(p)` init) with the BODIES
//   of every out-of-line callee that receives the temporary's address defined in this TU, as retail's TU had them
//   (_M_insert_dispatch 0x005152C0, the pair copy constructor 0x00A825D0, _M_insert_overflow 0x00A84C20 and its
//   __uninitialized_copy / __uninitialized_fill_n / __destroy_aux helpers, ~list 0x005127A0). With the bodies visible VC7.1
//   treats the temporary as unaliased: the dead proxy zero store goes, the fresh sentinel's begin() folds to the sentinel,
//   x.begin() / x.end() are loaded before the sentinel store (landed sibling 0x00A84500 lesson: with the callees only
//   declared, VC7.1 treats the escaped temporary as aliased and keeps the store / reloads).
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

class C2DVector {
public:
    float X, Y;
};
class C2DLineF {                                        // 16 bytes
public:
    C2DVector Point1;
    C2DVector Point2;
};

namespace std {
struct __false_type { __false_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    template <class _Tp1> allocator(const allocator<_Tp1>&) { }
    _Tp* allocate(unsigned int n) { return n != 0 ? (_Tp*)malloc(n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* p, unsigned int) { if (p) free(p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& a, _Value p) : _MaybeReboundAlloc(a), _M_data(p) { }
};

// _list.h
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };   // 0x18 bytes for C2DLineF
template <class _Tp> struct _Nonconst_traits { typedef _Tp* pointer; typedef _Tp& reference; };
template <class _Tp> struct _Const_traits { typedef const _Tp* pointer; typedef const _Tp& reference; };
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* p) : _M_node(p) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator!=(const _List_iterator_base& y) const { return _M_node != y._M_node; }
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef typename _Traits::reference reference;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) { }
    _List_iterator(const iterator& x) : _List_iterator_base(x._M_node) { }   // the iterator's copy constructor / the converting one
    reference operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
};
template <class _Tp, class _Alloc> class _List_base {
public:
    _List_base(const _Alloc&) : _M_node(allocator<_List_node<_Tp> >(), 0)
    {
        _List_node<_Tp>* n = _M_node.allocate(1);
        n->_M_next = n;
        n->_M_prev = n;
        _M_node._M_data = n;
    }
    ~_List_base() { clear(); _M_node.deallocate(_M_node._M_data, 1); }
    _STLP_alloc_proxy<_List_node<_Tp>*, _List_node<_Tp>, allocator<_List_node<_Tp> > > _M_node;
    void clear()
    {
        _List_node<_Tp>* cur = (_List_node<_Tp>*)_M_node._M_data->_M_next;
        while (cur != _M_node._M_data) {
            _List_node<_Tp>* tmp = cur;
            cur = (_List_node<_Tp>*)cur->_M_next;
            tmp->_M_data.~_Tp();
            _M_node.deallocate(tmp, 1);
        }
        _M_node._M_data->_M_next = _M_node._M_data;
        _M_node._M_data->_M_prev = _M_node._M_data;
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;
    list(const list& x) : _List_base<_Tp, _Alloc>(x.get_allocator()) { insert(begin(), x.begin(), x.end()); }
    __declspec(noinline) ~list() { }                    // 0x005127A0 (same-TU body: the inlined ~_List_base)
    _Alloc get_allocator() const { return _Alloc(); }
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator(this->_M_node._M_data->_M_next); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
    template <class _InputIter>
    void insert(iterator position, _InputIter first, _InputIter last) { _M_insert_dispatch(position, first, last, __false_type()); }
    template <class _InputIter>
    __declspec(noinline) void _M_insert_dispatch(iterator position, _InputIter first, _InputIter last,
                                                 const __false_type&)   // 0x005152C0 (same-TU body)
    {
        for (; first != last; ++first) insert(position, *first);
    }
protected:
    _List_node<_Tp>* _M_create_node(const _Tp& x)
    {
        _List_node<_Tp>* p = this->_M_node.allocate(1);
        new ((void*)&p->_M_data) _Tp(x);
        return p;
    }
public:
    iterator insert(iterator position, const _Tp& x)
    {
        _List_node<_Tp>* tmp = _M_create_node(x);
        _List_node_base* n = position._M_node;
        _List_node_base* p = n->_M_prev;
        tmp->_M_next = n; tmp->_M_prev = p; p->_M_next = tmp; n->_M_prev = tmp;
        return iterator(tmp);
    }
};

// _pair.h
template <class _T1, class _T2> struct pair {
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x08 (+0x0C padding)
    pair(const _T1& a, const _T2& b) : first(a), second(b) { }
    pair(const pair& p);                                // 0x00A825D0 (same-TU body below)
};
template <class _T1, class _T2> __declspec(noinline) pair<_T1, _T2>::pair(const pair& p) : first(p.first), second(p.second) { }

// _construct.h / _uninitialized.h
template <class _T1, class _T2> inline void _Construct(_T1* p, const _T2& v) { new (p) _T1(v); }
template <class _Tp> inline void _Destroy(_Tp* p) { p->~_Tp(); }
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter first, _InputIter last, _ForwardIter result, const __false_type&)   // 0x00A83030
{
    _ForwardIter cur = result;
    for (; first != last; ++first, ++cur) _Construct(cur, *first);
    return cur;
}
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter first, _Size n, const _Tp& x, const __false_type&)   // 0x00A830A0
{
    _ForwardIter cur = first;
    for (; n > 0; --n, ++cur) _Construct(cur, x);
    return cur;
}
template <class _ForwardIter>
void __fastcall __destroy_aux(_ForwardIter first, _ForwardIter last, const __false_type&)   // 0x00A83420
{
    for (; first != last; ++first) _Destroy(&*first);
}
template <class _ForwardIter> inline void _Destroy(_ForwardIter first, _ForwardIter last) { __destroy_aux(first, last, __false_type()); }

// _vector.h / _vector.c
template <class _Tp, class _Alloc> class _Vector_base {
public:
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
protected:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp value_type;
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    void push_back(const _Tp& x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, x);
            ++this->_M_finish;
        } else
            _M_insert_overflow(this->_M_finish, x, __false_type(), 1UL, true);
    }
protected:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(_Tp* s, _Tp* f, _Tp* e) { this->_M_start = s; this->_M_finish = f; this->_M_end_of_storage._M_data = e; }
    void _M_insert_overflow(_Tp* position, const _Tp& x, const __false_type&, unsigned int fill_len, bool atend);   // 0x00A84C20
};
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* position, const _Tp& x, const __false_type&, unsigned int fill_len,
                                             bool atend)   // STLport 4.6 _vector.c (same-TU body, the landed 0x00A84C20)
{
    const unsigned int old_size = size();
    const unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
    _Tp* new_start = this->_M_end_of_storage.allocate(len);
    _Tp* new_finish = __uninitialized_copy(this->_M_start, position, new_start, __false_type());
    if (fill_len == 1) {
        _Construct(new_finish, x);
        ++new_finish;
    }
    else
        new_finish = __uninitialized_fill_n(new_finish, fill_len, x, __false_type());
    if (!atend)
        new_finish = __uninitialized_copy(position, this->_M_finish, new_finish, __false_type());
    _M_clear();
    _M_set(new_start, new_finish, new_start + len);
}
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

template <class K, class V, class C> class CVectorMap : public std::vector<std::pair<K, V> > {
public:
    typedef std::pair<K, V> value_type;
    C    Compare;                                       // +0x0c (empty)
    bool Dirty;                                         // +0x0d
    void Insert(const K& key, const V& value)
    {
        push_back(value_type(key, value));
        Dirty = true;
    }
};

template void CVectorMap<unsigned __int64, std::list<C2DLineF>, CKeyPairCompareLess<unsigned __int64, std::list<C2DLineF> > >::Insert(
    const unsigned __int64& key, const std::list<C2DLineF>& value);
