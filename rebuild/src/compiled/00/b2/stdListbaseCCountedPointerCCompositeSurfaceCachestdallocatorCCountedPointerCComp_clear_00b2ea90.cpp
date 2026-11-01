// std::_List_base<CCountedPointer<CCompositeSurfaceCache>, std::allocator<CCountedPointer<CCompositeSurfaceCache> > >::clear() -- release every
//   node's counted pointer (count block decrement, DeleteFunc + delete at zero, both words cleared), free the node, re-link the sentinel to itself
// Leaf: ?clear@?$_List_base@V?$CCountedPointer@VCCompositeSurfaceCache@@@@V?$allocator@V?$CCountedPointer@VCCompositeSurfaceCache@@@@@std@@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B2EA90, 89 bytes, thiscall.
// Identity (chunk c681 queue row ?clear@?$_List_base@V?$CCountedPointer@VCCompositeSurfaceCache@@@@...): STLport 4.6 _List_base::clear of the
//   CEngineSurfaceCompositionManager cache list (~CEngineSurfaceCompositionManager 0x00B2D1B0 binds it); the element destructor is the
//   engine/CCountedPointer.h ~CCountedPointer (count block at +4, DeleteFunc slot +4 of the block, operator delete).
// Callees: CCPPointerInfo::DeleteFunc (indirect), operator delete 0x00BFE9BC, free 0x00BFEA14 (allocator<_Node>::deallocate).
// Shape: the real STLport 4.6 _list.c clear() text (the landed 0x00510D40 row), explicitly instantiated for the proven element type.
// Naming wave 18: landed as the descriptive CRetailList_CountedPointer::clear() on a local CCountedPointer / SCountBlock model; re-authored on the
//   STLport instantiation over the shared CCountedPointer header (name-only: the bytes are unchanged).
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

#include "engine/CCountedPointer.h"
class CCompositeSurfaceCache;

template void std::_List_base<CCountedPointer<CCompositeSurfaceCache>, std::allocator<CCountedPointer<CCompositeSurfaceCache> > >::clear();
