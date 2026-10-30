// 006f96c0 NAnimationEvents::CManager::GetExtraEventTime
// Leaf: ?GetExtraEventTime@CManager@NAnimationEvents@@ABEMKABVCCharString@@@Z
// Flags:
// Namespace CManager has no shared header; engine/CManager.h is NUISystem's unrelated type.
// Layout: CSymbolMap restores CBaseClassNonCopyable; unread32byte payload is a projection. CManager has no bases, and slot0 is GetEvents (PDB64145, RTTI2112).
class CCharString;                                        // 4-byte string handle (only passed by reference here)
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
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