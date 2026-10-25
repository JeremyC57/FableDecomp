// std::list::insert<const_iterator>(iterator pos, const_iterator first, const_iterator last) -- list<NUISystem::CComponent*> range insert; forwards to _M_insert_dispatch with a __false_type tag
// Leaf: ??$insert@U?$_List_iterator@PAVCComponent@NUISystem@@U?$_Const_traits@PAVCComponent@NUISystem@@@std@@@std@@@?$list@PAVCComponent@NUISystem@@
// Retail: Fable.exe @ 0x0042B945, 30 bytes, `ret 0Ch`. Catalog label CEngineSceneGrid::GetCellBoundingBox is wrong. Only caller
// 0x0042BF06, the list copy constructor (base ctor 0x0042ABAF, then insert(begin(), x.begin(), x.end())), whose only caller is
// NUISystem::CDeletion's copy constructor 0x0042CD84 (sweep2: Ego_r.pdb CDeletion::AssociatedParents is list<NUISystem::CComponent*>;
// the earlier list<CActionParam*> rested on 0x0042BED4 being CAction::CAction, but that 8-byte object goes by value to
// CComponent::SetDeletion, vtable slot 62, and its list dtor is ~list<CComponent*> 0x0042ABCA: it is CDeletion::CDeletion).
// Callee 0x0042B4C3 is STLport _M_insert_dispatch(pos, first, last, const __false_type&)
// Shape: STLport _List_iterator's converting ctor from the non-const iterator is the non-const iterator's user copy ctor, so
// `pos` is constructed in its argument slot (`push ecx ; mov eax,esp ; mov [eax],edx`) while the const iterators are pushed;
// the tag goes by const reference (`lea eax,[ebp+13h]`). A plain empty `struct __false_type {}` makes VC7.1 zero the
// `__false_type()` temporary (`stosb`), which retail does not, so the tag type carries an empty user ctor here. Size TU (ebp frame, `push dword ptr [ebp+..]`).
#pragma optimize("s", on)
namespace NUISystem { class CComponent; }
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
template void list<NUISystem::CComponent*>::insert<list<NUISystem::CComponent*>::const_iterator>(
    list<NUISystem::CComponent*>::iterator, list<NUISystem::CComponent*>::const_iterator,
    list<NUISystem::CComponent*>::const_iterator);
}