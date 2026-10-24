// _global::UninitCopy_ListLongPair_00865650(const SListLongPair_00865650* first, const SListLongPair_00865650* last, SListLongPair_00865650* result, const __false_type&) -- STLport __uninitialized_copy over 8-byte {list<T>, long} records (0x1C-byte T): copy-construct each list (fresh header + insert the source range) and the trailing word, return the advanced result
// Leaf: UninitCopy_ListLongPair_00865650
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00865650, length 93 bytes (+3 int3 pad), `ret 8`.
// Callers: 0x0086582B / 0x00865878 in the vector reallocation 0x008657E0 (catalog "Reallocate_pair_animation").
// Callees: malloc @ 0x00BFEA0E (the 0x24-byte header node), list<T>::_M_insert_dispatch(iterator, const_iterator,
//   const_iterator, const __false_type&) @ 0x0076FD40 (catalog "LinkedList_InsertAll").
// Identity: the catalog label `_Uninit_copy<_Vector_const_iterator<..unsigned int..>, ...>` is propagated. The element is a
//   retail 4-byte STLport list (header-node pointer; node 0x24 bytes = next, prev, a 0x1C-byte value) followed by a word
//   (+4); neither type is named by a landed relative -- descriptive.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)`, `__fastcall` (first in ecx -> edi,
//   last in edx -> ebx, result + the tag reference on the stack -> esi). The implicit memberwise copy runs list's copy
//   constructor -- `_List_base(a)` nulls the node word before the malloc, links the header to itself and stores it, then
//   `insert(begin(), x.begin(), x.end())` forwards to the out-of-line `_M_insert_dispatch` (the iterator built in its
//   argument slot, the const_iterators pushed as words, the empty tag homed in the dead `result` slot) -- and then copies
//   the word at +4. Placement-new null test; the advanced result returned from esi.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
struct SValue1C_00865650 { long m_words[7]; };  // 0x1C-byte list value
struct _List_node_00865650 : _List_node_base {  // 0x24 bytes
    SValue1C_00865650 _M_data;
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* x) : _M_node(x) { }
};
struct _List_iterator_00865650 : _List_iterator_base {          // iterator: user copy constructor
    _List_iterator_00865650(_List_node_base* x) : _List_iterator_base(x) { }
    _List_iterator_00865650(const _List_iterator_00865650& x) : _List_iterator_base(x._M_node) { }
};
struct _List_const_iterator_00865650 : _List_iterator_base {    // const_iterator: implicit (trivial) copy
    _List_const_iterator_00865650(_List_node_base* x) : _List_iterator_base(x) { }
};

template <class T> struct allocator { allocator() { } };
template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
    _STLP_alloc_proxy(const allocator<T>& a, T* p) : allocator<T>(a), _M_data(p) { }
};

struct _List_base_00865650 {                    // STLport _List_base (4 bytes)
    _STLP_alloc_proxy<_List_node_00865650> _M_node;
    _List_base_00865650(const allocator<_List_node_00865650>& a) : _M_node(a, 0)
    {
        _List_node_00865650* n = (_List_node_00865650*)malloc(sizeof(_List_node_00865650));
        n->_M_next = n;
        n->_M_prev = n;
        _M_node._M_data = n;
    }
};

struct SList1C_00865650 : _List_base_00865650 { // _STL::list<SValue1C>
    typedef _List_iterator_00865650 iterator;
    typedef _List_const_iterator_00865650 const_iterator;
    allocator<_List_node_00865650> get_allocator() const { return allocator<_List_node_00865650>(); }
    iterator begin() { return iterator(_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator(_M_node._M_data->_M_next); }
    const_iterator end() const { return const_iterator(_M_node._M_data); }
    void _M_insert_dispatch(iterator pos, const_iterator first, const_iterator last, const __false_type&);   // 0x0076FD40
    void insert(iterator pos, const_iterator first, const_iterator last) { _M_insert_dispatch(pos, first, last, __false_type()); }
    SList1C_00865650(const SList1C_00865650& x) : _List_base_00865650(x.get_allocator()) { insert(begin(), x.begin(), x.end()); }
};

struct SListLongPair_00865650 {                 // 8 bytes
    SList1C_00865650 m_list;                    // +0x00
    long             m_value;                   // +0x04
};

SListLongPair_00865650* __fastcall UninitCopy_ListLongPair_00865650(const SListLongPair_00865650* first, const SListLongPair_00865650* last, SListLongPair_00865650* result, const __false_type&)
{
    SListLongPair_00865650* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) SListLongPair_00865650(*first);
    return cur;
}