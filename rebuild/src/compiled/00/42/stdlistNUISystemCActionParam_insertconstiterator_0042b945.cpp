// std::list<NUISystem::CActionParam*>::insert<const_iterator>(iterator pos, const_iterator first, const_iterator last) -- range insert; forwards to _M_insert_dispatch with a __false_type tag
// Leaf: insert@?$list@PAVCActionParam@NUISystem@@
// Retail: Fable.exe @ 0x0042B945, 30 bytes, `ret 0Ch`. Catalog label CEngineSceneGrid::GetCellBoundingBox is wrong. Only caller
// 0x0042BF06, the list copy constructor (base ctor 0x0042ABAF, as in NUISystem::CAction::CAction 0x0042BED4, then
// insert(begin(), x.begin(), x.end())). Callee 0x0042B4C3 is STLport _M_insert_dispatch(pos, first, last, const __false_type&)
// (`ret 10h`, loops insert(pos, *first) via 0x0042AB8A on node +8).
// Shape: STLport _List_iterator's converting ctor from the non-const iterator is the non-const iterator's user copy ctor, so
// `pos` is constructed in its argument slot (`push ecx ; mov eax,esp ; mov [eax],edx`) while the const iterators are pushed;
// the tag goes by const reference (`lea eax,[ebp+13h]`). A plain empty `struct __false_type {}` makes VC7.1 zero the
// `__false_type()` temporary (`stosb`), which retail does not, so the tag type carries an empty user ctor here. Size TU (ebp frame, `push dword ptr [ebp+..]`).
#pragma optimize("s", on)
namespace NUISystem { class CActionParam; }
namespace std {
struct __false_type { __false_type() {} };
template <class T> class allocator { public: allocator() {} };
template <class T> struct _Nonconst_traits {};
template <class T> struct _Const_traits {};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> struct _List_node : _List_node_base {
    T _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* x) : _M_node(x) {}
    bool operator!=(const _List_iterator_base& y) const { return _M_node != y._M_node; }
};
template <class T, class Traits> struct _List_iterator : _List_iterator_base {
    typedef _List_iterator<T, _Nonconst_traits<T> > iterator;
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) {}
    _List_iterator(const iterator& x) : _List_iterator_base(x._M_node) {}
};
template <class T, class A = allocator<T> > class list {
public:
    typedef _List_iterator<T, _Nonconst_traits<T> > iterator;
    typedef _List_iterator<T, _Const_traits<T> >    const_iterator;
    _List_node_base* _M_node;
    iterator insert(iterator pos, const T& x);                              // 0x0042AB8A (out of line)
    template <class InputIter>
    void _M_insert_dispatch(iterator pos, InputIter first, InputIter last, const __false_type&);   // 0x0042B4C3
    template <class InputIter>
    void insert(iterator pos, InputIter first, InputIter last)
    {
        _M_insert_dispatch(pos, first, last, __false_type());
    }
};
template void list<NUISystem::CActionParam*>::insert<list<NUISystem::CActionParam*>::const_iterator>(
    list<NUISystem::CActionParam*>::iterator, list<NUISystem::CActionParam*>::const_iterator,
    list<NUISystem::CActionParam*>::const_iterator);
}