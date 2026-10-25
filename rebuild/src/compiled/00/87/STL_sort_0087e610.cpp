// _STL::sort(const CAnimationEntry** first, const CAnimationEntry** last, CTurningAnimCompare comp) -- sort the turning animations by how close their rotation is to the wanted turn: introsort to 16-element runs, then the final insertion sort
// Leaf: ??$sort@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087E610, 69 bytes (+ int3 padding), `ret 4` (/Gr algorithm TU: first in
//   ecx, last in edx, the 4-byte comparator by value on the stack).
// Identity: STLport 4.x `sort(first, last, comp)` over the vector<const CAnimationEntry*> of the
//   CCreatureAction_InterruptableMidAttackAutoTurn TU, comparator `?A0x37766ae9::CTurningAnimCompare` (Ego_r.pdb).
//   The catalog label "introsort_CAnimationEntry" is descriptive.
// Callees: __introsort_loop(first, last, (_Tp*)0, __lg(last - first) * 2, comp) @ 0x0087E530,
//   __final_insertion_sort(first, last, comp) @ 0x0087E400 (both same instantiation, out of line).
// Shape: `__lg` inline (the `sar eax,1 ; inc ecx` loop), the depth limit as `lea eax,[ecx+ecx]`.
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
template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit,
                                 _Compare __comp);   // 0x0087E530
template <class _RandomAccessIter, class _Compare>
void __fastcall __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x0087E400

template <class _Size>
inline _Size __lg(_Size __n)
{
    _Size __k;
    for (__k = 0; __n != 1; __n >>= 1) ++__k;
    return __k;
}
template <class _RandomAccessIter, class _Compare>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first != __last) {
        __introsort_loop(__first, __last, (TEntry*)0, __lg(__last - __first) * 2, __comp);
        __final_insertion_sort(__first, __last, __comp);
    }
}
template void __fastcall sort<TEntry*, CTurningAnimCompare>(TEntry*, TEntry*, CTurningAnimCompare);
}