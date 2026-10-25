// std::_List_base::clear() -- (_List_base<CEnginePrimitiveHandle, allocator<CEnginePrimitiveHandle> >) STLport 4.6 list clear: walk from the first node to the sentinel, destroy each node's element through its virtual destructor and free the node, then link the sentinel to itself
// Leaf: ?clear@?$_List_base@VCEnginePrimitiveHandle@@V?$allocator@VCEnginePrimitiveHandle@@@std@@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00834F90, 58 bytes (+ int3 padding), thiscall, `ret`.
// Identity: std::_List_base<CEnginePrimitiveHandle >::clear() -- proven; evidence: the landed CEngineInternalPrimitiveParticleGroup::ClearAllChildPrimitives 0x00BBD200 calls it on Primitives (+0x48 = Ego_r.pdb +0x48, list<CEnginePrimitiveHandle>), as do 0x00BBA140 / 0x00BBA1E0 (the same +0x48 list) and 0x00832190 / 0x00833C00 on +0x194 = Ego_r.pdb CThingAICreature::DebugRenderHandles (+0x1BC, list<CEnginePrimitiveHandle>), and the unreferenced list destructors 0x00835120 / 0x00835390; the PDB name sets hold list<CEnginePrimitiveHandle>::clear
//   Family G073 (seven byte-identical bodies, one COMDAT per T: retail has no ICF); the manifest label "SortTreeRecursively" is
//   propagated across the identical bodies (the landed sibling 0x00882C40 is the same body for a third element type).
// Callees: the element's virtual destructor (vtable slot 0, pushed 0 = destroy only, no delete), free 0x00BFEA14 (the node
//   allocator's deallocate: no null test).
// Body: STLport 4.6 list.c `_List_base<_Tp, _Alloc>::clear()` -- the out-of-line member that list::clear() and
//   ~_List_base() call (the unreferenced list destructors beside each instance call it, then free the sentinel); the
//   sentinel is re-read from `this` after every free.
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14

class CBaseClass {                                            // vtable 0x01231710
public:
    virtual ~CBaseClass();
};
class CEnginePrimitiveHandle : public CBaseClass {           // vtable 0x0122F598, 8 bytes
public:
    void* Data;                                              // +0x04 CIVCountedPointer
    virtual ~CEnginePrimitiveHandle();
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

typedef CEnginePrimitiveHandle _Elem;
template void _List_base<_Elem, allocator<_Elem> >::clear();

} // namespace std