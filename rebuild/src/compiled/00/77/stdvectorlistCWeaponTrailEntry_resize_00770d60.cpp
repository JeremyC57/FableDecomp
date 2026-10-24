// std::vector<list<CWeaponTrailEntry> >::resize(unsigned int new_size) -- STLport resize of the per-weapon trail-list vector: build an empty list, then erase the surplus lists or append copies of the empty list up to new_size
// Leaf: ?resize@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00770D60, 112 bytes, thiscall `ret 4`.
// Callees: malloc @ 0x00BFEA0E (the default list's 0x24-byte sentinel), vector::erase(iterator, iterator) @ 0x00770790,
//   vector::_M_fill_insert(iterator, size_type, const list&) @ 0x00770990, list::~list() @ 0x004DB5BB (the ICF-shared
//   STLport list destructor labelled CActiveFile::OnReadFinished).
// Caller: 0x0076EEC5 (CTCWeaponTrail: sizes Trail to the trail count). Element: the 4-byte STLport
//   list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry> of CTCWeaponTrail::Trail.
// Label correction: catalogued as `LinkedList_Phoneme_Insert`; the body is STLport `vector<list<..> >::resize(n)`.
// Shape: STLport 4.x `resize(n) { resize(n, _Tp()); }` over `resize(n, x) { if (n < size()) erase(begin() + n, end());
//   else insert(end(), n - size(), x); }` with `insert(pos, n, x)` forwarding to the out-of-line _M_fill_insert: the
//   default list (sentinel malloc'd and self-linked) lives in the `push ecx` frame word and is destroyed out of line on
//   both paths. The default list constructor initialises only the proxy's allocator base: STLport's `_M_node(__a, 0)`
//   leaves a `mov [esp+0Ch],0` before the malloc (the local's address escapes to the out-of-line callees, so VC7.1
//   keeps the store), which retail does not have.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E

class CEnginePrimitiveWeaponTrail {
public:
    class CWeaponTrailEntry {                             // 28 bytes: two C3DVectors and a float
    public:
        float Data[7];
    };
};

namespace std {

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
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
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a) : _MaybeReboundAlloc(__a) { }
};

template <class _Tp> class list {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _M_node;
    list() : _M_node(allocator<_Node>())
    {
        _List_node_base* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    ~list();                                         // 0x004DB5BB
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* iterator;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    iterator erase(iterator __first, iterator __last);                             // 0x00770790
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);           // 0x00770990
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(size_type __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};

}

template void std::vector<std::list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry> >::resize(unsigned int);