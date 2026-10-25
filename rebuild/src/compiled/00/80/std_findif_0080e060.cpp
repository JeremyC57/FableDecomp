// std::__find_if(std::pair<CIntelligentPointer<CThing>,long>* first, std::pair<CIntelligentPointer<CThing>,long>* last, CIsFirstElementEqualTo pred, const random_access_iterator_tag&) -- STLport random-access find_if over the explosion's (thing, emitter id) pairs: the first entry whose thing is pred's thing
// Leaf: ??$__find_if@PAU?$pair@V?$CIntelligentPointer@VCThing@@@@J@std@@VCIsFirstElementEqualTo@
// Retail: Fable.exe @ 0x0080E060, 291 bytes, /Gr `__fastcall` (first in ecx, last in edx), `ret 0Ch`: the 8-byte
//   predicate by value on the stack (destroyed by the callee: `call 0x00A01C10` on its slot before each return) plus the
//   empty iterator-tag reference.
// Callers: remove_if 0x0080E2F0 (landed, CTCDExplosion::FrameUpdate's Emitters clean-up) and the out-of-line find_if
//   front end 0x0080E2A0; both build the predicate copy with CIntelligentPointer's inline copy constructor.
// Identity: Ego_r.pdb `_Find_if<...,CIsFirstElementEqualTo@?A0x41ace303@@>` (the Dinkumware spelling of the same call);
//   CIsFirstElementEqualTo is 8 bytes (CIntelligentPointer<CThing> Value) in the CTCDExplosion TU's anonymous
//   namespace; the element is 12 bytes (pair<CIntelligentPointer<CThing>,long>, PDB CTCDExplosion::Emitters, +0x40).
//   The catalog's "std_find_CBaseIntelligentPointer" is descriptive.
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50, ~CBaseIntelligentPointer 0x00A01C10.
// Shape: STLport `__find_if` verbatim (`__trip_count = (__last - __first) >> 2`, the 4-way unrolled loop, then the
//   fall-through switch on `__last - __first`, 12-byte stride via the 0x2AAAAAAB reciprocal); the inline operator()
//   resolves the entry's thing first (`mov ecx,esi`) and the predicate's second (`lea ecx,[esp+14h]`); both returns
//   run the implicit predicate destructor (= the base dtor) on the argument slot.
class CBaseObject;
class CThing;
class CBaseIntelligentPointer {
public:
    virtual void v00();
    CBaseIntelligentPointer();                           // 0x00A01B10
    ~CBaseIntelligentPointer();                          // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
    CBaseObject* PData;                                  // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    virtual void v00();                                  // this instantiation's vtable 0x01238C6C
    CIntelligentPointer(const CIntelligentPointer<T>& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
namespace std {
template <class T1, class T2> struct pair {
    T1 first;                                            // +0x00
    T2 second;                                           // +0x08
    pair(const T1& a, const T2& b) : first(a), second(b) {}
};
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };
}
namespace {
class CIsFirstElementEqualTo {                           // 8 bytes (Ego_r.pdb)
public:
    CIntelligentPointer<CThing> Value;                   // +0x00
    bool operator()(const std::pair<CIntelligentPointer<CThing>, long>& entry) const
    {
        return entry.first.GetPItem() == Value.GetPItem();
    }
};
}
namespace std {
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
template pair<CIntelligentPointer<CThing>, long>* __fastcall __find_if(pair<CIntelligentPointer<CThing>, long>*,
    pair<CIntelligentPointer<CThing>, long>*, CIsFirstElementEqualTo, const random_access_iterator_tag&);
}