#ifndef FABLE_STLPORT_NUISYSTEM_LIST_OWNERS_H
#define FABLE_STLPORT_NUISYSTEM_LIST_OWNERS_H

#include "stlport/fable_containers.h"
#if defined(FABLE_RETAIL_STLP_NUISYSTEM_LIST_SIZE_OWNERS_PASS27) && defined(_MSC_VER)
namespace NUISystem { class CActionParam; class CComponent; }
namespace std {

#pragma optimize("s", on)

typedef _List_base<NUISystem::CActionParam*, allocator<NUISystem::CActionParam*> > UIActionParamListBase;
typedef _List_base<long, allocator<long> > UIThingActionParamListBase;
typedef _List_base<NUISystem::CComponent*, allocator<NUISystem::CComponent*> > UIComponentListBase;
typedef list<NUISystem::CActionParam*> UIActionParamList;
typedef list<long> UIThingActionParamList;
typedef list<NUISystem::CComponent*> UIComponentList;

template<> __declspec(noinline) inline void UIActionParamListBase::clear()
{
    _List_node<NUISystem::CActionParam*>* cur = static_cast<_List_node<NUISystem::CActionParam*>*>(_M_node._M_data->_M_next);
    while (cur != _M_node._M_data) {
        _List_node<NUISystem::CActionParam*>* tmp = cur;
        cur = static_cast<_List_node<NUISystem::CActionParam*>*>(cur->_M_next);
        _Destroy(&tmp->_M_data);
        _M_node.deallocate(tmp, 1);
    }
    _M_node._M_data->_M_next = _M_node._M_data;
    _M_node._M_data->_M_prev = _M_node._M_data;
}

template<> inline UIActionParamListBase::_List_base(const allocator<NUISystem::CActionParam*>&)
    : _M_node(allocator<_List_node<NUISystem::CActionParam*> >(), 0)
{
    _List_node<NUISystem::CActionParam*>* node = _M_node.allocate(1);
    node->_M_next = node;
    node->_M_prev = node;
    _M_node._M_data = node;
}

template<> __declspec(noinline) inline UIActionParamListBase::~_List_base()
{
    clear();
    _M_node.deallocate(_M_node._M_data, 1);
}

template<> __declspec(noinline) inline void UIThingActionParamListBase::clear()
{
    _List_node<long>* cur = static_cast<_List_node<long>*>(_M_node._M_data->_M_next);
    while (cur != _M_node._M_data) {
        _List_node<long>* tmp = cur;
        cur = static_cast<_List_node<long>*>(cur->_M_next);
        _Destroy(&tmp->_M_data);
        _M_node.deallocate(tmp, 1);
    }
    _M_node._M_data->_M_next = _M_node._M_data;
    _M_node._M_data->_M_prev = _M_node._M_data;
}

template<> inline UIThingActionParamListBase::_List_base(const allocator<long>&)
    : _M_node(allocator<_List_node<long> >(), 0)
{
    _List_node<long>* node = _M_node.allocate(1);
    node->_M_next = node;
    node->_M_prev = node;
    _M_node._M_data = node;
}

template<> __declspec(noinline) inline UIThingActionParamListBase::~_List_base()
{
    clear();
    _M_node.deallocate(_M_node._M_data, 1);
}

template<> __declspec(noinline) inline void UIComponentListBase::clear()
{
    _List_node<NUISystem::CComponent*>* cur = static_cast<_List_node<NUISystem::CComponent*>*>(_M_node._M_data->_M_next);
    while (cur != _M_node._M_data) {
        _List_node<NUISystem::CComponent*>* tmp = cur;
        cur = static_cast<_List_node<NUISystem::CComponent*>*>(cur->_M_next);
        _Destroy(&tmp->_M_data);
        _M_node.deallocate(tmp, 1);
    }
    _M_node._M_data->_M_next = _M_node._M_data;
    _M_node._M_data->_M_prev = _M_node._M_data;
}

template<> inline UIComponentListBase::_List_base(const allocator<NUISystem::CComponent*>&)
    : _M_node(allocator<_List_node<NUISystem::CComponent*> >(), 0)
{
    _List_node<NUISystem::CComponent*>* node = _M_node.allocate(1);
    node->_M_next = node;
    node->_M_prev = node;
    _M_node._M_data = node;
}

template<> __declspec(noinline) inline UIComponentListBase::~_List_base()
{
    clear();
    _M_node.deallocate(_M_node._M_data, 1);
}

template<> inline _List_node<NUISystem::CComponent*>* UIComponentList::_M_create_node(NUISystem::CComponent* const& x)
{
    _List_node<NUISystem::CComponent*>* p = this->_M_node.allocate(1);
    new(static_cast<void*>(&p->_M_data)) NUISystem::CComponent*(x);
    return p;
}

template<> inline UIComponentList::iterator UIComponentList::insert(iterator position, NUISystem::CComponent* const& x)
{
    _List_node<NUISystem::CComponent*>* tmp = _M_create_node(x);
    _List_node_base* n = position._M_node;
    _List_node_base* p = n->_M_prev;
    tmp->_M_next = n;
    tmp->_M_prev = p;
    p->_M_next = tmp;
    n->_M_prev = tmp;
    return iterator(tmp);
}
template<> inline UIActionParamList::list(const allocator<NUISystem::CActionParam*>& a) : UIActionParamListBase(a) { }
template<> inline UIActionParamList::~list() { }
template<> inline UIThingActionParamList::list(const allocator<long>& a) : UIThingActionParamListBase(a) { }
template<> inline UIThingActionParamList::~list() { }
template<> inline UIComponentList::list(const allocator<NUISystem::CComponent*>& a) : UIComponentListBase(a) { }
template<> inline UIComponentList::~list() { }
#pragma optimize("y", off)
template<> template<>  inline void UIComponentList::_M_insert_dispatch<UIComponentList::const_iterator>(
    iterator position, const_iterator first, const_iterator last, const __false_type&)
{
    for (; first != last; ++first)
        insert(position, *first);
}

template<> template<>  inline void UIComponentList::insert<UIComponentList::const_iterator>(
    iterator position, const_iterator first, const_iterator last)
{
    _M_insert_dispatch(position, first, last, __false_type());
}
template<> inline UIComponentList::list(const UIComponentList& x) : UIComponentListBase(x.get_allocator())
{
    insert(begin(), x.begin(), x.end());
}

#pragma optimize("", on)
}
#endif
#endif
