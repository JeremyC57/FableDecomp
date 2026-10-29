#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#ifndef _MSC_VER
#define __cdecl
#endif
// 009bf5a0 CDisplayManager::RemoveDeviceResetCallback
// Flags:
// Leaf: ?RemoveDeviceResetCallback@CDisplayManager@@QAEXPAVCDeviceResetCallback@@@Z
#include <stddef.h>
#ifndef _MSC_VER
#define __forceinline inline
#endif
extern "C" void __cdecl free(void*);
class CBaseClass{public:virtual~CBaseClass();};
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

CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
class CDeviceResetCallback{};
int main(){
 typedef std::_List_node<CDeviceResetCallback*>Node;
 CDeviceResetCallback callbacks[4];unsigned int cases=0;
 for(int n=0;n<=20;++n)for(int pattern=0;pattern<3;++pattern)for(int target=0;target<6;++target){
  CDisplayManager manager;memset(reinterpret_cast<unsigned char*>(&manager)+4,0xA5,sizeof(manager)-4);
  Node*head=new(malloc(sizeof(Node)))Node;Node*nodes[20];CDeviceResetCallback*values[20];
  head->_M_data=0;
  for(int i=0;i<n;++i){
   nodes[i]=new(malloc(sizeof(Node)))Node;
   values[i]=pattern==0?&callbacks[i%4]:pattern==1?&callbacks[0]:(i%2?&callbacks[i%4]:0);
   nodes[i]->_M_data=values[i];
  }
  head->_M_next=n?nodes[0]:head;head->_M_prev=n?nodes[n-1]:head;
  for(int i=0;i<n;++i){nodes[i]->_M_prev=i?nodes[i-1]:head;nodes[i]->_M_next=i+1<n?nodes[i+1]:head;}
  manager.DeviceResetCallbacks._M_node._M_data=head;
  unsigned char before[sizeof(manager)];memcpy(before,&manager,sizeof(manager));
  CDeviceResetCallback*needle=target<4?&callbacks[target]:target==4?0:reinterpret_cast<CDeviceResetCallback*>(head);
  int removed=-1;for(int i=0;i<n;++i)if(values[i]==needle){removed=i;break;}
  manager.RemoveDeviceResetCallback(needle);
  if(memcmp(before,&manager,sizeof(manager)))return 1;
  std::_List_node_base*prev=head;Node*walk=static_cast<Node*>(head->_M_next);
  for(int i=0;i<n;++i)if(i!=removed){
   if(walk!=nodes[i]||walk->_M_prev!=prev||walk->_M_data!=values[i])return 2;
   prev=walk;walk=static_cast<Node*>(walk->_M_next);
  }
  if(walk!=head||head->_M_prev!=prev||prev->_M_next!=head||head->_M_data)return 3;
  for(int i=0;i<n;++i)if(i!=removed){nodes[i]->~Node();free(nodes[i]);}
  head->~Node();free(head);++cases;
 }
 if(cases!=378)return 4;puts("009bf5a0_TEST PASS");return 0;
}