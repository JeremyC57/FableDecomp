// NGraphicAppearance::CTCGraphicAppearance::OnKill() -- tell every registered appearance component to clear itself, then destroy all primitives
// Leaf: ?OnKill@CTCGraphicAppearance@NGraphicAppearance@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004BF8F0, 41 bytes, thiscall, tail `jmp` to the out-of-line STLport list clear.
// Identity (proven): the only reference is slot 14 of the NGraphicAppearance::CTCGraphicAppearance vtable 0x0123986C
//   (0x012398A4); CTCBase slot 14 is OnKill (landed CThing::Kill body calls the component's OnKill through the vtable; chunk 702).
//   Ego_r.pdb name set ?OnKill@CTCGraphicAppearance@NGraphicAppearance@@UAEXXZ (donor 0x004C4780, 0x30 bytes, in the block
//   OnKill / AddPersistentPrimitive / RemovePrimitive / ClearAllPrimitivesDueToBeingOutOfRange 0x004C4850 / ClearAllPrimitives
//   0x004C4890 = retail 0x004BF8F0 / 0x004BF920 (RemovePrimitive, landed) / 0x004BF960 / 0x004BF9A0, which inline this same
//   component loop + Primitives.clear()). The catalog label "CTCGraphicAppearance::OnKill" lacked the namespace.
// Callees: the components' slot 1 NGraphicAppearance::CAGraphicAppearanceComponent::OnAppearanceClear() (pure in the base
//   vtable 0x0125A170; CTCLight's override 0x00641210 = Ego_r ?OnAppearanceClear@CTCLight@@UAEXXZ), and
//   std::_List_base<NGraphicAppearance::CPrimitive, std::allocator<...> >::clear 0x004C5690 (STLport 4.6 _list.c, out of line;
//   list::clear() is the inline `_Base::clear()`).
// Layout (retail; Ego_r.pdb with Dinkumware containers in brackets): MeshEffects map +0x0C, TintColours map +0x18,
//   PTCAnimation +0x24 [+0x2C], CachedStaticFocalPosOffset +0x28, Scale +0x34, Primitives list<CPrimitive> +0x38 [+0x40],
//   Components list<CAGraphicAppearanceComponent*> +0x3C [+0x4C] (4-byte STLport lists).
// Shape: the loop re-reads end() (`cmp esi,[edi+3Ch]`) after each virtual call; the `lea ecx,[ecx]` is loop-head alignment.
class CThing;

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator_base() { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp> struct _Nonconst_traits {
    typedef _Tp& reference;
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef typename _Traits::reference reference;
    typedef _List_iterator<_Tp, _Traits> _Self;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) { }
    _List_iterator() { }
    _List_iterator(const _Self& __x) : _List_iterator_base(__x._M_node) { }
    reference operator*() const { return ((_Node*)_M_node)->_M_data; }
    _Self& operator++() { this->_M_incr(); return *this; }
};
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;   // +0x0: the sentinel node
    void clear();                                                 // _list.c, out of line
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
    typedef _List_base<_Tp, _Alloc> _Base;
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    iterator begin() { return iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    iterator end() { return this->_M_node._M_data; }
    void clear() { _Base::clear(); }
};
}

class CTCBase {                                             // engine/CTCBase.h is flattened (vptr as a data member)
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
};

namespace NGraphicAppearance {
class CPrimitive;                                           // Ego_r.pdb, 16 bytes (node value of Primitives)
class CAGraphicAppearanceComponent {                        // vtable 0x0125A170 (slots 0 / 1 pure)
public:
    virtual void OnAppearanceDraw();                        // slot 0 (real signature returns bool; unused here)
    virtual void OnAppearanceClear() = 0;                   // slot 1 (+0x04)
};

class CTCGraphicAppearance : public CTCBase {               // vtable 0x0123986C
public:
    virtual void OnKill();
    unsigned char _pad0c[0x38 - 0x0c];                      // MeshEffects, TintColours, PTCAnimation, CachedStaticFocalPosOffset, Scale
    std::list<CPrimitive>                     Primitives;   // +0x38
    std::list<CAGraphicAppearanceComponent*>  Components;   // +0x3C
};

void CTCGraphicAppearance::OnKill()
{
    for (std::list<CAGraphicAppearanceComponent*>::iterator it = Components.begin(); it != Components.end(); ++it)
        (*it)->OnAppearanceClear();
    Primitives.clear();
}
}