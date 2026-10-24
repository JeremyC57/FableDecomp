// std::__unguarded_insertion_sort_aux<CThingBuilding**,CThingBuilding*,CIsHouseLargerThan>(CThingBuilding** first, CThingBuilding** last, CThingBuilding**, CIsHouseLargerThan comp) -- STLport sort tail for the region population's houses: insert each building of [first, last) backwards into the sorted run before it (no lower bound check: a sorted 16-element head guards it), largest house first
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- the comparator's real body (0x00512410) noinline in the TU and no user-declared copy ctor (the landed twin 0x00748C10 shape) homes the empty comparator copy over the spilled last word
// Leaf: ??$__unguarded_insertion_sort_aux@PAPAVCThingBuilding@@PAV1@VCIsHouseLargerThan@@@std@@YIXPAPAVCThingBuilding@@00VCIsHouseLargerThan@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005154C0, 94 bytes (padded to 96), `ret 8` (`__fastcall`, a /Gr algorithm TU: first
//   in ecx, last in edx, the value-type dummy and the empty comparator on the stack).
// Callers: 0x00517587, 0x00519007, 0x0051C456 -- the `__final_insertion_sort` tails of std::sort over CThingBuilding*
//   with CIsHouseLargerThan (0x0051C456 follows __introsort_loop 0x0051BAF0 and __insertion_sort 0x00517500 of the
//   first 16). Catalog label "std_insertion_sort_CThingBuilding" is descriptive.
// Callees: CIsHouseLargerThan::operator()(const CThingBuilding*, const CThingBuilding*) const 0x00512410 (PDB
//   ??RCIsHouseLargerThan@@QBE_NPBVCThingBuilding@@0@Z, thiscall on the empty functor).
// Shape: STLport `for (i = first; i != last; ++i) __unguarded_linear_insert(i, T(*i), comp);` with the linear insert
//   (`next = last; --next; while (comp(val, *next)) { *last = *next; last = next; --next; } *last = val;`) inlined;
//   the by-value comparator copy is an empty object whose address is the frame word that holds `last`.
// Fix (agent901): the comparator's real body (landed 0x00512410) in the TU, noinline, and no user-declared copy ctor --
//   the landed byte-identical twin 0x00748C10 (CCompare_FurthestFromInitPos2D) uses the same shape; the empty copy then
//   lands on the spilled `last` word as retail.

struct CRefCounted {                               // reference count base at +4 of a def (landed 0x00512410 model)
    long Count;
    void DecRef() { --Count; }
    long GetRefCount() const { return Count; }
};
struct CBuildingSimDefinition {
    virtual void v00();
    virtual void Destroy();                        // slot 1 (+0x04)
    CRefCounted Ref;                               // +0x04
    unsigned char _pad08[0x44 - 0x08];
    long HouseSize;                                // +0x44
    __forceinline void ReleaseTemporary() { Ref.DecRef(); if (Ref.GetRefCount() == 0) Destroy(); }
};
template <class T> struct CDefPointer {
    T* Object;
    __forceinline ~CDefPointer() { if (Object) { Object->ReleaseTemporary(); Object = 0; } }
    T* operator->() const { return Object; }
};
class CThingBuilding {
public:
    CDefPointer<CBuildingSimDefinition> PeekPSimDefinition() const;   // 0x0082D2D0
};

namespace std {
template <class Arg1, class Arg2, class Result>
struct binary_function {
    typedef Arg1 first_argument_type;
    typedef Arg2 second_argument_type;
    typedef Result result_type;
};
}

// Empty functor (PDB: base binary_function<const CThingBuilding*, const CThingBuilding*, bool>), implicit copy ctor.
class CIsHouseLargerThan : public std::binary_function<const CThingBuilding*, const CThingBuilding*, bool> {
public:
    __declspec(noinline) bool operator()(const CThingBuilding* a, const CThingBuilding* b) const   // 0x00512410 (same-TU body)
    {
        if (a->PeekPSimDefinition().Object && b->PeekPSimDefinition().Object)
            return a->PeekPSimDefinition()->HouseSize > b->PeekPSimDefinition()->HouseSize;
        return false;
    }
};

namespace std {

template <class RandomAccessIter, class T, class Compare>
inline void __unguarded_linear_insert(RandomAccessIter last, T val, Compare comp)
{
    RandomAccessIter next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}

template <class RandomAccessIter, class T, class Compare>
void __fastcall __unguarded_insertion_sort_aux(RandomAccessIter first, RandomAccessIter last, T*, Compare comp)
{
    for (RandomAccessIter i = first; i != last; ++i)
        __unguarded_linear_insert(i, T(*i), comp);
}

}

template void __fastcall std::__unguarded_insertion_sort_aux<CThingBuilding**, CThingBuilding*, CIsHouseLargerThan>(
    CThingBuilding** first, CThingBuilding** last, CThingBuilding**, CIsHouseLargerThan comp);