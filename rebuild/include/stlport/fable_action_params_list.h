#ifndef FABLE_STLPORT_ACTION_PARAMS_LIST_H
#define FABLE_STLPORT_ACTION_PARAMS_LIST_H
#include "stlport/fable_containers.h"

#if defined(FABLE_RETAIL_STLP_ACTION_PARAM_LIST_SIZE_OWNERS_PASS26) && defined(_MSC_VER)
namespace NUISystem { class CActionParam; }
namespace std {
// Fidelity: scaffolding, not retail. 00429FBE and 0042AADC use the size-mode STLport list implementation.
// 0042B646 sets size mode and retains a frame pointer. A row-head setting follows this include.
#pragma optimize("s", on)
typedef list<NUISystem::CActionParam*> CActionParamPointerList;
#if defined(FABLE_RETAIL_STLP_ACTION_PARAM_LIST_ALLOCATE_INLINE_PASS26) && defined(FABLE_RETAIL_STLP_ALLOC_PROXY_REBIND)
// Inline-budget: the native 00429FBE node creator expands both allocator layers.
typedef _List_node<NUISystem::CActionParam*> CActionParamListNode;
template<>
FABLE_FORCEINLINE CActionParamListNode* allocator<CActionParamListNode>::allocate(size_t n, const void*)
{
    return n ? static_cast<CActionParamListNode*>(malloc(n * sizeof(CActionParamListNode))) : 0;
}

template<>
FABLE_FORCEINLINE CActionParamListNode* _STLP_alloc_proxy<CActionParamListNode*, CActionParamListNode, allocator<CActionParamListNode> >::allocate(size_t n)
{
    return __stl_alloc_rebind(static_cast<allocator<CActionParamListNode>&>(*this), static_cast<CActionParamListNode*>(0)).allocate(n, 0);
}
#endif // FABLE_RETAIL_STLP_ACTION_PARAM_LIST_ALLOCATE_INLINE_PASS26

template<>
inline _List_node<NUISystem::CActionParam*>* CActionParamPointerList::_M_create_node(NUISystem::CActionParam* const& x)
{
    _List_node<NUISystem::CActionParam*>* p = this->_M_node.allocate(1);
    new(static_cast<void*>(&p->_M_data)) NUISystem::CActionParam*(x);
    return p;
}

template<>
inline CActionParamPointerList::iterator CActionParamPointerList::insert(iterator position, NUISystem::CActionParam* const& x)
{
    _List_node<NUISystem::CActionParam*>* tmp = _M_create_node(x);
    _List_node_base* n = position._M_node;
    _List_node_base* p = n->_M_prev;
    tmp->_M_next = n;
    tmp->_M_prev = p;
    p->_M_next = tmp;
    n->_M_prev = tmp;
    return iterator(tmp);
}

#pragma optimize("y", off)
template<>
inline void CActionParamPointerList::push_back(NUISystem::CActionParam* const& x)
{
    insert(end(), x);
}
#pragma optimize("", on)
}
#endif // FABLE_RETAIL_STLP_ACTION_PARAM_LIST_SIZE_OWNERS_PASS26
#endif
