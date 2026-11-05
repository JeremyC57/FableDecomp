// std::_List_base<CIntelligentPointer<CThingAICreature>, std::allocator<CIntelligentPointer<CThingAICreature> > >::clear() -- destroy every node's value (its virtual destructor), free the node, re-link the sentinel to itself
// Leaf: ?clear@?$_List_base@V?$CIntelligentPointer@VCThingAICreature@@@@V?$allocator@V?$CIntelligentPointer@VCThingAICreature@@@@@std@@@std@@QAEXXZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00510D40, 58 bytes, thiscall.
// Identity (proven, chunk c669): STLport 4.6 _List_base::clear; every caller clears a list<CIntelligentPointer<CThingAICreature> >: ~CTCTrophy 0x006DDB80 and CTCTrophy::OnCreate 0x006DDA20 (Ego_r CTCTrophy witness lists), CTCFollowed::RemoveAllFollowers 0x006D4D40 (Ego_r AIFollowers), CTCBuyableHouse::Evict 0x006C0FD0.
// Callees: the element's virtual destructor (CBaseIntelligentPointer slot 0, the scalar deleting destructor with flag 0 -- an
//   explicit destructor call), free 0x00BFEA14 (allocator<_Node>::deallocate).
// Shape: the real STLport 4.6 _list.c clear() text (the model the landed 0x006DDB80 ~CTCTrophy TU uses), explicitly instantiated for the
//   proven element type; the cursor advances before the old node's value is destroyed, the sentinel is re-read from `this`.
// Naming wave 16: landed as the descriptive CRetailList_00510d40::clear_CRetailList_00510d40; re-authored on the proven STLport
//   instantiation (name-only: the bytes are unchanged).
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14

namespace std {
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
// ---- list (stl/_list.h, stl/_list.c)
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _M_node;
    void clear();
};
template <class _Tp, class _Alloc>
void _List_base<_Tp, _Alloc>::clear()
{
    _List_node<_Tp>* __cur = (_List_node<_Tp>*)this->_M_node._M_data->_M_next;
    while (__cur != this->_M_node._M_data) {
        _List_node<_Tp>* __tmp = __cur;
        __cur = (_List_node<_Tp>*)__cur->_M_next;
        _Destroy(&__tmp->_M_data);
        this->_M_node.deallocate(__tmp, 1);
    }
    this->_M_node._M_data->_M_next = this->_M_node._M_data;
    this->_M_node._M_data->_M_prev = this->_M_node._M_data;
}
} // namespace std

class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CThingAICreature;

template void std::_List_base<CIntelligentPointer<CThingAICreature>, std::allocator<CIntelligentPointer<CThingAICreature> > >::clear();
