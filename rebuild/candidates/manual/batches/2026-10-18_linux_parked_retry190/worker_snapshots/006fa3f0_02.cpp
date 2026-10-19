// NAnimationEvents::CManager::AddEventToAnim(ulong,const CEvent&) -- append extra event to existing animation, or insert a new sorted animation/event entry; mark saved state dirty.
// Native006FA3F0 length236. Native vector-map16 at+2C; donor20 at+34; CSmallVector8/Event8/Pair12 from PDB.
#define CManager CManagerDonor
#include "engine/CManager.h"
#undef CManager
#include <stddef.h>
void operator delete[](void*) throw(); //00BFEB1C native array delete, not scalar CRT delete
namespace NAnimationEvents {
struct CEvent {unsigned long Index:30;unsigned long ExternallyImported:1;unsigned long StopEvent:1;float Time;CEvent(){}};
struct EventVector {
 CEvent* Start;int Size;
 EventVector():Start(0),Size(0){}
 EventVector(unsigned long n,const CEvent& value):Start(0),Size(0){Allocate(n,value);}
 EventVector(const EventVector& o):Start(0),Size(0){Copy(o);}
 ~EventVector(){delete[]Start;}
 void Allocate(unsigned long,const CEvent&); //006FAFE0 PDBSmallVectorAllocate00720690
 void Copy(const EventVector&); //006FB5C0 PDBSmallVectorCopy007206F0
 void resize(long,const CEvent& = CEvent()); //006FADE0
 void push_back(const CEvent& value){resize(Size+1);Start[Size-1]=value;}
};
struct EventPair {unsigned long first;EventVector second;EventPair(unsigned long k,const EventVector& v):first(k),second(v){}};
struct KeyPairCompareLess {};
void FABLE_FASTCALL SortEventPairs(EventPair*,EventPair*,KeyPairCompareLess); //006FD380 native sort specialization, original library leaf projection
struct EventMap {
 EventPair* First;EventPair* Last;EventPair* Limit;KeyPairCompareLess Compare;bool Dirty;
 EventPair* LowerBound(const unsigned long&); //006FAF20
 void PushBack(const EventPair&); //006FCD90
 EventPair* Find(const unsigned long& key){EventPair* p=LowerBound(key);EventPair* end=Last;if(p==end||key<p->first)p=end;return p;}
 void Insert(unsigned long key,const EventVector& value){{EventPair pair(key,value);PushBack(pair);}Dirty=true;}
 void Sort(){if(Dirty){SortEventPairs(First,Last,Compare);Dirty=false;}}
};
struct CManager {
 unsigned char NativeManagerPrefix[0x2c];EventMap AnimToExtraEventsMap;bool DifferentToSavedCopy;
 void AddEventToAnim(unsigned long,const CEvent&);
};
#pragma auto_inline(off)
EventPair* EventMap::LowerBound(const unsigned long& key){
 EventPair* first=First;long count=Last-first;
 while(count>0){long half=count/2;EventPair* middle=first+half;
  if(middle->first<key){first=middle+1;count=count-half-1;}else count=half;
 }return first;
}
#pragma auto_inline(on)
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CEvent)==8);
FABLE_STATIC_ASSERT(sizeof(EventVector)==8);
FABLE_STATIC_ASSERT(sizeof(EventPair)==12);
FABLE_STATIC_ASSERT(sizeof(EventMap)==16);
FABLE_STATIC_ASSERT(offsetof(CManager,AnimToExtraEventsMap)==0x2c);
FABLE_STATIC_ASSERT(offsetof(CManager,DifferentToSavedCopy)==0x3c);
#endif
void CManager::AddEventToAnim(unsigned long animation,const CEvent& event){
 EventPair* entry=AnimToExtraEventsMap.Find(animation);
 if(entry==AnimToExtraEventsMap.Last){AnimToExtraEventsMap.Insert(animation,EventVector(1,event));AnimToExtraEventsMap.Sort();}
 else entry->second.push_back(event);
 DifferentToSavedCopy=true;
}
}
