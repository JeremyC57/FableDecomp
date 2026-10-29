#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
#define private public
// 006f95a0 NAnimationEvents::CManager::GetExtraEventsBetweenTimes
// Leaf: ?GetExtraEventsBetweenTimes@CManager@NAnimationEvents@@ABE_NKAAV?$CSmallVector@VCEvent@NAnimationEvents@@$0CA@@@MM@Z
// Flags:
// Namespace CManager has no shared header; engine/CManager.h is NUISystem's unrelated type.
// Layout: CSymbolMap restores CBaseClassNonCopyable; unread32byte payload is a projection. CManager has no bases, and slot0 is GetEvents (PDB64145, RTTI2112).
class CCharString;                                        // 4-byte string handle (only passed by reference here)
class CBaseClass{public:virtual ~CBaseClass();};
class CBaseClassNonCopyable:public CBaseClass{protected:virtual ~CBaseClassNonCopyable();};
class CSymbolMap:public CBaseClassNonCopyable {                                        // retail 0x24 bytes
public:
    unsigned char unobservedSymbolMap[0x20];
    virtual ~CSymbolMap();
    long GetSymbolSLONG(const CCharString& name) const;              // 0x009E5120
};

namespace std {
template<class T> class allocator { public: allocator() {} };
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
template<class T,class A>class _Vector_base{public:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{};
template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
struct input_iterator_tag {  };
struct forward_iterator_tag : public input_iterator_tag {  };
struct bidirectional_iterator_tag : public forward_iterator_tag {  };
struct random_access_iterator_tag : public bidirectional_iterator_tag {  };
}

namespace NAnimationEvents {
class CEvent {                                            // Ego_r.pdb: 8 bytes
public:
    unsigned long Index : 30;                             // +0x00
    unsigned long ExternallyImported : 1;
    unsigned long StopEvent : 1;
    float Time;                                           // +0x04
};
}

template<class T, int N> class CSmallVector {             // Ego_r.pdb: 8 bytes
public:
    T* Start;                                             // +0x00
    int Size;                                             // +0x04
    const T* begin() const { return Start; }
    const T* end() const { return Start + Size; }
};

template<class T>class CArray:public std::vector<T>{};

template<class K, class V> class CKeyPairCompareLess { public: CKeyPairCompareLess() {} };

template<class K, class V, class Comp = CKeyPairCompareLess<K, V> > class CVectorMap : public CArray<std::pair<K,V> > {
public:
    typedef std::pair<K, V> value_type;
    typedef const value_type* const_iterator;
    Comp Compare;                                         // +0x0c
    bool Dirty;                                           // +0x0d
    const_iterator LowerBound(const K& key) const;        // 0x006FAF80
    const_iterator Find(const K& key) const
    {
        const_iterator it = LowerBound(key);
        if (it == this->_M_finish || key < it->first)
            it = this->_M_finish;
        return it;
    }
};

#pragma auto_inline(off)
template<>
__declspec(noinline) CVectorMap<unsigned long,CSmallVector<NAnimationEvents::CEvent,32> >::const_iterator
CVectorMap<unsigned long,CSmallVector<NAnimationEvents::CEvent,32> >::LowerBound(const unsigned long& key) const
{
    long count=this->_M_finish-this->_M_start;
    const value_type* first=this->_M_start;
    while(count>0) {
        long half=count/2;
        const value_type* middle=first+half;
        if(middle->first<key) { first=middle+1; count=count-half-1; }
        else count=half;
    }
    return first;
}
#pragma auto_inline(on)
class CMeshDataBank;

namespace NAnimationEvents {
class CIsEventIndexEqualTo {                              // Ego_r.pdb: 4 bytes
public:
    unsigned long Index;                                  // +0x00
    CIsEventIndexEqualTo(unsigned long index) : Index(index) {}
    bool operator()(const CEvent& event) const { return event.Index == Index; }
};
class CManager {
public:
    virtual void GetEvents(unsigned long,std::vector<std::pair<CCharString,float> >&)const;
    CMeshDataBank* MeshDataBank;
    CSymbolMap EventSymbolMap;
    CVectorMap<unsigned long, CSmallVector<CEvent, 32> > AnimToExtraEventsMap;
    bool DifferentToSavedCopy;
    bool UseEmbeddedEvents;
private:
    const CSmallVector<CEvent, 32>* GetPExtraAnimEvents(unsigned long anim) const
    {
        const CSmallVector<CEvent,32>* result=0;
        CVectorMap<unsigned long,CSmallVector<CEvent,32> >::const_iterator it=AnimToExtraEventsMap.Find(anim);
        if(it!=AnimToExtraEventsMap._M_finish) result=&it->second;
        return result;
    }
    bool GetExtraEventsBetweenTimes(unsigned long,CSmallVector<CEvent,32>&,float,float)const;
};
}

namespace NAnimationEvents {
class CGetEventsFilteredByTime {
public:
    float StartTime,EndTime;
    CSmallVector<CEvent,32>* OutEvents;
    CGetEventsFilteredByTime(float start,float end,CSmallVector<CEvent,32>& events):StartTime(start),EndTime(end),OutEvents(&events) { }
};
}
namespace std {
template<class I,class P> P __fastcall for_each(I,I,P);
}
namespace NAnimationEvents {
bool CManager::GetExtraEventsBetweenTimes(unsigned long animation,CSmallVector<CEvent,32>& events,float start,float end) const
{
    int oldCount=events.Size;
    const CSmallVector<CEvent,32>* list=GetPExtraAnimEvents(animation);
    if(list && list->Size) {
        CGetEventsFilteredByTime collector(start,end,events);
        std::for_each(list->begin(),list->end(),collector);
    }
    return events.Size>oldCount;
}
}

#undef private
class CCharStringData;class CCharString{public:CCharStringData*PStringData;};
CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CSymbolMap::~CSymbolMap(){}
static unsigned int fixtureCalls=0,fixtureErrors=0;
static const NAnimationEvents::CEvent*fixtureFirst;
static const NAnimationEvents::CEvent*fixtureLast;
static CSmallVector<NAnimationEvents::CEvent,32>*fixtureOut;
static float fixtureStart,fixtureEnd;
void NAnimationEvents::CManager::GetEvents(unsigned long,std::vector<std::pair<CCharString,float> >&)const{++fixtureErrors;}
namespace std{
template<class I,class P>P __fastcall for_each(I first,I last,P pred){
    ++fixtureCalls;
    if(first!=fixtureFirst||last!=fixtureLast||pred.OutEvents!=fixtureOut||pred.StartTime!=fixtureStart||pred.EndTime!=fixtureEnd)++fixtureErrors;
    for(;first!=last;++first){if(first->Time>=pred.StartTime&&first->Time<pred.EndTime){if(pred.OutEvents->Size>=64){++fixtureErrors;break;}pred.OutEvents->Start[pred.OutEvents->Size]=*first;++pred.OutEvents->Size;}}
    return pred;
}
}
int main(){
    NAnimationEvents::CManager manager;
    typedef NAnimationEvents::CEvent Event;typedef CSmallVector<Event,32>Events;typedef std::pair<unsigned long,Events>Mapping;
    Mapping map[3];Event input[3][12],before[3][12],output[64],expected[64];memset(input,0,sizeof(input));
    for(int j=0;j<3;++j){map[j].first=(unsigned long)(7+14*j);map[j].second.Start=input[j];for(int i=0;i<12;++i){input[j][i].Index=(unsigned long)(100*j+i);input[j][i].ExternallyImported=i&1;input[j][i].StopEvent=(i>>1)&1;input[j][i].Time=(float)(i/2)+(float)j/4.0f;}}
    memcpy(before,input,sizeof(input));manager.AnimToExtraEventsMap._M_start=map;manager.AnimToExtraEventsMap._M_finish=map+3;manager.AnimToExtraEventsMap._M_end_of_storage._M_data=map+3;
    const int widths[4]={-1,0,1,3};unsigned int cases=0;
    for(int n=0;n<=12;++n){for(int j=0;j<3;++j)map[j].second.Size=n;
        for(int key=0;key<4;++key)for(int b=-1;b<=13;++b)for(int wi=0;wi<4;++wi)for(int prefix=0;prefix<=3;prefix+=3){
            memset(output,0,sizeof(output));memset(expected,0,sizeof(expected));
            for(int i=0;i<prefix;++i){output[i].Index=999;output[i].Time=-16.0f;expected[i]=output[i];}
            Events out;out.Start=output;out.Size=prefix;int count=prefix;
            float start=(float)b/2.0f,end=start+(float)widths[wi]/2.0f;
            if(key<3)for(int i=0;i<n;++i)if(!(input[key][i].Time<start)&&input[key][i].Time<end)expected[count++]=input[key][i];
            fixtureFirst=key<3?input[key]:0;fixtureLast=key<3?input[key]+n:0;fixtureOut=&out;fixtureStart=start;fixtureEnd=end;
            unsigned int calls=fixtureCalls;
            bool got=manager.GetExtraEventsBetweenTimes(key<3?map[key].first:999,out,start,end);
            if(got!=(count>prefix)||out.Size!=count||out.Start!=output||fixtureErrors)return 1;
            if(fixtureCalls!=calls+(key<3&&n>0?1:0))return 2;
            if(memcmp(output,expected,(unsigned int)count*sizeof(Event))||memcmp(before,input,sizeof(input)))return 3;
            ++cases;
        }
    }
    if(cases!=6240)return 4;
    puts("006f95a0_TEST PASS");return 0;
}