// std::remove_if(std::pair<CIntelligentPointer<CThing>,long>* first, std::pair<CIntelligentPointer<CThing>,long>* last, CIsFirstElementEqualTo pred) -- STLport remove_if over the explosion's (thing, emitter id) pairs: drop every entry whose thing is pred's thing, returning the new end
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- the real STLport __find_if body (4-way unrolled + remainder switch) noinline in the TU with the tag unnamed homes the tag temporary on the live predicate slot
// Leaf: ??$remove_if@PAU?$pair@V?$CIntelligentPointer@VCThing@@@@J@std@@VCIsFirstElementEqualTo@
// Retail: Fable.exe @ 0x0080E2F0, 182 bytes, `ret 8`: __fastcall (the algorithm TUs are /Gr), first in ecx, last in edx,
//   the 8-byte predicate by value on the stack (destroyed by the callee: `call 0x00A01C10` on its slot).
// Caller: 0x0080D7DF (the CTCDExplosion TU, the Emitters vector<pair<CIntelligentPointer<CThing>,long>> at +0x40).
// Callees: __find_if 0x0080E060 (first, last, predicate copy, const random_access_iterator_tag&) and remove_copy_if
//   0x0080DF90 (next, last, result, predicate copy; catalogued `_Remove_if<pair<CIntelligentPointer<CThing>,long>*,
//   _anon_41ACE303::CIsFirstElementEqualTo>`), CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 /
//   SetPItem 0x00A01B90 / dtor 0x00A01C10; CIntelligentPointer<CThing> vtable 0x01238C6C.
// Identity: Ego_r.pdb `??$remove_if@...VCIsFirstElementEqualTo@?A0x41ace303@@@std@@` (the Dinkumware vector-iterator
//   spelling of the same call) and its _Find_if / _Remove_if helpers; CIsFirstElementEqualTo (anonymous namespace of the
//   CTCDExplosion TU, 8 bytes: CIntelligentPointer<CThing> Value). Label: catalogued `ExplosionFilter_RemoveMatchingElement`.
// Shape: STLport `remove_if` -- `first = find_if(first, last, pred); if (first == last) return first; next = first;
//   return remove_copy_if(++next, last, first, pred);` -- find_if inlined (its by-value predicate a frame copy, the
//   __find_if copy built in the argument area, the empty iterator-tag temporary homed in the predicate's own slot); every
//   predicate copy is the implicit copy constructor over CIntelligentPointer's inline copy (base ctor, vtable, SetPItem(
//   GetPItem())); the implicit predicate destructor is the base dtor alone.
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
template <class T> inline random_access_iterator_tag iterator_category(const T*) { return random_access_iterator_tag(); }
}
namespace {
class CIsFirstElementEqualTo {                           // 8 bytes (Ego_r.pdb)
public:
    CIntelligentPointer<CThing> Value;                   // +0x00
    bool operator()(const std::pair<CIntelligentPointer<CThing>, long>& entry) const;
};
}
namespace std {
// __find_if @ 0x0080E060: the real STLport random-access body in the TU, never inlined, the tag parameter unnamed (the
// GOTCHAS empty-tag rule) -- VC7.1 then homes the tag temporary on the live predicate argument slot as retail.
#pragma auto_inline(off)
template <class RandomAccessIter, class Predicate>
__declspec(noinline) RandomAccessIter __fastcall __find_if(RandomAccessIter first, RandomAccessIter last, Predicate pred,
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
#pragma auto_inline(on)
template <class InputIter, class OutputIter, class Predicate>
OutputIter __fastcall remove_copy_if(InputIter first, InputIter last, OutputIter result, Predicate pred);           // 0x0080DF90

template <class InputIter, class Predicate>
inline InputIter find_if(InputIter first, InputIter last, Predicate pred)
{
    return __find_if(first, last, pred, random_access_iterator_tag());
}

template <class ForwardIter, class Predicate>
ForwardIter __fastcall remove_if(ForwardIter first, ForwardIter last, Predicate pred)
{
    first = find_if(first, last, pred);
    if (first != last) {
        ForwardIter next = first;
        first = remove_copy_if(++next, last, first, pred);
    }
    return first;
}
template pair<CIntelligentPointer<CThing>, long>* __fastcall remove_if(pair<CIntelligentPointer<CThing>, long>*,
    pair<CIntelligentPointer<CThing>, long>*, CIsFirstElementEqualTo);
}