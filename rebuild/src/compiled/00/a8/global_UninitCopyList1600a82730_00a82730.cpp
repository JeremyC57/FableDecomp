// _global::UninitCopy_List16_00a82730(const SList16_00a82730* first, const SList16_00a82730* last, SList16_00a82730* result, const __false_type&) -- STLport __uninitialized_copy<list<T>*> (16-byte T): placement copy-construct each list (fresh self-linked header, then insert the source range), return the advanced result
// Leaf: UninitCopy_List16_00a82730
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A82730, length 87 bytes, `ret 8`.
// Callers: 0x00A82AA9 (0x00A829E0), 0x00A831B7 (vector copy constructor 0x00A83170), 0x00A83DC7 / 0x00A83E66 (0x00A83D70),
//   0x00A854DD (the landed record __uninitialized_copy 0x00A85420, whose same-TU stand-in for this callee is a 4-byte
//   int copy -- only its frame matters there) and 0x00A8561C (0x00A85560): the vector<list<T> > helpers of the
//   navigation quad-tree TU.
// Callees: malloc @ 0x00BFEA0E (the 0x18-byte header node), list<T>::_M_insert_dispatch(iterator, const_iterator,
//   const_iterator, const __false_type&) @ 0x005152C0 (`ret 10h`; catalog "Std_List_Construct_From_Iterator_Range").
// Identity: the catalog label `_Uninit_copy<_Vector_const_iterator<..unsigned int..>, ...>` is propagated. The element is a
//   retail 4-byte STLport list (one header-node pointer, GOTCHAS 00519ef0) whose node is 0x18 bytes (next, prev, a 16-byte
//   value); the value type is not named -- descriptive. Byte twin of 0x00A831D0 (another 16-byte value list).
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)`, `__fastcall` (first in ecx -> edi,
//   last in edx -> ebx, result + the tag reference on the stack -> esi). list's copy constructor is `_List_base(a)` -- the
//   alloc proxy nulls the node word BEFORE the malloc, the header is linked to itself and stored -- then
//   `insert(begin(), x.begin(), x.end())` forwarding to the out-of-line `_M_insert_dispatch(pos, first, last, _Integral())`:
//   the iterator has a user copy constructor so `pos` is built in its argument slot (`push ecx ; mov ecx,esp ; mov [ecx],eax`),
//   the const_iterators are pushed as plain words, and the empty __false_type temporary is homed in the dead `result`
//   argument slot (`lea ebp,[esp+14h]`). Placement-new null test; the advanced result returned from esi.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
struct SValue16_00a82730 { long m_0, m_4, m_8, m_c; };
struct _List_node_00a82730 : _List_node_base {  // 0x18 bytes
    SValue16_00a82730 _M_data;
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* x) : _M_node(x) { }
};
struct _List_iterator_00a82730 : _List_iterator_base {          // iterator: user copy constructor
    _List_iterator_00a82730(_List_node_base* x) : _List_iterator_base(x) { }
    _List_iterator_00a82730(const _List_iterator_00a82730& x) : _List_iterator_base(x._M_node) { }
};
struct _List_const_iterator_00a82730 : _List_iterator_base {    // const_iterator: implicit (trivial) copy
    _List_const_iterator_00a82730(_List_node_base* x) : _List_iterator_base(x) { }
};

template <class T> struct allocator { allocator() { } };
template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
    _STLP_alloc_proxy(const allocator<T>& a, T* p) : allocator<T>(a), _M_data(p) { }
};

struct _List_base_00a82730 {                    // STLport _List_base (4 bytes)
    _STLP_alloc_proxy<_List_node_00a82730> _M_node;
    _List_base_00a82730(const allocator<_List_node_00a82730>& a) : _M_node(a, 0)
    {
        _List_node_00a82730* n = (_List_node_00a82730*)malloc(sizeof(_List_node_00a82730));
        n->_M_next = n;
        n->_M_prev = n;
        _M_node._M_data = n;
    }
};

struct SList16_00a82730 : _List_base_00a82730 { // _STL::list<SValue16>
    typedef _List_iterator_00a82730 iterator;
    typedef _List_const_iterator_00a82730 const_iterator;
    allocator<_List_node_00a82730> get_allocator() const { return allocator<_List_node_00a82730>(); }
    iterator begin() { return iterator(_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator(_M_node._M_data->_M_next); }
    const_iterator end() const { return const_iterator(_M_node._M_data); }
    void _M_insert_dispatch(iterator pos, const_iterator first, const_iterator last, const __false_type&);   // 0x005152C0
    void insert(iterator pos, const_iterator first, const_iterator last) { _M_insert_dispatch(pos, first, last, __false_type()); }
    SList16_00a82730(const SList16_00a82730& x) : _List_base_00a82730(x.get_allocator()) { insert(begin(), x.begin(), x.end()); }
};

SList16_00a82730* __fastcall UninitCopy_List16_00a82730(const SList16_00a82730* first, const SList16_00a82730* last, SList16_00a82730* result, const __false_type&)
{
    SList16_00a82730* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) SList16_00a82730(*first);
    return cur;
}