// 009bf5a0 CDisplayManager::RemoveDeviceResetCallback
// Flags:
// Leaf: ?RemoveDeviceResetCallback@CDisplayManager@@QAEXPAVCDeviceResetCallback@@@Z
#include <stddef.h>
#ifndef _MSC_VER
#define __forceinline inline
#endif
extern "C" void __cdecl free(void*);
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CBaseClassNonCopyable:public CBaseClass{protected:virtual~CBaseClassNonCopyable();};
class CDeviceResetCallback;
namespace std{
template<class T>class allocator{};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
struct _List_node_base{_List_node_base*_M_next,*_M_prev;};
template<class T>struct _List_node:public _List_node_base{T _M_data;};
struct _List_iterator_base{
 _List_node_base*_M_node;
 __forceinline _List_iterator_base(_List_node_base*p):_M_node(p){}
 __forceinline void _M_incr(){_M_node=_M_node->_M_next;}
 __forceinline bool operator==(const _List_iterator_base&x)const{return _M_node==x._M_node;}
 __forceinline bool operator!=(const _List_iterator_base&x)const{return _M_node!=x._M_node;}
};
template<class T>struct _Nonconst_traits{typedef T*pointer;typedef T&reference;};
template<class T,class Traits>struct _List_iterator:public _List_iterator_base{
 typedef _List_iterator<T,_Nonconst_traits<T> >iterator;
 __forceinline _List_iterator(_List_node<T>*p):_List_iterator_base(p){}
 __forceinline _List_iterator(const iterator&x):_List_iterator_base(x._M_node){}
 __forceinline T&operator*()const{return static_cast<_List_node<T>*>(this->_M_node)->_M_data;}
 __forceinline _List_iterator&operator++(){this->_M_incr();return *this;}
};
template<class T,class A>class _List_base{public:_STLP_alloc_proxy<_List_node<T>*,_List_node<T>,allocator<_List_node<T> > >_M_node;};
template<class T,class A=allocator<T> >class list:public _List_base<T,A>{public:
 typedef _List_iterator<T,_Nonconst_traits<T> >iterator;
 __forceinline iterator begin(){return iterator(static_cast<_List_node<T>*>(this->_M_node._M_data->_M_next));}
 __forceinline iterator end(){return iterator(this->_M_node._M_data);}
 __forceinline iterator erase(iterator position){
  _List_node_base*next=position._M_node->_M_next;
  _List_node_base*prev=position._M_node->_M_prev;
  _List_node<T>*node=static_cast<_List_node<T>*>(position._M_node);
  prev->_M_next=next;next->_M_prev=prev;
  node->_M_data.~T();free(node);
  return iterator(static_cast<_List_node<T>*>(next));
 }
};
template<class I,class T>__forceinline I find(I first,I last,const T&value){while(first!=last&&!(*first==value))++first;return first;}
}
// Layout: RTTI CDisplayManager:CBaseClassNonCopyable:CBaseClass at0.
// Original DeviceResetCallbacks follows unread608bytes afterbase4, at612.
class CDisplayManager:public CBaseClassNonCopyable{unsigned char pass9_unread_prefix[608];public:
 std::list<CDeviceResetCallback*>DeviceResetCallbacks;
 void RemoveDeviceResetCallback(CDeviceResetCallback*);
};
void CDisplayManager::RemoveDeviceResetCallback(CDeviceResetCallback*callback){
 std::list<CDeviceResetCallback*>::iterator found=std::find(DeviceResetCallbacks.begin(),DeviceResetCallbacks.end(),callback);
 if(found!=DeviceResetCallbacks.end())DeviceResetCallbacks.erase(found);
}