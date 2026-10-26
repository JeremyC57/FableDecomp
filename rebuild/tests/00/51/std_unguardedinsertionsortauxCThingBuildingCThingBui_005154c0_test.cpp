// Behaviour test for 0x005154C0 std::__unguarded_insertion_sort_aux<CThingBuilding**, CThingBuilding*, CIsHouseLargerThan>:
// insert every building of [first, last) backwards (no lower bound test) so the range ends sorted largest house first.
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
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
template <class T> class CDefPointer { public:
    T* Object;
    __forceinline ~CDefPointer() { if (Object) { Object->ReleaseTemporary(); Object = 0; } }
    T* operator->() const { return Object; }
};
class CThingBuilding {
public:
    struct CBuildingSimDefinition* Def;
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


static int g_destroys = 0;
void CBuildingSimDefinition::v00() { }
void CBuildingSimDefinition::Destroy() { ++g_destroys; }
CDefPointer<CBuildingSimDefinition> CThingBuilding::PeekPSimDefinition() const
{
    CDefPointer<CBuildingSimDefinition> p;
    p.Object = Def;                                 // the test's defs hold a large count, so temporaries never destroy them
    return p;
}

int main()
{
    const int N = 7;
    long sizes[N] = { 1000, 3, 9, 1, 9, 7, 4 };     // [0] is the unguarded sentinel (the largest house)
    CBuildingSimDefinition defs[N];
    CThingBuilding buildings[N];
    CThingBuilding* a[N];
    for (int k = 0; k < N; ++k) {
        defs[k].Ref.Count = 100000; defs[k].HouseSize = sizes[k];
        buildings[k].Def = &defs[k]; a[k] = &buildings[k];
    }
    std::__unguarded_insertion_sort_aux(a + 1, a + N, (CThingBuilding**)0, CIsHouseLargerThan());
    int ok = 1;
    long want[N] = { 1000, 9, 9, 7, 4, 3, 1 };
    for (int k = 0; k < N; ++k) if (a[k]->Def->HouseSize != want[k]) ok = 0;
    if (a[0] != &buildings[0] || g_destroys != 0) ok = 0;
    printf(ok ? "005154c0_TEST PASS\n" : "005154c0_TEST FAIL\n");
    return ok ? 0 : 1;
}