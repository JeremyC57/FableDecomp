// _STL::__unguarded_partition(SKeyedList* first, SKeyedList* last, SKeyedList pivot, CKeyPairCompareLess comp) -- SGI quicksort partition step over (unsigned __int64 key, list) pairs: advance from both ends past elements on the correct side of the pivot key and swap the strays; returns the split point
// Leaf: __unguarded_partition
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A83CC0, 88 bytes (+ int3 padding), `ret 14h`: /Gr `__fastcall` (first in
//   ecx -> esi, last in edx -> edi; the 16-byte pivot by value and the empty comparator on the stack).
// Identity: the only caller is 0x00A85137 (the introsort loop 0x00A85050) of the CNavQuadTree TU's
//   CVectorMap<unsigned __int64, list<C2DLineF> > sort (the 16-byte pair of the landed sort_heap 0x00A84470). The catalog
//   label "Partition_GuiTree_16ByteElements" is descriptive.
// Callees: iter_swap 0x00A82B10 (`__fastcall`, the two element pointers in ecx/edx), list::~list 0x005127A0 (the
//   by-value pivot destroyed by the callee).
// Shape: SGI `for (;;) { while (comp(*first, pivot)) ++first; --last; while (comp(pivot, *last)) --last; if (!(first <
//   last)) return first; iter_swap(first, last); ++first; }` -- the pivot key held in ebp (low) / ebx (high), the
//   unsigned 64-bit key comparisons inline, the second scan folded into one decrement loop; the pivot's list destroyed on
//   the way out.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);             // 0x00BFEA14 (stand-in body only)
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

// Same-TU stand-in for list::~list 0x005127A0 (the STLport clear + sentinel free): with only a declaration VC7.1 treats
// the by-value pivot as address-exposed for the whole function and re-reads its key after every iter_swap call; with a
// body it can see that `this` does not escape, so the key stays in ebx/ebp as retail.
#pragma auto_inline(off)
__declspec(noinline) list::~list()
{
    _List_node_base* cur = _M_node._M_data->_M_next;
    while (cur != _M_node._M_data) {
        _List_node_base* tmp = cur;
        cur = cur->_M_next;
        free(tmp);
    }
    _M_node._M_data->_M_next = _M_node._M_data;
    _M_node._M_data->_M_prev = _M_node._M_data;
    if (_M_node._M_data)
        free(_M_node._M_data);
}
#pragma auto_inline(on)

struct SKeyedList {                                     // std::pair<unsigned __int64, list<C2DLineF> >, 0x10 bytes
    unsigned __int64 first;                             // +0x00
    list             second;                            // +0x08 (+0x0C padding)
};
struct CKeyPairCompareLess {                            // empty, passed by value
    bool operator()(const SKeyedList& a, const SKeyedList& b) const { return a.first < b.first; }
};

void __fastcall iter_swap(SKeyedList* a, SKeyedList* b);   // 0x00A82B10

SKeyedList* __fastcall __unguarded_partition(SKeyedList* first, SKeyedList* last, SKeyedList pivot, CKeyPairCompareLess comp)
{
    for (;;) {
        while (comp(*first, pivot))
            ++first;
        --last;
        while (comp(pivot, *last))
            --last;
        if (!(first < last))
            return first;
        iter_swap(first, last);
        ++first;
    }
}