// std::__insertion_sort(SKeyedList* first, SKeyedList* last, CKeyPairCompareLess comp) -- SGI insertion sort over (unsigned __int64 key, list) pairs: each element from the second on is passed by value to __linear_insert against the sorted prefix
// Leaf: __insertion_sort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A848C0, 117 bytes (+ int3 padding), `ret 4`: /Gr `__fastcall` (first in
//   ecx, spilled to the `push ecx` frame word; last in edx -> ebx; the empty comparator on the stack).
// Identity: callers 0x00A84D90/0x00A84DAE and 0x00A853ED/0x00A85407 are the two __final_insertion_sort arms (the
//   `last - first > 16` split) of the navigation quad-tree TU's CVectorMap<unsigned __int64, list<C2DLineF> > sort; the
//   callee 0x00A843F0 is __linear_insert. The catalog's "Vector_ForEach_ConstructList" is descriptive.
// Callees: malloc 0x00BFEA0E (the copied list's sentinel), list::_M_insert_dispatch 0x005152C0, __linear_insert
//   0x00A843F0 (`__fastcall`, `ret 14h`).
// Shape: SGI `if (first == last) return; for (i = first + 1; i != last; ++i) __linear_insert(first, i, value_type(*i),
//   comp);` -- the by-value pair copy built in its argument slot INLINE here (key dwords, then STLport's list copy
//   constructor: proxy zeroed, malloc(18h) sentinel self-linked, `insert(begin(), x.begin(), x.end())` with the position
//   iterator copy-constructed into its slot and the empty tag homed in the comparator's slot -- the same-TU noinline
//   _M_insert_dispatch stand-in, as in __pop_heap 0x00A83570).
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
struct CKeyPairCompareLess { };                         // empty, passed by value (compared inside __linear_insert)


void __fastcall __linear_insert(SKeyedList* first, SKeyedList* last, SKeyedList value, CKeyPairCompareLess comp);   // 0x00A843F0

void __fastcall __insertion_sort(SKeyedList* first, SKeyedList* last, CKeyPairCompareLess comp)
{
    if (first == last)
        return;
    for (SKeyedList* i = first + 1; i != last; ++i)
        __linear_insert(first, i, SKeyedList(*i), comp);
}