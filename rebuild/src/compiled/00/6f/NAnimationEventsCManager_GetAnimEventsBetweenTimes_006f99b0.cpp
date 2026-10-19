// NAnimationEvents::CManager::GetAnimEventsBetweenTimes(unsigned long,CSmallVector<CEvent,32>&,float,float) const -- collect embedded and extra events across animation wrap.
// Leaf: GetAnimEventsBetweenTimes
// Native 0x006f9430 length146. PDB names prove namespace; engine/CManager.h is unrelated GUI manager.
// PDB animation manager map +0x34 uses donor vectors; retail map +0x2c and flags+0x3c.
// LowerBound 0x006faf80 unsigned key comparator verified. Float pool 0x0129ba3c=17b7d138=0.0001f.
#include <math.h>
#include "engine/CMeshDataBank.h"
#include "fable_string.h"
template<class T,int N> struct CSmallVector {T* Start;int Size;};
namespace NAnimationEvents {
struct CEvent { CEvent(); // FableWin PDB023f9630, fixture stub uses same signature
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
struct CManager {
 CManager(const CMeshDataBank&,const CWideString&,bool); // FableWin023fa5c0
 unsigned char EventSymbolMapAndBank[0x2c]; EventMap AnimToExtraEventsMap; bool DifferentToSavedCopy,UseEmbeddedEvents;
 bool GetEmbeddedEventsBetweenTimes(unsigned long,CSmallVector<CEvent,32>&,float,float)const; //006f9820
 bool GetExtraEventsBetweenTimes(unsigned long,CSmallVector<CEvent,32>&,float,float)const; //006f95a0
 bool GetAnimEventsBetweenTimes(unsigned long,CSmallVector<CEvent,32>&,float,float)const;
};
inline const float& MinFloat(const float& a,const float& b){return a<b?a:b;}
bool CManager::GetAnimEventsBetweenTimes(unsigned long key,CSmallVector<CEvent,32>& out,float start,float end)const{
 bool found=false;
 if(end>1.0f){
  float wrap=fmod(end,1.0f);
  found=(UseEmbeddedEvents&&(GetEmbeddedEventsBetweenTimes(key,out,start,1.0f)||GetEmbeddedEventsBetweenTimes(key,out,0.0f,wrap)))||GetExtraEventsBetweenTimes(key,out,start,1.0f)||GetExtraEventsBetweenTimes(key,out,0.0f,wrap);
 }
 end=MinFloat(1.0f,end);
 if(UseEmbeddedEvents)found=found||GetEmbeddedEventsBetweenTimes(key,out,start,end);
 found=found||GetExtraEventsBetweenTimes(key,out,start,end);
 return found;
}
}