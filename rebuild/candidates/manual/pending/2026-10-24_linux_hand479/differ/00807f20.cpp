// std::__find_if(NPlayerTargeting::CTargetPoint* first, NPlayerTargeting::CTargetPoint* last, CTargetPointIsNotEnemyOfThing pred, const random_access_iterator_tag&) -- STLport random-access find_if: the first target point whose thing is alive and not an enemy of the predicate's thing, or last
// Grade: DIFFER(476v480) -- retail holds the dead-flag mask 1 in bl (`mov bl,1` ahead of the loop, `test byte ptr [eax+91h],bl` in all seven inlined tests) and so spills `last` to a `push ecx` word with the predicate in ebp; every spelling tried tests the flag with an immediate and keeps last/pred in ebx/ebp. Everything else (trip count, unrolled tests, GetPItem/AreThingsEnemies sequence, switch tail) matches.
// Leaf: ??$__find_if@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00807F20, 478 bytes, /Gr `__fastcall` (first ecx, last edx; the 4-byte
//   predicate by value and the tag reference on the stack, `ret 8`).
// Identity: Ego_r.pdb `std::_Find_if<CTargetPoint*, `anonymous namespace'::CTargetPointIsNotEnemyOfThing>` (donor
//   0x008018C0) is the Dinkumware counterpart; retail is STLport's `__find_if` (callers 0x00804D35, 0x0080893A,
//   0x008094AE push the tag reference, then the predicate). The catalog's "std_find_if_EnemyFilter" is descriptive.
// Layout: Ego_r.pdb CTargetPointIsNotEnemyOfThing (4 bytes: CThing* Thing, anonymous namespace ?A0x58ed6966),
//   NPlayerTargeting::CTargetPoint (0x1c); retail CThing Dead flag +0x91 bit 0.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (three per test: null, dead flag, enemy test),
//   AreThingsEnemies(CThing*, CThing*) 0x0076D2F0 (`__fastcall`).
// Shape: STLport 4.x `__find_if` for random-access iterators: `trip_count = (last - first) >> 2` (signed division by
//   28), four predicate tests per pass, then `switch (last - first)` over 3/2/1 (dec/je chain), default returns last;
//   the inline predicate is one negated `||` chain (`!(!thing || thing->Dead || AreThingsEnemies(Thing, thing))`,
//   GetPItem re-fetched each time), which keeps the bit mask 1 in bl across the
//   loop (as the landed __find_if 0x00748E40) and spills `last` to the `push ecx` word.
class CBaseObject;
class CThing {                                             // retail flag byte only
public:
    unsigned char _pad00[0x91];
    unsigned char Dead : 1;                                // +0x91 bit 0
};

bool __fastcall AreThingsEnemies(CThing* a, CThing* b);   // 0x0076D2F0

class CBaseIntelligentPointer {
public:
    void* PData;                                           // +0x04
    virtual ~CBaseIntelligentPointer();                    // 0x00A01C10
    CBaseObject* GetPItem() const;                         // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* Get() const { return (T*)GetPItem(); }
};

class C3DVector { public: float X, Y, Z; };

namespace NPlayerTargeting {
class CTargetPoint {                                       // 0x1c bytes
public:
    CIntelligentPointer<CThing> PThing;                    // +0x00
    long Index;                                            // +0x08
    C3DVector TargetingPosition;                           // +0x0c
    long FrameRejected;                                    // +0x18
};
}

namespace {
class CTargetPointIsNotEnemyOfThing {                      // Ego_r.pdb, 4 bytes
public:
    CThing* Thing;                                         // +0x00
    bool operator()(const NPlayerTargeting::CTargetPoint& point) const
    {
        return !(point.PThing.Get() == 0 || point.PThing.Get()->Dead || AreThingsEnemies(Thing, point.PThing.Get()));
    }
};
}

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };

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
    case 0:
    default:
        return __last;
    }
}

template NPlayerTargeting::CTargetPoint* __fastcall __find_if(NPlayerTargeting::CTargetPoint*,
                                                              NPlayerTargeting::CTargetPoint*,
                                                              CTargetPointIsNotEnemyOfThing,
                                                              const random_access_iterator_tag&);
}
