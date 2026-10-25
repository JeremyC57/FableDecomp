// std::vector<list<C2DLineF> >::resize(unsigned int newSize) -- resize a navigation layer's per-cell line-list array: erase the surplus lists, or append empty lists up to the new size
// Leaf: ?resize@?$vector@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A85240, 112 bytes, thiscall `ret 4`.
// Identity: the only caller 0x00A7A613 is CNavQuadTree::InitialiseLines 0x00A7A5E0, whose landed source calls it as the line
//   lists' EnsureCapacity(width * height); the callees are this vector's erase 0x00A84090 and _M_fill_insert 0x00A84690.
//   The catalog's "Vector_EnsureCapacity_TreeLines" is descriptive.
// Callees: malloc 0x00BFEA0E (the default list's sentinel), erase(first, last) 0x00A84090, _M_fill_insert(pos, n, x)
//   0x00A84690, list::~list 0x005127A0.
// Shape: STLport `resize(n) { resize(n, _Tp()); }` with `resize(n, x) { if (n < size()) erase(begin() + n, end()); else
//   insert(end(), n - size(), x); }` inline: the default list temporary lives in the `push ecx` frame word (sentinel
//   malloc'd and self-linked, the proxy's zero store dropped), and its destructor runs on both exits. The same-TU
//   noinline stand-ins for ~list and _M_fill_insert keep the temporary from being address-exposed (with declarations
//   only VC7.1 keeps the dead proxy zero store, as in the parked CVectorMap::Insert 0x00A852B0).
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // stand-in bodies only

class C2DLineF;

namespace std {

struct _List_node_base {
    _List_node_base* _M_next;                           // +0x00
    _List_node_base* _M_prev;                           // +0x04
};

template <class _Tp> struct _List_node : public _List_node_base {
    unsigned char _M_data[0x10];                        // +0x08 C2DLineF (16 bytes)
};

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp, class _Alloc> class _List_base {
public:
    _STLP_alloc_proxy<_List_node<_Tp>*, _List_node<_Tp>, allocator<_List_node<_Tp> > > _M_node;   // +0x00 sentinel
    _List_base(const allocator<_List_node<_Tp> >& __a) : _M_node(__a, (_List_node<_Tp>*)0)
    {
        _M_node._M_data = _M_node.allocate(1);
        _M_node._M_data->_M_next = _M_node._M_data;
        _M_node._M_data->_M_prev = _M_node._M_data;
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    list() : _List_base<_Tp, _Alloc>(allocator<_List_node<_Tp> >()) { }
    ~list();                                            // 0x005127A0 (same-TU body below)
};

template <class _Tp> class vector_allocator {
public:
    vector_allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _Vector_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _Vector_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x08
};

template <class _Tp, class _Alloc = vector_allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    _Tp* begin() { return this->_M_start; }
    _Tp* end() { return this->_M_finish; }
    _Tp* erase(_Tp* __first, _Tp* __last);              // 0x00A84090
    void _M_fill_insert(_Tp* __pos, unsigned int __n, const _Tp& __x);   // 0x00A84690 (same-TU body below)
    void insert(_Tp* __pos, unsigned int __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(unsigned int __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
    void resize(unsigned int __new_size) { resize(__new_size, _Tp()); }
};

// Same-TU stand-ins for ~list 0x005127A0 and _M_fill_insert 0x00A84690 (simplified; only their visibility matters): they do
// not let the temporary's address escape, so VC7.1 drops the proxy's dead zero store as retail.
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) list<_Tp, _Alloc>::~list()
{
    _List_node_base* __head = this->_M_node._M_data;
    _List_node_base* __cur = __head->_M_next;
    while (__cur != __head) {
        _List_node_base* __tmp = __cur;
        __cur = __cur->_M_next;
        free(__tmp);
    }
    __head->_M_next = __head;
    __head->_M_prev = __head;
    if (__head)
        free(__head);
}

template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_fill_insert(_Tp* __pos, unsigned int __n, const _Tp& __x)
{
    if (__n != 0 && __pos == this->_M_finish && __x._M_node._M_data != 0)
        this->_M_finish = this->_M_end_of_storage._M_data;
}
#pragma auto_inline(on)

template void vector<list<C2DLineF> >::resize(unsigned int __new_size);
}