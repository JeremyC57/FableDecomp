// std::__find_if(pair<CIntelligentPointer<const CThing>,NStealth::CDangerStatus>* first, pair<CIntelligentPointer<const CThing>,NStealth::CDangerStatus>* last, CDeadDangerThing pred, const random_access_iterator_tag&) -- find the first stealth danger entry whose thing is gone, dead, or whose status is clear
// Leaf: ??$__find_if@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00748E40, 370 bytes, __fastcall (first ecx, last edx), `ret 8` (pred, tag).
// Label correction: catalog "LinearSearch_Range_WithValidityCheck" -> STLport 4.6's random-access __find_if (the body
//   std::find_if forwards to) inside CTCStealth's inlined remove_if (callers 0x00747CA0 = CTCStealth::UpdateDangerInfo,
//   0x0074915A, 0x0074931E); the predicate is the anonymous-namespace CDeadDangerThing of the landed remove_copy_if
//   0x00748D80.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (twice per test: null, then the dead flag).
// Shape: STLport `__find_if`: trip_count = (last - first) >> 2 (`sar 6` on 16-byte elements), four unrolled
//   `if (pred(*first)) return first; ++first;` per trip, then `switch (last - first)` with cases 3, 2, 1 falling
//   through; the predicate `!item || item->Dead || status == 0` keeps the bit mask 1 in bl across the whole loop.
class CThing {                                  // retail flag byte only
public:
    unsigned char _pad00[0x91];
    unsigned char Dead : 1;                     // +0x91 bit 0
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {

public:
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
public:
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};

namespace NStealth {
enum EDangerStatus { DANGER_STATUS_NONE = 0 };
class CDangerStatus {                           // Ego_r.pdb, 8 bytes
public:
    EDangerStatus DangerStatus;                 // +0x00
    long          Timer;                        // +0x04
};
}

namespace std {
template<class T1, class T2> struct pair {
    T1 first;                                   // +0x00
    T2 second;                                  // +0x08
};
struct random_access_iterator_tag { random_access_iterator_tag() {} };
}

typedef std::pair<CIntelligentPointer<const CThing>, NStealth::CDangerStatus> TDangerPair;

namespace {
class CDeadDangerThing {
public:
    bool operator()(const TDangerPair& e) const
    {
        return e.first.GetPItem() == 0 || e.first.GetPItem()->Dead || e.second.DangerStatus == NStealth::DANGER_STATUS_NONE;
    }
};
}

namespace std {

template <class _RandomAccessIter, class _Predicate>
_RandomAccessIter __fastcall __find_if(_RandomAccessIter __first, _RandomAccessIter __last, _Predicate __pred,
                                       const random_access_iterator_tag&)
{
    int __trip_count = (__last - __first) >> 2;

    for (; __trip_count > 0; --__trip_count) {
        if (__pred(*__first)) return __first;
        ++__first;

        if (__pred(*__first)) return __first;
        ++__first;

        if (__pred(*__first)) return __first;
        ++__first;

        if (__pred(*__first)) return __first;
        ++__first;
    }

    switch (__last - __first) {
    case 3:
        if (__pred(*__first)) return __first;
        ++__first;
    case 2:
        if (__pred(*__first)) return __first;
        ++__first;
    case 1:
        if (__pred(*__first)) return __first;
        ++__first;
    case 0:
    default:
        return __last;
    }
}

template TDangerPair* __fastcall __find_if(TDangerPair*, TDangerPair*, CDeadDangerThing, const random_access_iterator_tag&);

}