// _STL::__find_if(const pair<CIntelligentPointer<const CThing>,NStealth::CDangerStatus>* first, const pair<CIntelligentPointer<const CThing>,NStealth::CDangerStatus>* last, CFindDangerThing pred, const random_access_iterator_tag&) -- first danger-thing entry whose thing is the predicate's thing (const scan)
// Leaf: __find_if
// Retail: Fable.exe (TLC, VC7.1) @ 0x00748690, 159 bytes (+1 int3), `ret 8` (link-time __fastcall: first in ecx, last in
//   edx, the 4-byte predicate and the iterator-tag reference on the stack).
// Callee: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the inlined predicate: entry.first's item == pred.Thing).
// Callers: CTCStealth::IsDangerThing(const CThing&) const 0x00747450 (Ego_r.pdb `QBE`: the const vector, so the const
//   pointer instance) and the out-of-line find_if forwarder 0x007489E0.
// Label: the manifest's "CVector_Find_LinearProbe" is descriptive. Ego_r.pdb lists both
//   `_Find_if<pair<..>*, CFindDangerThing>` and `_Find_if<const pair<..>*, CFindDangerThing>`; this is the const one, a
//   byte-identical twin of 0x007485F0.
// Shape: STLport random-access `__find_if`: `(last - first) >> 2` unrolled trips, then the `switch (last - first)`
//   fall-through tail; the predicate's thing pointer is held in ebx.
class CThing;

class CBaseIntelligentPointer {
public:
    void* PData;                                         // +0x04 (CBaseObjectPointer*)
    virtual void v00();
    CBaseIntelligentPointer();                           // 0x00A01B10
    void* GetPItem() const;                              // 0x00A01B50
    void SetPItem(const void* item);                     // 0x00A01B90
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
    T* Get() const { return (T*)GetPItem(); }
    virtual void v00();                                  // own vtable (CIntelligentPointer<const CThing> 0x01237F1C)
};

namespace NStealth {
enum EDangerStatus { DANGER_STATUS_NONE = 0 };
class CDangerStatus {                                    // Ego_r.pdb 8 bytes
public:
    EDangerStatus DangerStatus;                          // +0x00
    long          Timer;                                 // +0x04
};
}

namespace _STL {
template <class A, class B> struct pair {
    A first;                                             // +0x00
    B second;                                            // +0x08
    pair(const pair& other) : first(other.first), second(other.second) {}
};
}

typedef _STL::pair<CIntelligentPointer<const CThing>, NStealth::CDangerStatus> TDangerPair;   // 16 bytes

namespace {
class CFindDangerThing {
public:
    const CThing* Thing;
    CFindDangerThing(const CThing& thing) : Thing(&thing) {}
    bool operator()(const TDangerPair& entry) const { return entry.first.Get() == Thing; }
};
}

namespace _STL {
struct random_access_iterator_tag {};

template <class RandomAccessIter, class Predicate>
RandomAccessIter __fastcall __find_if(RandomAccessIter first, RandomAccessIter last, Predicate pred,
                                      const random_access_iterator_tag&)
{
    int trip_count = (last - first) >> 2;

    for (; trip_count > 0; --trip_count) {
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

template const TDangerPair* __fastcall __find_if(const TDangerPair*, const TDangerPair*, CFindDangerThing, const random_access_iterator_tag&);
}