// std::_List_base<CIntelligentPointer<C3DAnimation2>, std::allocator<CIntelligentPointer<C3DAnimation2> > >::clear() -- destroy every node's value (its virtual destructor), free the node, re-link the sentinel to itself
// Leaf: ?clear@?$_List_base@V?$CIntelligentPointer@VC3DAnimation2@@@@V?$allocator@V?$CIntelligentPointer@VC3DAnimation2@@@@@std@@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A53300, 58 bytes, thiscall.
// Identity (proven, chunk c672): called by ~C3DAnimation2 0x00A4EEC0 on InstanceList (Ego_r list<CIntelligentPointer<C3DAnimation2> > at +0x78); STLport _list.c clear body; its only other callers 0x00A54133 / 0x00A544C3 sit in unreferenced ~list bodies of the same TU.
// Callees: the element's virtual destructor (CBaseIntelligentPointer slot 0, the scalar deleting destructor with flag 0 -- an
//   explicit destructor call), free 0x00BFEA14 (allocator<_Node>::deallocate).
// Shape: the real STLport 4.6 _list.c clear() text (the model the landed 0x00A4EEC0 ~C3DAnimation2 TU uses), explicitly instantiated for the
//   proven element type; the cursor advances before the old node's value is destroyed, the sentinel is re-read from `this`.
// Naming wave 16: landed as the descriptive CRetailList_00a53300::clear_CRetailList_00a53300; re-authored on the proven STLport
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
class CBaseIntelligentPointer {                                          // vtable 0x0129C95C, 8 bytes
public:
    CBaseIntelligentPointer();                                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                                  // 0x00A01C10
private:
    CBaseObjectPointer* PData;                                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
};
class C3DAnimation2;

template void std::_List_base<CIntelligentPointer<C3DAnimation2>, std::allocator<CIntelligentPointer<C3DAnimation2> > >::clear();
