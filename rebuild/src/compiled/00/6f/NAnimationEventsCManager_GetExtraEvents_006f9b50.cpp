// NAnimationEvents::CManager::GetExtraEvents(unsigned long anim, vector<pair<CCharString,float>>& events) const -- append (name, time) for every extra event of `anim`
// Leaf: ?GetExtraEvents@CManager@NAnimationEvents@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F9B50, 90 bytes, `ret 8`.
// Label correction: catalog "Game_GetBestTargetInDirection" is propagated; Ego_r.pdb `?GetExtraEvents@CManager@
//   NAnimationEvents@@` names this body (the twin of 0x006F9AC0 with the CFillEvents functor).
// Callees: CVectorMap LowerBound 0x006FAF80, std::for_each<const CEvent*, CFillEvents> 0x006FCE00.
// Shape: as 0x006F9AC0.
class CCharString;                                        // 4-byte string handle (only passed by reference here)
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
template<class InputIter, class Function> Function __fastcall for_each(InputIter first, InputIter last, Function f);   // 0x006FC8E0 (CFillEventNames) / 0x006FCE00 (CFillEvents)
}

namespace NAnimationEvents {
void CManager::GetExtraEvents(unsigned long anim, std::vector<std::pair<CCharString, float> >& events) const
{
    const CSmallVector<CEvent, 32>* animEvents = GetPExtraAnimEvents(anim);
    if (animEvents)
        std::for_each(animEvents->begin(), animEvents->end(), CFillEvents(events, EventSymbols));
}
}