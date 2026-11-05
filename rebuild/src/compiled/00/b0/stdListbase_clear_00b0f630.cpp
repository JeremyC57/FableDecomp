// std::_List_base::clear() -- (_List_base<CIntelligentPointer<NParticleEngine::CParticleEmitter>, allocator<CIntelligentPointer<NParticleEngine::CParticleEmitter>> >) STLport 4.6 list clear: walk from the first node to the sentinel, destroy each node's element through its virtual destructor and free the node, then link the sentinel to itself
// Leaf: ?clear@?$_List_base@V?$CIntelligentPointer@VCParticleEmitter@NParticleEngine@@@@V?$allocator@V?$CIntelligentPointer@VCParticleEmitter@NParticleEngine@@@@@std@@@std@@QAEXXZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B0F630, 58 bytes (+ int3 padding), thiscall, `ret`.
// Identity: std::_List_base<CIntelligentPointer<NParticleEngine::CParticleEmitter> >::clear() -- proven; evidence: 0x00B0CA90 and 0x00B0F990 (the NParticleEngine::CPSCUpdateNormal destructor and its deleting form: they store the RTTI vtable 0x0129FC1C) call it on +0x9C = Ego_r.pdb CPSCUpdateNormal::DecalEmitterList (+0x9C, list<CIntelligentPointer<NParticleEngine::CParticleEmitter> >), as do the unreferenced list destructors 0x00B0F790 / 0x00B0F810; catalogue "CExclusiveAssert::~CExclusiveAssert" (0x00B0CA90) is propagated; the PDB name sets hold list<CIntelligentPointer<NParticleEngine::CParticleEmitter> >::clear
//   Family G073 (seven byte-identical bodies, one COMDAT per T: retail has no ICF); the manifest label "SortTreeRecursively" is
//   propagated across the identical bodies (the landed sibling 0x00882C40 is the same body for a third element type).
// Callees: the element's virtual destructor (vtable slot 0, pushed 0 = destroy only, no delete), free 0x00BFEA14 (the node
//   allocator's deallocate: no null test).
// Body: STLport 4.6 list.c `_List_base<_Tp, _Alloc>::clear()` -- the out-of-line member that list::clear() and
//   ~_List_base() call (the unreferenced list destructors beside each instance call it, then free the sentinel); the
//   sentinel is re-read from `this` after every free.
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14

namespace NParticleEngine { class CParticleEmitter; }
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

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

typedef CIntelligentPointer<NParticleEngine::CParticleEmitter> _Elem;
template void _List_base<_Elem, allocator<_Elem> >::clear();

} // namespace std