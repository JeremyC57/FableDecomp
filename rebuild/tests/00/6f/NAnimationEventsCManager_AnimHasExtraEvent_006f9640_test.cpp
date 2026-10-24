// Behaviour test for 0x006F9640 NAnimationEvents::CManager::AnimHasExtraEvent(anim, name): true when the anim has extra
// events and one of them has the index the event symbol map gives for `name`.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __fastcall
#define __declspec(x)
#endif
class CCharString { public: int Tag; };                  // test: a complete 4-byte handle
class CSymbolMap {                                        // retail 0x24 bytes
public:
    unsigned char _body[0x24];
    unsigned long Lookup(const CCharString& name) const;              // 0x009E5120
    bool Lookup(const CCharString& name, long* value) const;          // 0x009E5170
};

namespace std {
template<class T> class allocator { public: allocator() {} };
template<class T, class Alloc = allocator<T> > class vector {
public:
    T* _M_start;                                          // +0x00
    T* _M_finish;                                         // +0x04
    T* _M_end_of_storage;                                 // +0x08
};
template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
struct input_iterator_tag { input_iterator_tag() {} };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() {} };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() {} };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() {} };
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

template<class K, class V> class CKeyPairCompareLess { public: CKeyPairCompareLess() {} };

template<class K, class V, class Compare = CKeyPairCompareLess<K, V> > class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    typedef const value_type* const_iterator;
    std::vector<value_type> Map;                          // +0x00
    Compare Comp;                                         // +0x0c
    bool Dirty;                                           // +0x0d
    const_iterator End() const { return Map._M_finish; }
    const_iterator LowerBound(const K& key) const;        // 0x006FAF80
    const_iterator Find(K key) const
    {
        K lookupKey = key;
        const_iterator it = LowerBound(lookupKey);
        if (it == End() || key < it->first)
            it = End();
        return it;
    }
};

template<class K, class V, class Compare>
__declspec(noinline) typename CVectorMap<K, V, Compare>::const_iterator CVectorMap<K, V, Compare>::LowerBound(const K& key) const
{
    const_iterator first = Map._M_start;
    int len = Map._M_finish - first;
    while (len > 0) {
        int half = len >> 1;
        const_iterator middle = first + half;
        if (middle->first < key) {
            first = middle + 1;
            len = len - half - 1;
        }
        else
            len = half;
    }
    return first;
}

class CMeshDataBank;

namespace NAnimationEvents {
class CFillEventNames {                                   // Ego_r.pdb: 8 bytes
public:
    std::vector<CCharString>* EventNames;                 // +0x00
    const CSymbolMap* EventSymbolMap;                     // +0x04
    CFillEventNames(std::vector<CCharString>& names, const CSymbolMap& symbols) : EventNames(&names), EventSymbolMap(&symbols) {}
    void operator()(const CEvent& event);                 // inlined into for_each 0x006FC8E0
};
class CFillEvents {                                       // Ego_r.pdb: 8 bytes
public:
    std::vector<std::pair<CCharString, float> >* Events;  // +0x00
    const CSymbolMap* EventSymbolMap;                     // +0x04
    CFillEvents(std::vector<std::pair<CCharString, float> >& events, const CSymbolMap& symbols) : Events(&events), EventSymbolMap(&symbols) {}
    void operator()(const CEvent& event);                 // 0x006FCC80
};
class CIsEventIndexEqualTo {                              // Ego_r.pdb: 4 bytes
public:
    unsigned long Index;                                  // +0x00
    CIsEventIndexEqualTo(unsigned long index) : Index(index) {}
    bool operator()(const CEvent& event) const { return event.Index == Index; }
};
class CManager {
public:
    virtual void GetEvents(unsigned long anim, std::vector<std::pair<CCharString, float> >& events) const;
    const CMeshDataBank* Bank;                                                   // +0x04
    CSymbolMap EventSymbols;                                                     // +0x08
    CVectorMap<unsigned long, CSmallVector<CEvent, 32> > AnimToExtraEventsMap;   // +0x2c
    bool DifferentToSavedCopy;                                                   // +0x3c
    bool UseEmbeddedEvents;                                                      // +0x3d
    const CSmallVector<CEvent, 32>* GetPExtraAnimEvents(unsigned long anim) const    // 0x006F9560 (inlined)
    {
        const CSmallVector<CEvent, 32>* result = 0;
        CVectorMap<unsigned long, CSmallVector<CEvent, 32> >::const_iterator it = AnimToExtraEventsMap.Find(anim);
        if (it != AnimToExtraEventsMap.End())
            result = &it->second;
        return result;
    }
    bool AnimHasExtraEvent(unsigned long anim, const CCharString& name) const;
    float GetExtraEventTime(unsigned long anim, const CCharString& name) const;
    void GetExtraEventNames(unsigned long anim, std::vector<CCharString>& names) const;
    void GetExtraEvents(unsigned long anim, std::vector<std::pair<CCharString, float> >& events) const;
};
}

namespace std {
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
template<class InputIter, class Predicate> inline InputIter find_if(InputIter first, InputIter last, Predicate pred)
{
    return __find_if(first, last, pred, random_access_iterator_tag());
}
}

// test stubs: the symbol map resolves a name handle to its event index (names are tagged ints)
static int g_lookups = 0;
bool CSymbolMap::Lookup(const CCharString& name, long* value) const
{
    ++g_lookups;
    int tag = *(const int*)&name;
    if (tag < 0) return false;
    *value = tag;
    return true;
}
void NAnimationEvents::CManager::GetEvents(unsigned long, std::vector<std::pair<CCharString, float> >&) const { }

namespace NAnimationEvents {
bool CManager::AnimHasExtraEvent(unsigned long anim, const CCharString& name) const
{
    bool result = false;
    const CSmallVector<CEvent, 32>* events = GetPExtraAnimEvents(anim);
    if (events) {
        long index = -1;
        if (EventSymbols.Lookup(name, &index)) {
            if (std::find_if(events->begin(), events->end(), CIsEventIndexEqualTo(index)) != events->end())
                result = true;
        }
    }
    return result;
}
}


int main()
{
    using namespace NAnimationEvents;
    typedef std::pair<unsigned long, CSmallVector<CEvent, 32> > Entry;
    CEvent evA[6]; CEvent evB[1];
    for (int k = 0; k < 6; ++k) { evA[k].Index = 10 + k; evA[k].ExternallyImported = 0; evA[k].StopEvent = 0; evA[k].Time = 0.0f; }
    evB[0].Index = 3; evB[0].ExternallyImported = 0; evB[0].StopEvent = 0; evB[0].Time = 0.0f;
    Entry entries[2];
    entries[0].first = 100; entries[0].second.Start = evA; entries[0].second.Size = 6;
    entries[1].first = 200; entries[1].second.Start = evB; entries[1].second.Size = 1;
    CManager* m = (CManager*)malloc(sizeof(CManager)); memset(m, 0, sizeof(*m));
    m->AnimToExtraEventsMap.Map._M_start = entries; m->AnimToExtraEventsMap.Map._M_finish = entries + 2;
    m->AnimToExtraEventsMap.Map._M_end_of_storage = entries + 2;
    int names[4] = { 15, 3, 99, -1 };
    const CCharString& n15 = *(const CCharString*)&names[0];
    const CCharString& n3 = *(const CCharString*)&names[1];
    const CCharString& n99 = *(const CCharString*)&names[2];
    const CCharString& nBad = *(const CCharString*)&names[3];
    int ok = 1;
    if (!m->AnimHasExtraEvent(100, n15)) ok = 0;            // the sixth event (remainder path)
    if (m->AnimHasExtraEvent(100, n3)) ok = 0;
    if (!m->AnimHasExtraEvent(200, n3)) ok = 0;
    if (m->AnimHasExtraEvent(100, n99)) ok = 0;
    if (m->AnimHasExtraEvent(100, nBad)) ok = 0;            // unknown symbol
    int before = g_lookups;
    if (m->AnimHasExtraEvent(150, n15)) ok = 0;             // no such anim: no lookup at all
    if (m->AnimHasExtraEvent(300, n15)) ok = 0;
    if (g_lookups != before) ok = 0;
    printf(ok ? "006f9640_TEST PASS\n" : "006f9640_TEST FAIL\n");
    return ok ? 0 : 1;
}