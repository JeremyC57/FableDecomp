// _STL::__make_heap(SKeyedList* first, SKeyedList* last, CKeyPairCompareLess comp, SKeyedList*, int*) -- SGI make_heap over (unsigned __int64 key, list) pairs: sift every parent position down from (len - 2) / 2 to the root
// Leaf: __make_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A83680, 156 bytes (+ int3 padding), `ret 0Ch`: /Gr `__fastcall` (first in
//   ecx, last in edx; the empty comparator and the two type pointers on the stack).
// Identity: callers 0x00A83EF9 (0x00A83E80) and 0x00A84A76 (partial_sort 0x00A84A60) -- the CNavQuadTree TU's
//   CVectorMap<unsigned __int64, list<C2DLineF> > sort (the 16-byte pair of the landed sort_heap 0x00A84470 and
//   __destroy_aux 0x00A83420). The catalog label "HeapSort_BuildHeap_PairTextureDecal" is descriptive.
// Callees: malloc 0x00BFEA0E (the copied list's sentinel), list::_M_insert_dispatch 0x005152C0, __adjust_heap 0x00A82BE0
//   (`__fastcall`, `ret 18h`).
// Shape: SGI `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2; for (;;) { __adjust_heap(first,
//   parent, len, _Tp(*(first + parent)), comp); if (parent == 0) return; parent--; }` -- first and len spilled to the
//   `sub esp,8` frame, the element pointer strength-reduced (edi = &first[parent].second), the pair copy built in the
//   argument area (key dwords, STLport's inline list copy constructor), its empty tag over the comparator slot through
//   the same-TU _M_insert_dispatch stand-in.
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
struct CKeyPairCompareLess { };                         // empty, passed by value

void __fastcall __adjust_heap(SKeyedList* first, int holeIndex, int len, SKeyedList value, CKeyPairCompareLess comp);   // 0x00A82BE0

void __fastcall __make_heap(SKeyedList* first, SKeyedList* last, CKeyPairCompareLess comp, SKeyedList*, int*)
{
    if (last - first < 2)
        return;
    int len = int(last - first);
    int parent = (len - 2) / 2;

    for (;;) {
        __adjust_heap(first, parent, len, SKeyedList(*(first + parent)), comp);
        if (parent == 0)
            return;
        parent--;
    }
}