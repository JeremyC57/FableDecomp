// std::list<CSliderAnimator>::push_front(const CSliderAnimator& __x) -- insert x before the first element
// Leaf: ?push_front@?$list@VCSliderAnimator@@V?$allocator@VCSliderAnimator@@@std@@@std@@QAEXABVCSliderAnimator@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0057953E, 28 bytes, thiscall, `ret 4` (frame-pointer size module).
// Identity (probable): called eight times by CTCInventoryExperience's virtual update methods 0x00577216 / 0x00577AC0; its insert 0x0057929A builds 0x24-byte nodes (a 28-byte element copied with `rep movsd` x7), and Ego_r.pdb CTCInventoryExperience has exactly one list, Animators, list<CSliderAnimator> (CSliderAnimator: 28 bytes, pSlider / StartValue / EndValue ...).
// Callees: list<CSliderAnimator>::insert(iterator, const CSliderAnimator&) 0x0057929A (thiscall, the iterator returned through a hidden pointer).
// Shape (family M122, representative 0x0053CA3C): STLport 4.6 `void push_front(const _Tp& __x) { insert(begin(), __x); }` --
//   `optimize("y", off)` + `optimize("s", on)`; begin() is built in its argument slot by the iterator's user copy constructor
//   (`push ecx ; mov eax,esp ; mov [eax],edx`), the discarded result is homed in the dead x slot.
#pragma optimize("y", off)
#pragma optimize("s", on)
namespace NUISystem { class CComponent; }
class CSliderAnimator {                                  // Ego_r.pdb, 28 bytes
public:
    NUISystem::CComponent* pSlider;
    float StartValue;
    float EndValue;
    unsigned long _rest[4];
};
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp> struct _Nonconst_traits { };
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp, class _Traits> struct _List_iterator {
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) { }
    _List_node_base* _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator insert(iterator __position, const _Tp& __x);  // 0x0057929A
    void push_front(const _Tp& __x) { insert(begin(), __x); }
    _List_node_base* _M_node;
};
template void list<CSliderAnimator >::push_front(CSliderAnimator const&);
}