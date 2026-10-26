// std::__find_if(pair<CIntelligentPointer<const CThing>,NStealth::CDangerStatus>* first, pair<CIntelligentPointer<const CThing>,NStealth::CDangerStatus>* last, CFindDangerThing pred, const random_access_iterator_tag&) -- first danger-thing entry whose thing is the predicate's thing (four-way unrolled scan)
// Leaf: __find_if
// Retail: Fable.exe (TLC, VC7.1) @ 0x007485F0, 159 bytes (+1 int3), `ret 8` (link-time __fastcall: first in ecx, last in
//   edx, the 4-byte predicate and the iterator-tag reference on the stack).
// Callee: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the inlined predicate: entry.first's item == pred.Thing).
// Callers: CTCStealth::SetAsInDanger 0x00747B80 (non-const DangerThings scan before the push_back 0x00749620) and the
//   out-of-line find_if forwarder 0x007489C0.
// Label: the manifest's "CVector_Find_LinearProbe" is descriptive. Ego_r.pdb has the Dinkumware twin
//   `std::_Find_if<pair<CIntelligentPointer<const CThing>,NStealth::CDangerStatus>*, (anonymous)::CFindDangerThing>`;
//   this is the STLport random-access `__find_if` over the mutable pointer (its const-pointer twin is 0x00748690).
// Shape: STLport `trip_count = (last - first) >> 2` (`sar 6` over 16-byte entries), four unrolled `if (pred(*first))
//   return first; ++first;` per trip, then the `switch (last - first)` fall-through tail (cases 3/2/1) returning last; the
//   predicate's thing pointer is held in ebx. CFindDangerThing(const CThing&) stores &thing (Ego_r.pdb names the ctor).
class CThing;

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
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
    T* Get() const { return (T*)GetPItem(); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

namespace NStealth {
enum EDangerStatus { DANGER_STATUS_NONE = 0 };
class CDangerStatus {                                    // Ego_r.pdb 8 bytes
public:
    EDangerStatus DangerStatus;                          // +0x00
    long          Timer;                                 // +0x04
};
}

namespace std {
template <class A, class B> struct pair {
    A first;                                             // +0x00
    B second;                                            // +0x08
    pair(const pair& other) : first(other.first), second(other.second) {}
};
}

typedef std::pair<CIntelligentPointer<const CThing>, NStealth::CDangerStatus> TDangerPair;   // 16 bytes

namespace {
class CFindDangerThing {
public:
    const CThing* Thing;
    CFindDangerThing(const CThing& thing) : Thing(&thing) {}
    bool operator()(const TDangerPair& entry) const { return entry.first.Get() == Thing; }
};
}

namespace std {
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

template TDangerPair* __fastcall __find_if(TDangerPair*, TDangerPair*, CFindDangerThing, const random_access_iterator_tag&);
}