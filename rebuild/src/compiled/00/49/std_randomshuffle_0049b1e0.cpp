// std::random_shuffle(long*, long*, RandFunc&) -- STLport random shuffle of a long array: each element from first+1 swaps with first[rand(i - first + 1)], RandFunc drawing through GFFloatRandom on the caller's seed
// Flags: /Gr
// Leaf: ??$random_shuffle@PAJURandFunc@@@std@@YIXPAJ0AAURandFunc@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049B1E0, 88 bytes, `ret 4` (__fastcall: first in ecx -> ebx, last in edx ->
//   ebp, the RandFunc reference on the stack; the `push ecx` frame word is the int -> float conversion slot).
// Callees: 0x004978A0 GFFloatRandom(float, unsigned long*) (__fastcall: the seed pointer in ecx, the float on the stack),
//   0x00BFEA70 __ftol2 (the (long) conversion of the float draw).
// Caller: CProgressDisplay::InitialiseTextures 0x0049A0B0 (`RandFunc rand(&seed); random_shuffle(v.begin(), v.end(),
//   rand)` over a vector<long>, the functor a frame local passed by reference).
// Identity: FableWin.pdb names `??RRandFunc@@QAEJJ@Z` (long RandFunc::operator()(long)), `??0RandFunc@@QAE@PAK@Z` and
//   `random_shuffle<vector<long>::iterator, RandFunc>`; Ego_r.pdb layout RandFunc { unsigned long* Seed; } (4 bytes).
// Shape: STLport `if (first == last) return; for (i = first + 1; i != last; ++i) iter_swap(i, first + rand((i - first)
//   + 1));` with the inline `RandFunc::operator()` = `(long)GFFloatRandom((float)n, Seed)` and the inline long swap.
// Name (round 4, NF40): the STLport template instance in namespace std, `random_shuffle<long*, RandFunc>` (FableWin.pdb
//   random_shuffle<vector<..>::iterator, RandFunc>; the caller 0x0049A0B0 already binds this spelling through stlport/fable_containers.h).
// Label correction: catalogued as `Shuffle_RandomIndices`; the body is STLport `random_shuffle<long*, RandFunc>`.
float __fastcall GFFloatRandom(float range, unsigned long* seed);    // 0x004978A0

struct RandFunc {
    unsigned long* Seed;                            // +0x00
    long operator()(long n) { return (long)GFFloatRandom((float)n, Seed); }
};

namespace std {
template <class _Tp>
inline void __iter_swap_aux(_Tp* __a, _Tp* __b)
{
    _Tp __tmp = *__a;
    *__a = *__b;
    *__b = __tmp;
}

template <class _RandomAccessIter, class _RandomNumberGenerator>
void __fastcall random_shuffle(_RandomAccessIter __first, _RandomAccessIter __last, _RandomNumberGenerator& __rand)
{
    if (__first == __last) return;
    for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
        __iter_swap_aux(__i, __first + __rand((__i - __first) + 1));
}

template void __fastcall random_shuffle<long*, RandFunc>(long*, long*, RandFunc&);
}
