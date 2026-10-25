// _STL::__final_insertion_sort(const CAnimationEntry** first, const CAnimationEntry** last, CTurningAnimCompare comp) -- introsort tail for the turning animations: insertion-sort the first 16 entries, then unguarded-insert the rest
// Leaf: ??$__final_insertion_sort@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087E400, 75 bytes (+ int3 padding), `ret 4` (/Gr algorithm TU: first
//   in ecx, last in edx, the 4-byte comparator by value on the stack).
// Identity: STLport 4.x `__final_insertion_sort` of the CCreatureAction_InterruptableMidAttackAutoTurn sort (caller
//   sort() 0x0087E610, comparator `?A0x37766ae9::CTurningAnimCompare`, Ego_r.pdb). The catalog label
//   "for_each_range_dispatch" is descriptive.
// Callees: __insertion_sort @ 0x0087E370, __unguarded_linear_insert(i, *i, comp) @ 0x0087DC30 (landed) -- the
//   `__unguarded_insertion_sort` / `__unguarded_insertion_sort_aux` pair is inline here (no call to an aux body).
// Shape: `(last - first) > 16` as `and eax,-4 ; cmp eax,40h ; jle`; the else arm reloads the comparator slot.
extern "C" double __cdecl fabs(double x);
#pragma intrinsic(fabs)

class CAnimationEntry {
public:
    void GetRotationInAnimation(float* rotation) const; // 0x005DBFD0
};

namespace {
class CTurningAnimCompare {                             // Ego_r.pdb, 4 bytes (a class: mangled V)
public:
    float RotationAngle;                                // +0x00
    bool operator()(const CAnimationEntry* a, const CAnimationEntry* b) const    // out of line @ 0x0087C660
    {
        float rotation_a;
        float rotation_b;
        a->GetRotationInAnimation(&rotation_a);
        a->GetRotationInAnimation(&rotation_b);
        float delta_a = (float)fabs(RotationAngle - rotation_a);
        float delta_b = (float)fabs(RotationAngle - rotation_b);
        if (delta_a < delta_b)
            return true;
        return false;
    }
};
}
typedef const CAnimationEntry* TEntry;

namespace _STL {
const int __stl_threshold = 16;

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x0087DC30
template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x0087E370

template <class _RandomAccessIter, class _Tp, class _Compare>
inline void __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}
template <class _RandomAccessIter, class _Compare>
inline void __unguarded_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    __unguarded_insertion_sort_aux(__first, __last, (TEntry*)0, __comp);   // _STLP_VALUE_TYPE
}
template <class _RandomAccessIter, class _Compare>
void __fastcall __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__last - __first > __stl_threshold) {
        __insertion_sort(__first, __first + __stl_threshold, __comp);
        __unguarded_insertion_sort(__first + __stl_threshold, __last, __comp);
    }
    else
        __insertion_sort(__first, __last, __comp);
}
template void __fastcall __final_insertion_sort<TEntry*, CTurningAnimCompare>(TEntry*, TEntry*, CTurningAnimCompare);
}