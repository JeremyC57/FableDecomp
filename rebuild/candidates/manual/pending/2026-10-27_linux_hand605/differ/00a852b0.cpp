// CVectorMap::Insert(const unsigned __int64& key, const list<C2DLineF>& value) -- append a (key, line list) entry to the navigation map's unsorted vector and mark it dirty
// Grade: DIFFER(143v143) 4 lines + 5 call relocs (unchanged in chunk 605): retail loads x.begin() (`mov edx,[ecx]`) right after the x sentinel and sinks the temporary's sentinel store `temp.second._M_node._M_data = n` to just before the _M_insert_dispatch call; ours stores it after pushing `last` and loads x.begin() after it. Tried in chunk 605: STLport's direct `_M_node._M_data = allocate(1)` base-ctor form (172), sentinel store before the self-links (168), declaration-only _M_insert_dispatch (tag slot moves, 10), a member-template insert (same), a named __false_type tag (same).
// Leaf: ?Insert@?$CVectorMap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A852B0, 141 bytes (+ int3 padding), thiscall `ret 8`.
// Identity: Ego_r.pdb `?Insert@?$CVectorMap@_KV?$list@VC2DLineF@@...@@QAEXAB_KABV?$list@VC2DLineF@@...@@@Z` (CVectorMap<
//   unsigned __int64, list<C2DLineF>, CKeyPairCompareLess<...> >::Insert); an unreferenced COMDAT copy (its callers inline
//   it). The catalog label "Vector_PushBack_ControlTreeNode_With_Sort" is descriptive.
// Callees: malloc 0x00BFEA0E (the temporary's list sentinel), list::_M_insert_dispatch 0x005152C0, the pair copy
//   constructor 0x00A825D0, vector::_M_insert_overflow 0x00A84C20, list::~list 0x005127A0.
// Layout: CVectorMap = STLport vector<pair<K, V> > (12 bytes) + empty Compare +0x0C + Dirty +0x0D (GOTCHAS, 0x00A34950).
// Shape: `push_back(pair<K, V>(key, value)); Dirty = true;` -- the pair temporary in a 16-byte frame (key dwords, then the
//   STLport list copy constructor inline: the local's dead proxy zero store dropped, `begin()` of the fresh self-linked
//   sentinel folded to the sentinel, the empty tag homed in the dead `key` slot through the same-TU _M_insert_dispatch
//   stand-in), STLport push_back (placement copy through the out-of-line pair copy constructor, or _M_insert_overflow with
//   the __false_type tag in the dead key slot), the temporary's list destroyed, then Dirty set.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

class C2DLineF {                                        // 16 bytes (start, end)
public:
    float X0, Y0, X1, Y1;
};

namespace std {
struct __false_type { __false_type() { } };

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : _List_node_base {   // 0x18 bytes for C2DLineF
    _Tp _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* x) : _M_node(x) { }
};
template <class _Tp> struct _List_iterator : _List_iterator_base {
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) { }
    _List_iterator(const _List_iterator& x) : _List_iterator_base(x._M_node) { }   // copy constructor
};
template <class _Tp> struct _List_const_iterator : _List_iterator_base {
    _List_const_iterator(_List_node_base* x) : _List_iterator_base(x) { }
    _List_const_iterator(const _List_iterator<_Tp>& x) : _List_iterator_base(x._M_node) { }   // converting, not a copy
};
template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int n) { return n != 0 ? (_Tp*)malloc(n * sizeof(_Tp)) : 0; }
};
template <class _Tp> struct _STLP_alloc_proxy : allocator<_Tp> {
    _Tp* _M_data;
    _STLP_alloc_proxy(const allocator<_Tp>& a, _Tp* p) : allocator<_Tp>(a) { }
};
template <class _Tp> class _List_base {
public:
    _STLP_alloc_proxy<_List_node<_Tp> > _M_node;
    _List_base(const allocator<_List_node<_Tp> >& a) : _M_node(a, (_List_node<_Tp>*)0)
    {
        _List_node<_Tp>* n = _M_node.allocate(1);
        n->_M_next = n;
        n->_M_prev = n;
        _M_node._M_data = n;
    }
};
template <class _Tp> class list : public _List_base<_Tp> {
public:
    typedef _List_iterator<_Tp> iterator;
    typedef _List_const_iterator<_Tp> const_iterator;
    allocator<_List_node<_Tp> > get_allocator() const { return allocator<_List_node<_Tp> >(); }
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator(this->_M_node._M_data->_M_next); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
    __declspec(noinline) void _M_insert_dispatch(iterator pos, const_iterator first, const_iterator last,
                                                 const __false_type&)   // 0x005152C0 (same-TU body)
    {
        for (; first._M_node != last._M_node; first._M_node = first._M_node->_M_next) {
            _List_node<_Tp>* n = this->_M_node.allocate(1);
            new (&n->_M_data) _Tp(((_List_node<_Tp>*)first._M_node)->_M_data);
            n->_M_next = pos._M_node;
            n->_M_prev = pos._M_node->_M_prev;
            pos._M_node->_M_prev->_M_next = n;
            pos._M_node->_M_prev = n;
        }
    }
    void insert(iterator pos, const_iterator first, const_iterator last) { _M_insert_dispatch(pos, first, last, __false_type()); }
    list(const list& x) : _List_base<_Tp>(x.get_allocator()) { insert(iterator(this->_M_node._M_data), x.begin(), x.end()); }
    ~list();                                            // 0x005127A0
};

template <class _T1, class _T2> struct pair {
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x08 (+0x0C padding)
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair& __o);                              // 0x00A825D0 (out of line here)
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class vector {                     // STLport vector (12 bytes)
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _Tp* _M_end_of_storage;                             // +0x08
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);   // 0x00A84C20
    void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage) {
            _Construct(_M_finish, __x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, __x, __false_type(), 1UL, true);
    }
};
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

template <class K, class V, class C> class CVectorMap : public std::vector<std::pair<K, V> > {
public:
    C    Compare;                                       // +0x0c
    bool Dirty;                                         // +0x0d
    void Insert(const K& key, const V& value)
    {
        push_back(std::pair<K, V>(key, value));
        Dirty = true;
    }
};

template void CVectorMap<unsigned __int64, std::list<C2DLineF>, CKeyPairCompareLess<unsigned __int64, std::list<C2DLineF> > >::Insert(
    const unsigned __int64& key, const std::list<C2DLineF>& value);
