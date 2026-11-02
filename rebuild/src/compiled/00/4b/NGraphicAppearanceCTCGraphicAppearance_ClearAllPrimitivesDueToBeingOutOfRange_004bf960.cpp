// NGraphicAppearance::CTCGraphicAppearance::ClearAllPrimitivesDueToBeingOutOfRange() -- when primitives were drawn: clear every component and all primitives, stamp the next frame, clear PrimitivesDrawn
// Leaf: ?ClearAllPrimitivesDueToBeingOutOfRange@CTCGraphicAppearance@NGraphicAppearance@@QAEXXZ
// Naming wave 31 (family c703): the PDB member (was the invented free __fastcall CTCGraphicAppearance_ClearAllPrimitivesDueToBeingOutOfRange
//   over the invented ?SubObj_Clear@@YIXPAUSubObj@@@Z binding of 0x004C5690)
// Retail: Fable.exe (TLC, VC7.1) @ 0x004BF960.
// Identity: Ego_r.pdb / FableWin.pdb ?ClearAllPrimitivesDueToBeingOutOfRange@CTCGraphicAppearance@NGraphicAppearance@@QAEXXZ (block OnKill 0x004BF8F0 / RemovePrimitive 0x004BF920 /
//   ClearAllPrimitivesDueToBeingOutOfRange 0x004BF960 / ClearAllPrimitives 0x004BF9A0, Ego_r donors 0x004C4780..0x004C4890).
// Callees: the components' slot 1 NGraphicAppearance::CAGraphicAppearanceComponent::OnAppearanceClear(), the STLport 4.6
//   std::_List_base<NGraphicAppearance::CPrimitive, std::allocator<...> >::clear 0x004C5690 (list::clear() is the inline
//   `_Base::clear()`), CWorld::GetFrame 0x0049D870 (?GetFrame@CWorld@@SIJXZ, both PDBs: static __fastcall).
// Layout (retail; Ego_r.pdb with Dinkumware containers in brackets): Primitives list<CPrimitive> +0x38 [+0x40], Components
//   list<CAGraphicAppearanceComponent*> +0x3C [+0x4C] (4-byte STLport lists), FrameUpdatedWasRenderedFlag +0x54, PrimitivesDrawn +0x59 bit 7.
// Model: the landed 0x004BF8F0 (OnKill) model (engine/CTCBase.h is flattened; STLport list as in 0x004BF8F0).
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

class CWorld {
public:
    static long __fastcall GetFrame();                      // 0x0049D870
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
    unsigned char _pad0c[0x38 - 0x0c];                      // MeshEffects, TintColours, PTCAnimation, CachedStaticFocalPosOffset, Scale
    std::list<CPrimitive>                     Primitives;   // +0x38
    std::list<CAGraphicAppearanceComponent*>  Components;   // +0x3C
    unsigned char _pad40[0x54 - 0x40];                      // LightingChannel, MainGraphic, ActivePersistenceFlags
    long          FrameUpdatedWasRenderedFlag;              // +0x54
    unsigned char UpdatePrimitiveReasons;                   // +0x58
    unsigned char SelfIlluminating : 1;                     // +0x59 bit 0
    unsigned char ForceResendEveryRenderFrame : 1;          //        bit 1
    unsigned char ResendPrimsEveryRenderFrame : 1;          //        bit 2
    unsigned char DisableOcclusionTesting : 1;              //        bit 3
    unsigned char DrawAfterWater : 1;                       //        bit 4
    unsigned char CachedStaticFocalPos : 1;                 //        bit 5
    unsigned char RenderedLastFrame : 1;                    //        bit 6
    unsigned char PrimitivesDrawn : 1;                      //        bit 7
    void ClearAllPrimitivesDueToBeingOutOfRange();
};

void CTCGraphicAppearance::ClearAllPrimitivesDueToBeingOutOfRange()
{
    if (PrimitivesDrawn) {
        for (std::list<CAGraphicAppearanceComponent*>::iterator it = Components.begin(); it != Components.end(); ++it)
            (*it)->OnAppearanceClear();
        Primitives.clear();
        FrameUpdatedWasRenderedFlag = CWorld::GetFrame() + 1;
        PrimitivesDrawn = false;
    }
}
}
