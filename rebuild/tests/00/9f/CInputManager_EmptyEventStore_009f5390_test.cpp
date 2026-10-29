#include <stdio.h>
#define PASS9_TEST
// 009f5390 CInputManager::EmptyEventStore
// Leaf: ?EmptyEventStore@CInputManager@@IAEXXZ
// Flags:
class CInputEvent;
namespace std{
 template<class T>class allocator{};
 template<class P,class T,class A>class _STLP_alloc_proxy:public A{public:P _M_data;};
 template<class T,class A>class _Vector_base{public:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
 template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:
  T*begin(){return this->_M_start;}T*end(){return this->_M_finish;}
  T*erase(T*,T*);
  void clear(){erase(begin(),end());}
 };
}
class CBaseClass{public:virtual~CBaseClass();};
class CInputManager:public CBaseClass{public:
 std::vector<CInputEvent>ProcessedEvents,EventStore;long NoStoredEvents;
 // Layout: unread remainder of the original 0x80-byte retail manager is opaque.
 unsigned char unread[0x60];
protected:void EmptyEventStore();
#ifdef PASS9_TEST
 friend int main();
#endif
};
void CInputManager::EmptyEventStore(){ProcessedEvents.clear();EventStore.clear();NoStoredEvents=0;}

class CInputEvent{public:unsigned char bytes[52];};
static void*testReceivers[2];static void*testFirst[2];static void*testLast[2];static int testCalls;
template<class T,class A>T*std::vector<T,A>::erase(T*first,T*last){
 if(testCalls<2){testReceivers[testCalls]=this;testFirst[testCalls]=first;testLast[testCalls]=last;}
 ++testCalls;this->_M_finish=first;return first;
}
CBaseClass::~CBaseClass(){}
int main(){
 CInputManager manager;CInputEvent a[17],b[17];
 for(int x=0;x<17;++x)for(int y=0;y<17;++y){
  manager.ProcessedEvents._M_start=a;manager.ProcessedEvents._M_finish=a+x;manager.ProcessedEvents._M_end_of_storage._M_data=a+17;
  manager.EventStore._M_start=b;manager.EventStore._M_finish=b+y;manager.EventStore._M_end_of_storage._M_data=b+17;
  manager.NoStoredEvents=x-y+159;
  for(int k=0;k<96;++k)manager.unread[k]=(unsigned char)(k^x^y);
  testCalls=0;manager.EmptyEventStore();
  if(testCalls!=2||testReceivers[0]!=&manager.ProcessedEvents||testReceivers[1]!=&manager.EventStore||testFirst[0]!=a||testLast[0]!=a+x||testFirst[1]!=b||testLast[1]!=b+y)return 1;
  if(manager.NoStoredEvents!=0||manager.ProcessedEvents._M_finish!=a||manager.EventStore._M_finish!=b)return 2;
  if(manager.ProcessedEvents._M_start!=a||manager.EventStore._M_start!=b||manager.ProcessedEvents._M_end_of_storage._M_data!=a+17||manager.EventStore._M_end_of_storage._M_data!=b+17)return 3;
  for(int k=0;k<96;++k)if(manager.unread[k]!=(unsigned char)(k^x^y))return 4;
 }
 puts("009f5390_TEST PASS");return 0;
}