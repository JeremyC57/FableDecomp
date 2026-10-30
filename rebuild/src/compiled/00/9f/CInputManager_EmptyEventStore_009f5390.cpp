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
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
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