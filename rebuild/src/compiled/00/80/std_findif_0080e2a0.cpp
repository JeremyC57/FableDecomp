// std::find_if(std::pair<CIntelligentPointer<CThing>,long>* first, std::pair<CIntelligentPointer<CThing>,long>* last, CIsFirstElementEqualTo pred) -- STLport find_if over (intelligent pointer, value) pairs: forward to the random-access __find_if with a copy of the predicate (the first pair whose thing is the predicate's thing)
// Leaf: ??$find_if@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0080E2A0, length 75 bytes (+ int3 padding), `ret 8`, the iterator in eax.
// Callers: no direct call or absolute reference (xref.py finds none); the only other caller of the callee is the
//   remove-matching helper @ 0x0080E2F0 (call 0x0080E34B).
// Callees: STLport __find_if(first, last, pred, const random_access_iterator_tag&) @ 0x0080E060 (`__fastcall`, the 4x
//   unrolled 12-byte-stride search, predicate by value, `ret 0Ch`); CBaseIntelligentPointer::CBaseIntelligentPointer @
//   0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 / ~CBaseIntelligentPointer @ 0x00A01C10. Vtable 0x01238C6C =
//   CIntelligentPointer<CThing>.
// Identity: Ego_r.pdb has `std::_Find_if<pair<CIntelligentPointer<CThing>,long>*, `anonymous namespace'::
//   CIsFirstElementEqualTo>`; the callee's compare (`first->first.GetPItem() == pred.GetPItem()`) is that predicate.
// Shape: `return __find_if(first, last, pred, random_access_iterator_tag());` under /Gr: arguments right to left -- the
//   empty tag temporary is homed at the predicate's own argument slot (`lea eax,[esp+10h] ; push eax`), then the
//   predicate is copy-constructed into its outgoing 8-byte slot (`sub esp,8 ; mov esi,esp`: base ctor, vtable store,
//   SetPItem(GetPItem())); the by-value predicate is destroyed after the call (`~CBaseIntelligentPointer`). The tag
//   only overlays that slot when VC7.1 sees the callee's body (STLport's 4x-unrolled __find_if, the retail 0x0080E060
//   shape, kept out of line with `__declspec(noinline)` as in retail): with a bare declaration the tag gets its own
//   `push ecx` frame byte (77 v 75). The leaf is `??$find_if@` because `find_if` also matches the larger __find_if.
// Label correction: the catalog's `_Tree<_Tmap_traits<long,long,less<long>,...>>` is propagated (the body is find_if).
class CBaseObject;
class CThing;

class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                 // 0x00A01B10
    virtual ~CBaseIntelligentPointer();        // 0x00A01C10
    CBaseObject* GetPItem() const;             // 0x00A01B50
    void SetPItem(CBaseObject* item);          // 0x00A01B90
    void* PData;                               // +0x4
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C for CThing
public:
    __forceinline CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
struct random_access_iterator_tag { random_access_iterator_tag() { } };

// STLport's random-access __find_if (0x0080E060), in the TU as in retail (an instantiated template).
template <class RandomAccessIter, class Predicate>
__declspec(noinline) RandomAccessIter __fastcall __find_if(RandomAccessIter first, RandomAccessIter last, Predicate pred, const random_access_iterator_tag&)
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

template <class InputIter, class Predicate>
InputIter __fastcall find_if(InputIter first, InputIter last, Predicate pred)
{
    return __find_if(first, last, pred, random_access_iterator_tag());
}
}

typedef std::pair<CIntelligentPointer<CThing>, long> CThingEntry;   // 12 bytes

namespace {
struct CIsFirstElementEqualTo {
    CIntelligentPointer<CThing> Thing;         // +0x0 (8 bytes)
    bool operator()(const CThingEntry& entry) const { return entry.first.GetPItem() == Thing.GetPItem(); }
};
}

template CThingEntry* __fastcall std::find_if(CThingEntry*, CThingEntry*, CIsFirstElementEqualTo);