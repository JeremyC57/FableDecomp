// std::_List_base::clear() -- (_List_base<CGameScriptThingManager::CScriptThingInfo, allocator<CGameScriptThingManager::CScriptThingInfo> >) STLport 4.6 list clear: destroy and free every node, then link the sentinel to itself
// Leaf: ?clear@?$_List_base@VCScriptThingInfo@CGameScriptThingManager@@V?$allocator@VCScriptThingInfo@CGameScriptThingManager@@@std@@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004BA810, 89 bytes, thiscall, plain `ret`.
// Identity (proven, the catalog label "remove_if<NEntityEvents::CConvFilterToCP<..>>" is propagated and wrong):
//   (1) callers: CGameScriptThingManager::~CGameScriptThingManager 0x004BB600 (landed) calls it on `this` (+0, the
//       ScriptThings list) and the two STLport list destructors 0x004BAE90 / 0x004BB140 (landed: clear, then free the
//       sentinel) call it; the body is the STLport 4.6 _list.c `_List_base<_Tp,_Alloc>::clear()` loop (the same shape as
//       the landed clear 0x00566E10).
//   (2) Ego_r.pdb CGameScriptThingManager {list<CScriptThingInfo> ScriptThings +0; CScriptThing NullThing} and
//       ?clear@?$list@VCScriptThingInfo@CGameScriptThingManager@@...@std@@QAEXXZ (Ego_r 004BE490, FableWin 01C6A940);
//       CScriptThingInfo (Ego_r, 16 bytes) = {ulong64 ThingUID +0; CCountedPointer<CScriptThing> PScriptThing +8}: the
//       node is 0x18 (8-aligned value at +8) and the destroyed counted pointer sits at node+0x10, as the bytes show.
// Callees: the count block's DeleteFunc (`mov ecx,[eax+8]; call [eax+4]`), operator delete 0x00BFE9BC (the block),
//   free 0x00BFEA14 (the node allocator's deallocate). The element's destructor is the implicit ~CScriptThingInfo, i.e.
//   the inline ~CCountedPointer<CScriptThing> (engine/CCountedPointer.h: release, then Data = 0, Info = 0).
#include "engine/CCountedPointer.h"
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
class CScriptThing;
class CGameScriptThingManager {
public:
    class CScriptThingInfo {                              // Ego_r.pdb, 16 bytes
    public:
        unsigned __int64 ThingUID;                        // +0x0
        CCountedPointer<CScriptThing> PScriptThing;       // +0x8
    };
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
    _List_node_base* _M_next;                             // +0x0
    _List_node_base* _M_prev;                             // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                          // +0x8
};
template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    typedef _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _AllocProxy;
    _AllocProxy _M_node;                                  // +0x0: the sentinel node
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
template void _List_base<CGameScriptThingManager::CScriptThingInfo, allocator<CGameScriptThingManager::CScriptThingInfo > >::clear();
}