#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
#define private public
// 006f96c0 NAnimationEvents::CManager::GetExtraEventTime
// Leaf: ?GetExtraEventTime@CManager@NAnimationEvents@@ABEMKABVCCharString@@@Z
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
    float GetExtraEventTime(unsigned long anim,const CCharString& name) const;
};
}

namespace std {
#pragma auto_inline(off)
// 0x006F8EA0 (separate function; same-TU body so the empty tag can share a live slot, GOTCHAS 24l)
template<class RandomAccessIter, class Predicate>
__declspec(noinline) RandomAccessIter __fastcall __find_if(RandomAccessIter first, RandomAccessIter last, Predicate pred,
                                                           const random_access_iterator_tag&)
{
    int tripCount = (last - first) >> 2;
    for (; tripCount > 0; --tripCount) {
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
    }
    switch (last - first) {
    case 3:
        if (pred(*first)) return first;
        ++first;
    case 2:
        if (pred(*first)) return first;
        ++first;
    case 1:
        if (pred(*first)) return first;
    case 0:
    default:
        return last;
    }
}
#pragma auto_inline(on)
template<class InputIter, class Predicate> inline InputIter __fastcall find_if(InputIter first, InputIter last, Predicate pred)
{
    return __find_if(first, last, pred, random_access_iterator_tag());
}
}

namespace NAnimationEvents {
float CManager::GetExtraEventTime(unsigned long anim, const CCharString& name) const
{
    const CSmallVector<CEvent, 32>* events = GetPExtraAnimEvents(anim);
    unsigned long index = EventSymbolMap.GetSymbolSLONG(name);
    return std::find_if(events->begin(), events->end(), CIsEventIndexEqualTo(index))->Time;
}
}

#undef private
class CCharStringData;
class CCharString{public:CCharStringData*PStringData;};
CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CSymbolMap::~CSymbolMap(){}
static unsigned long fixtureSymbol;
static const CCharString*fixtureName;
static const CSymbolMap*fixtureReceiver;
static unsigned int fixtureCalls=0,fixtureErrors=0;
long CSymbolMap::GetSymbolSLONG(const CCharString&name)const{
    ++fixtureCalls;if(this!=fixtureReceiver||&name!=fixtureName)++fixtureErrors;return (long)fixtureSymbol;
}
void NAnimationEvents::CManager::GetEvents(unsigned long,std::vector<std::pair<CCharString,float> >&)const{++fixtureErrors;}
int main(){
    NAnimationEvents::CManager manager;
    typedef CSmallVector<NAnimationEvents::CEvent,32> Events;
    typedef std::pair<unsigned long,Events> Mapping;
    Mapping map[3];NAnimationEvents::CEvent data[3][20],before[3][20];memset(data,0,sizeof(data));
    map[0].first=7;map[1].first=21;map[2].first=50;
    manager.AnimToExtraEventsMap._M_start=map;manager.AnimToExtraEventsMap._M_finish=map+3;manager.AnimToExtraEventsMap._M_end_of_storage._M_data=map+3;
    for(int j=0;j<3;++j)map[j].second.Start=data[j];
    CCharString name={0};fixtureName=&name;fixtureReceiver=&manager.EventSymbolMap;
    unsigned int cases=0;
    for(int n=1;n<=20;++n)for(int flag=0;flag<4;++flag){
        for(int j=0;j<3;++j){map[j].second.Size=n;for(int i=0;i<n;++i){data[j][i].Index=(unsigned long)(i*17+3);data[j][i].ExternallyImported=flag&1;data[j][i].StopEvent=(flag>>1)&1;data[j][i].Time=(float)(j*100+i)/4.0f;}}
        memcpy(before,data,sizeof(data));
        for(int j=0;j<3;++j)for(int pos=0;pos<n;++pos){
            fixtureSymbol=(unsigned long)(pos*17+3);float expected=-1000;
            for(int i=0;i<n;++i)if(data[j][i].Index==fixtureSymbol){expected=data[j][i].Time;break;}
            unsigned int calls=fixtureCalls;float got=manager.GetExtraEventTime(map[j].first,name);
            if(got!=expected||fixtureCalls!=calls+1||fixtureErrors)return 1;
            if(memcmp(before,data,sizeof(data)))return 2;++cases;
        }
    }
    for(int j=0;j<3;++j)for(int n=2;n<=20;++n){
        map[j].second.Size=n;data[j][0].Index=0x3fffffffUL;data[j][n-1].Index=0x3fffffffUL;data[j][0].Time=-8.0f;data[j][n-1].Time=16.0f;fixtureSymbol=0x3fffffffUL;
        if(manager.GetExtraEventTime(map[j].first,name)!=-8.0f||fixtureErrors)return 3;++cases;
        data[j][0].Index=3;data[j][n-1].Index=(unsigned long)((n-1)*17+3);
    }
    if(cases!=2577||fixtureCalls!=cases||sizeof(NAnimationEvents::CEvent)!=8||sizeof(Events)!=8)return 4;
    puts("006f96c0_TEST PASS");return 0;
}