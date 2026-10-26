// NAnimationEvents::CManager::EventExists(long,const CEvent&) const -- find same index, stop flag and nearby time.
// Native 0x006f9430 length146. PDB names prove namespace; engine/CManager.h is unrelated GUI manager.
// PDB animation manager map +0x34 uses donor vectors; retail map +0x2c and flags+0x3c.
// LowerBound 0x006faf80 unsigned key comparator verified. Float pool 0x0129ba3c=17b7d138=0.0001f.
#include <math.h>
#include "engine/CMeshDataBank.h"
#include "fable_string.h"
namespace NAnimationEvents {
class CEvent { public: CEvent(); // FableWin PDB023f9630, fixture stub uses same signature
 unsigned int Index:30; unsigned int ExternallyImported:1; unsigned int StopEvent:1; float Time;
 bool operator==(const CEvent& b) const { return Index==b.Index && StopEvent==b.StopEvent && fabs(Time-b.Time)<0.0001f; }
};
struct Entry { unsigned long Key; CEvent* Start; unsigned long Size; };
struct EventMap {
 Entry *First,*Last,*EndStorage; unsigned char Compare,Dirty;
 typedef const Entry* const_iterator;
 const_iterator LowerBound(const unsigned long&) const; // 0x006faf80
 const_iterator Find(unsigned long key) const { unsigned long lookupKey=key; const_iterator i=LowerBound(lookupKey); if(i==Last || key<i->Key) i=Last; return i; }
};
class CManager { public:
 CManager(const CMeshDataBank&,const CWideString&,bool); // FableWin023fa5c0
 unsigned char EventSymbolMapAndBank[0x2c]; EventMap AnimToExtraEventsMap; bool DifferentToSavedCopy,UseEmbeddedEvents;
 bool EventExists(long,const CEvent&) const;
};
bool CManager::EventExists(long key,const CEvent& event) const {
 const Entry* i=AnimToExtraEventsMap.Find((unsigned long)key);
 if(i!=AnimToExtraEventsMap.Last) {
  CEvent* end=i->Start+i->Size;
  for(CEvent* e=i->Start;e!=end;++e) if(*e==event) return true;
 }
 return false;
}
}