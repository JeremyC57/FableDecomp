// std::sort_heap(SKeyedList* first, SKeyedList* last, CKeyPairCompareLess comp) -- SGI sort_heap over (unsigned __int64 key, STLport list) pairs: pop the root to the back until one element remains
// Leaf: sort_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A84470, length 142 bytes (+ int3 padding), `ret 4`.
// Identity: the catalog label `_Sort_heap<std::pair<long,unsigned long>*>` is propagated over the byte-identical
// skeleton. The callee 0x00A83570 (manifest "Std_Heap_Construct_And_Adjust") is the SGI `__pop_heap` of a 16-byte
// pair (`ret 1Ch`; key dwords +0/+4, list assignment 0x00517720 at +8, the list destroyed through 0x005127A0) and
// 0x005152C0 is the list's `_M_insert_dispatch(pos, first, last, const __false_type&)` (0x18-byte nodes carrying a
// four-dword payload) -- the same (unsigned __int64 key, list) record as the landed 0x0051C590 source: +0x0C is
// alignment padding after the list (never copied).
// Callees: __pop_heap @ 0x00A83570 (out of line, __fastcall), malloc @ 0x00BFEA0E (the list's sentinel node),
// list::_M_insert_dispatch @ 0x005152C0 (thiscall, `ret 10h`).
// Shape: /Gr SGI algorithm TU -- `while (last - first > 1) pop_heap(first, last--, comp)` with pop_heap and
// __pop_heap_aux inline (16-byte stride). The value copy in the argument area is the implicit pair copy: the key's
// two dwords, then STLport's list copy constructor inline -- `_List_base(a)` (proxy zeroed, `allocate(1)` =
// malloc(18h), sentinel self-linked) and `insert(begin(), x.begin(), x.end())` forwarding to the out-of-line
// `_M_insert_dispatch` (the position iterator copy-constructed into its slot: iterator's copy constructor is
// user-defined, const_iterator only has the converting one, so first/last are plain pushes). _M_insert_dispatch
// gets a same-TU __declspec(noinline) stand-in body (the 0x00A85420 recipe): with only a declaration the empty
// `__false_type()` tag takes its own frame byte (`sub esp,8`, `lea ebp,[esp+2Bh]`, 146 v 142 bytes); with the body
// in the TU it overlays the dead comparator slot (`lea ebp,[esp+30h]`) as retail. A tag without a user ctor adds a
// zero-byte store.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

struct __false_type { __false_type() { } };

struct SListPayload {                                   // 16-byte list element (four dwords)
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
};

struct SKeyedList {                                     // std::pair<unsigned __int64, list>, 0x10 bytes
    unsigned __int64 first;                             // +0x00
    list             second;                            // +0x08 (+0x0C padding)
};
struct CKeyPairCompareLess { };                         // empty, passed by value

void __fastcall __pop_heap(SKeyedList* first, SKeyedList* last, SKeyedList* result, SKeyedList value,
                           CKeyPairCompareLess comp, int*);   // 0x00A83570

inline void __pop_heap_aux(SKeyedList* first, SKeyedList* last, SKeyedList*, CKeyPairCompareLess comp)
{
    __pop_heap(first, last - 1, last - 1, SKeyedList(*(last - 1)), comp, (int*)0);
}

inline void pop_heap(SKeyedList* first, SKeyedList* last, CKeyPairCompareLess comp)
{
    __pop_heap_aux(first, last, (SKeyedList*)0, comp);
}

void __fastcall sort_heap(SKeyedList* first, SKeyedList* last, CKeyPairCompareLess comp)
{
    while (last - first > 1)
        pop_heap(first, last--, comp);
}