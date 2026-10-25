// std::list<CCountedPointer<CCombatActionBase::CStrikeEvent> >::insert<const_iterator>(iterator pos, const_iterator first, const_iterator last)
// -- STLport range insert: insert a copy of each element of [first, last) before pos
// Leaf: ??$insert@U?$_List_iterator@V?$CCountedPointer@VCStrikeEvent@CCombatActionBase@@@@U?$_Const_traits@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0062F650, 54 bytes, thiscall `ret 0Ch` (pos, first, last).
// Renamed from std::list<CCountedPointer<C3DSkeletalMorphParameter> >::insert (FINDINGS chunk 382: the element is
//   CCountedPointer<CStrikeEvent>). Identity: the only caller is the landed list copy constructor 0x0062F610
//   (`insert(begin(), x.begin(), x.end())`), whose only caller is the CCombatActionBase copy constructor 0x0062F3B0
//   copying StrikeEvents (struct_layouts_egor: list<CCountedPointer<CCombatActionBase::CStrikeEvent>>); the only callee
//   is the landed single-element insert 0x0062F690 of the same list. Retail has no identical-code folding (the
//   C3DSkeletalMorphParameter instance is the byte twin 0x0067CFF0). Ego_r.pdb nests CStrikeEvent in CCombatActionBase.
// Layout: retail list = the bare sentinel-node pointer; node next +0, prev +4, value +8.
// Shape: STLport 4.x `insert(pos, first, last)` with `_M_insert_dispatch(.., __false_type)` inlined:
//   `for (; first != last; ++first) insert(pos, *first);`. pos is an iterator, whose STLport
//   `_List_iterator(const iterator&)` is its user copy constructor, so each call builds the copy in its argument slot
//   (`push ecx ; mov eax,esp ; mov [eax],ebp`); first/last are const_iterators (trivial copies). The returned iterator
//   temporary is homed on the dead `first` argument slot.
class CCPPointerInfo;
template <class T> class CCountedPointer {       // 8 bytes: object + shared reference-count block
public:
    T*              Data;                        // +0x00
    CCPPointerInfo* Info;                        // +0x04
};

class CCombatActionBase {
public:
    class CStrikeEvent;
};

namespace std {

template <class _Tp> class allocator;

struct _List_node_base {
    _List_node_base* _M_next;           // +0x00
    _List_node_base* _M_prev;           // +0x04
};

template <class _Tp> struct _List_node : _List_node_base {
    _Tp _M_data;                        // +0x08
};

template <class _Tp> struct _Nonconst_traits;
template <class _Tp> struct _Const_traits {
    typedef const _Tp& reference;
    typedef _Nonconst_traits<_Tp> _Non_const_traits;
};
template <class _Tp> struct _Nonconst_traits {
    typedef _Tp& reference;
    typedef _Nonconst_traits<_Tp> _Non_const_traits;
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) {}
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp, class _Traits> struct _List_iterator : _List_iterator_base {
    typedef _List_iterator<_Tp, typename _Traits::_Non_const_traits> iterator;
    typedef typename _Traits::reference reference;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) {}
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) {}
    reference operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_incr(); return *this; }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list {
public:
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> >    const_iterator;
    _List_node<_Tp>* _M_node;                          // +0x00 the sentinel
    iterator insert(iterator __position, const _Tp& __x);                    // 0x0062F690
    template <class _InputIterator>
    void insert(iterator __pos, _InputIterator __first, _InputIterator __last)
    {
        for (; __first != __last; ++__first)
            insert(__pos, *__first);
    }
};

typedef CCountedPointer<CCombatActionBase::CStrikeEvent> TStrikeEventPtr;
template void list<TStrikeEventPtr>::insert<list<TStrikeEventPtr>::const_iterator>(
    list<TStrikeEventPtr>::iterator, list<TStrikeEventPtr>::const_iterator, list<TStrikeEventPtr>::const_iterator);

} // namespace std
