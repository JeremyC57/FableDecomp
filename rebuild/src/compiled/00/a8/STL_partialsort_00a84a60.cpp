// _STL::__partial_sort(SKeyedList* first, SKeyedList* middle, SKeyedList* last, SKeyedList*, CKeyPairCompareLess comp) -- SGI partial_sort over (unsigned __int64 key, list) pairs: heap the head range, pop every smaller tail element into it, then sort the heap
// Leaf: __partial_sort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A84A60, 161 bytes (+ int3 padding), `ret 0Ch`: /Gr `__fastcall` (first in
//   ecx -> ebx, middle in edx -> esi, spilled to the `push ecx` frame word; last, the type pointer and the empty comparator
//   on the stack).
// Identity: callers 0x00A84DEC (0x00A84CF0) and 0x00A85179 (0x00A85050), the introsort's depth-limit fallback in the
//   CNavQuadTree TU's CVectorMap<unsigned __int64, list<C2DLineF> > sort; its callees are the landed / authored heap
//   helpers of the same 16-byte pair. The catalog label "Heap_Merge_PairULong_BinaryTree" is descriptive.
// Callees: __make_heap 0x00A83680, malloc 0x00BFEA0E + list::_M_insert_dispatch 0x005152C0 (the inline pair copy),
//   __pop_heap 0x00A83570, sort_heap 0x00A84470.
// Shape: SGI `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first)) __pop_heap(first,
//   middle, i, _Tp(*i), comp, (ptrdiff_t*)0); sort_heap(first, middle, comp);` -- make_heap inline (the two null type
//   pointers pushed), the key comparison inline (unsigned 64-bit: high words `ja/jb`, then low `jae`), the pair copy
//   built in the argument area with its tag over the dead type-pointer slot (same-TU _M_insert_dispatch stand-in).
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

struct __false_type { __false_type() { } };

struct SListPayload {                                   // 16-byte list element (C2DLineF: four dwords)
    unsigned int m_words[4];
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
struct _List_node : _List_node_base {                   // 0x18 bytes
    SListPayload _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* x) : _M_node(x) { }
};
struct _List_iterator : _List_iterator_base {
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) { }
    _List_iterator(const _List_iterator& x) : _List_iterator_base(x._M_node) { }   // copy constructor
};
struct _List_const_iterator : _List_iterator_base {
    _List_const_iterator(_List_node_base* x) : _List_iterator_base(x) { }
    _List_const_iterator(const _List_iterator& x) : _List_iterator_base(x._M_node) { }   // converting, not a copy
};
template <class T> struct allocator {
    allocator() { }
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};
template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
    _STLP_alloc_proxy(const allocator<T>& a, T* p) : allocator<T>(a), _M_data(p) { }
};
struct _List_base {
    _STLP_alloc_proxy<_List_node> _M_node;
    _List_base(const allocator<_List_node>& a) : _M_node(a, (_List_node*)0)
    {
        _List_node* n = _M_node.allocate(1);
        n->_M_next = n;
        n->_M_prev = n;
        _M_node._M_data = n;
    }
};
struct list : _List_base {
    allocator<_List_node> get_allocator() const { return allocator<_List_node>(); }
    _List_iterator begin() { return _List_iterator(_M_node._M_data->_M_next); }
    _List_const_iterator begin() const { return _List_const_iterator(_M_node._M_data->_M_next); }
    _List_const_iterator end() const { return _List_const_iterator(_M_node._M_data); }
    __declspec(noinline) void _M_insert_dispatch(_List_iterator pos, _List_const_iterator first, _List_const_iterator last,
                                                 const __false_type&)   // 0x005152C0 (same-TU body)
    {
        for (; first._M_node != last._M_node; first._M_node = first._M_node->_M_next) {
            _List_node* n = _M_node.allocate(1);
            new (&n->_M_data) SListPayload(((_List_node*)first._M_node)->_M_data);
            n->_M_next = pos._M_node;
            n->_M_prev = pos._M_node->_M_prev;
            pos._M_node->_M_prev->_M_next = n;
            pos._M_node->_M_prev = n;
        }
    }
    void insert(_List_iterator pos, _List_const_iterator first, _List_const_iterator last)
    {
        _M_insert_dispatch(pos, first, last, __false_type());
    }
    list(const list& x) : _List_base(x.get_allocator()) { insert(begin(), x.begin(), x.end()); }
    list& operator=(const list& x);                     // 0x00517720
    ~list();                                            // 0x005127A0
};

struct SKeyedList {                                     // std::pair<unsigned __int64, list<C2DLineF> >, 0x10 bytes
    unsigned __int64 first;                             // +0x00
    list             second;                            // +0x08 (+0x0C padding)
};
struct CKeyPairCompareLess {                            // empty, passed by value
    bool operator()(const SKeyedList& a, const SKeyedList& b) const { return a.first < b.first; }
};

void __fastcall __make_heap(SKeyedList* first, SKeyedList* last, CKeyPairCompareLess comp, SKeyedList*, int*);   // 0x00A83680
void __fastcall __pop_heap(SKeyedList* first, SKeyedList* last, SKeyedList* result, SKeyedList value,
                           CKeyPairCompareLess comp, int*);   // 0x00A83570
void __fastcall sort_heap(SKeyedList* first, SKeyedList* last, CKeyPairCompareLess comp);   // 0x00A84470

inline void make_heap(SKeyedList* first, SKeyedList* last, CKeyPairCompareLess comp)
{
    __make_heap(first, last, comp, (SKeyedList*)0, (int*)0);
}

void __fastcall __partial_sort(SKeyedList* first, SKeyedList* middle, SKeyedList* last, SKeyedList*, CKeyPairCompareLess comp)
{
    make_heap(first, middle, comp);
    for (SKeyedList* i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, SKeyedList(*i), comp, (int*)0);
    sort_heap(first, middle, comp);
}