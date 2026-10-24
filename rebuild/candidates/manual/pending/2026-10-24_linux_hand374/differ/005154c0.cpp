// std::__unguarded_insertion_sort_aux<CThingBuilding**,CThingBuilding*,CIsHouseLargerThan>(CThingBuilding** first, CThingBuilding** last, CThingBuilding**, CIsHouseLargerThan comp) -- STLport sort tail for the region population's houses: insert each building of [first, last) backwards into the sorted run before it (no lower bound check: a sorted 16-element head guards it), largest house first
// Grade: DIFFER(98v96) -- only the frame: retail homes the empty comparator copy over the spilled `last` word (`push ecx`, `lea ecx,[esp+18h]` = the `[esp+4]` last slot), the lane gives it a word of its own (`sub esp,8 ; add esp,8`), which shifts the loop-head padding (retail `nop` + `lea ebx,[ebx+0]`, lane `jmp` + `lea ecx,[ecx+0]`); body, registers and calls match
// Leaf: ??$__unguarded_insertion_sort_aux@
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
class CThingBuilding;

namespace std {
template <class Arg1, class Arg2, class Result>
struct binary_function {
    typedef Arg1 first_argument_type;
    typedef Arg2 second_argument_type;
    typedef Result result_type;
};
}

// PDB: an empty class whose base binary_function<const CThingBuilding*,const CThingBuilding*,bool> is listed with it.
class CIsHouseLargerThan : public std::binary_function<const CThingBuilding*, const CThingBuilding*, bool> {
public:
    CIsHouseLargerThan() { }
    CIsHouseLargerThan(const CIsHouseLargerThan&) { }
    bool operator()(const CThingBuilding* a, const CThingBuilding* b) const;   // 0x00512410
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
