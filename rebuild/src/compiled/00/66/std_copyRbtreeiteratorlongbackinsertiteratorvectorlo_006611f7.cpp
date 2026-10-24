// std::__copy<_Rb_tree_iterator<long>,back_insert_iterator<vector<long> >,int>(first, last, result, const input_iterator_tag&, int*) -- push every key of a set<long> range onto a vector through a back inserter
// Leaf: ??$__copy@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006611F7, 51 bytes, `__fastcall` (hidden result in ecx, the empty tag reference in
// edx; first, last, the back inserter and the null distance pointer on the stack, `ret 10h`). Only caller: std::copy
// @ 0x0066124B (`push 0`, tag at [ebp+13h]; manifest "ConstructString" is propagated), the CTCVillage serialiser's copy of
// PlayersCrimes (set<long>) into a vector<long> (whose reserve 0x00660684 it called first).
// Callees: vector<long>::push_back(const long&) @ 0x00485FF3 (the inserter's operator=, `mov ecx,ebx`: the container
// pointer is the inserter's only member), _Rb_global<bool>::_M_increment @ 0x004292C0 (cdecl, the set iterator's ++).
// Label correction: the catalog "MemoryAllocator_IterateAllocatedAreas" is descriptive; this is STLport `__copy` for
// input iterators.
// Shape: STLport verbatim `for (; first != last; ++result, ++first) *result = *first; return result;` -- the key
// address is node+0x10, the inserter's container pointer stays in ebx and is stored through the hidden pointer; the
// back_insert_iterator has a user constructor, so it is returned in memory.
// Opt: size TU (`optimize("s", on)`; `jmp` to the loop test).
#pragma optimize("s", on)

namespace std {
struct input_iterator_tag { };

struct _Rb_tree_node_base {
    bool                _M_color;                 // +0x00
    _Rb_tree_node_base* _M_parent;                // +0x04
    _Rb_tree_node_base* _M_left;                  // +0x08
    _Rb_tree_node_base* _M_right;                 // +0x0c
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                        // +0x10
};
template <class _Dummy> struct _Rb_global {
    static _Rb_tree_node_base* __cdecl _M_increment(_Rb_tree_node_base* __x);   // 0x004292C0
};
typedef _Rb_global<bool> _Rb_global_inst;

template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) { }
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) : _M_node(__it._M_node) { }
    _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    _Rb_tree_iterator& operator++() { _M_node = _Rb_global_inst::_M_increment(_M_node); return *this; }
    bool operator!=(const _Rb_tree_iterator& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp> class vector {
public:
    typedef _Tp value_type;
    _Tp* _M_start;                                // +0x00
    _Tp* _M_finish;                               // +0x04
    _Tp* _M_end_of_storage;                       // +0x08
    void push_back(const _Tp& __x);               // 0x00485FF3
};

template <class _Container> class back_insert_iterator {
protected:
    _Container* container;
public:
    explicit back_insert_iterator(_Container& __x) : container(&__x) { }
    back_insert_iterator& operator=(const typename _Container::value_type& __value) { container->push_back(__value); return *this; }
    back_insert_iterator& operator*() { return *this; }
    back_insert_iterator& operator++() { return *this; }
};

template <class _InputIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_InputIter __first, _InputIter __last, _OutputIter __result, const input_iterator_tag&, _Distance*)
{
    for (; __first != __last; ++__result, ++__first)
        *__result = *__first;
    return __result;
}

template back_insert_iterator<vector<long> > __fastcall __copy(_Rb_tree_iterator<long>, _Rb_tree_iterator<long>,
    back_insert_iterator<vector<long> >, const input_iterator_tag&, int*);
}