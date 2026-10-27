// std::_List_base<CIntelligentPointer<CThingBuilding>, std::allocator<CIntelligentPointer<CThingBuilding> > >::clear() -- STLport list clear: destroy every node's intelligent pointer (virtual destructor), free the node, leave the sentinel self-linked
// Leaf: ?clear@?$_List_base@V?$CIntelligentPointer@VCThingBuilding@@@@V?$allocator@V?$CIntelligentPointer@VCThingBuilding@@@@@std@@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065e4d1, 50 bytes, thiscall, plain `ret`; size-optimised TU (unaligned address).
// Identity (proven): its only caller, the list destructor stub 0x0065F1C8 (`clear(); if (node) free(node)`), is called by
//   ~CTCVillage 0x0065B598 on this+0x28 = Ego_r.pdb CTCVillage::VillageBuildings (list<CIntelligentPointer<CThingBuilding> >; retail lists are
//   4 bytes, the 16-byte container before VillageMarkers a 12-byte vector).
// Callees: the element's virtual destructor (vtable slot 0, flags 0), free 0x00BFEA14.
// Shape (family L026, model the landed _List_base::clear 0x00B0F630): STLport 4.6 _list.c clear() verbatim under
//   `#pragma optimize("s", on)` (the sentinel reloaded for every loop test).
#pragma optimize("s", on)
class CThingBuilding;
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14

class CBaseObjectPointer;
class CBaseIntelligentPointer {                          // 8 bytes (Ego_r.pdb): vptr, PData
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    virtual ~CIntelligentPointer();
};

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

struct _List_node_base {
    _List_node_base* _M_next;                              // +0x0
    _List_node_base* _M_prev;                              // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                           // +0x8
};

template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    typedef _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _AllocProxy;
    _AllocProxy _M_node;                                   // +0x0: the sentinel node
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
}
template class std::_List_base<CIntelligentPointer<CThingBuilding>, std::allocator<CIntelligentPointer<CThingBuilding> > >;