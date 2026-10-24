// std::random_shuffle<CThing**,RandFunc>(CThing** first, CThing** last, RandFunc& rand) -- shuffle a pointer range in place, swapping each element with a random earlier-or-same one drawn from the seeded game RNG
// Leaf: ??$random_shuffle@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00669850, 113 bytes (+ int3 padding to 128), `__fastcall` (first in ecx, last in
//   edx, the generator reference on the stack, `ret 4`). Only caller: 0x00667040 (in the unlabelled 0x00666EF0, which
//   builds the generator from CThing::GetRandomSeed 0x004C7AE0 in a frame slot and passes its address).
// Name: Ego_r.pdb name set has `random_shuffle<vector<CThing*>::iterator, (anonymous namespace 0xeff05290)::RandFunc>`
//   (the anonymous namespace of CThingFindHeightZBelowAt, this TU); retail's STLport vector iterator is the raw pointer.
// Callees: GFROR13 @ 0x00497890 (__fastcall rotate right by 13; landed as retail_asm).
// Label correction: the catalog "Shuffle_LinearCongruentialRNG" is descriptive; this is STLport random_shuffle.
// Shape: STLport 4.x verbatim `if (first == last) return; for (i = first + 1; i != last; ++i) iter_swap(i, first +
//   rand((i - first) + 1));` with RandFunc::operator() `return GFRandom(n, Seed);` over the functor's one member, the seed pointer, and the
//   landed __forceinline GFRandom (`*seed = *seed * 0x24a1 + 0x24df; *seed = GFROR13(*seed); return range ? *seed %
//   range : 0;`): through the by-value pointer the seed is loaded once and the rotated value reused (the same body on
//   the member re-reads Seed after every store, 124v113). The register
//   arguments are homed in the 8-byte frame (first and last re-read from it in the loop). No pragma.
class CThing;
unsigned long __fastcall GFROR13(unsigned long value);       // 0x00497890

// The landed GFRandom (0x005472A0 model): the seed pointer is a by-value parameter, so the rotated value stays in eax.
__forceinline unsigned long GFRandom(unsigned long range, unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return range ? *seed % range : 0;
}

namespace {
class RandFunc {
public:
    unsigned long* Seed;
    unsigned long operator()(unsigned long n) { return GFRandom(n, Seed); }
};
}

namespace std {
template <class _Tp> inline void swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}
template <class _ForwardIter1, class _ForwardIter2> inline void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2)
{
    swap(*__i1, *__i2);
}
template <class _RandomAccessIter, class _RandomNumberGenerator>
void __fastcall random_shuffle(_RandomAccessIter __first, _RandomAccessIter __last, _RandomNumberGenerator& __rand)
{
    if (__first == __last) return;
    for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
        iter_swap(__i, __first + __rand((__i - __first) + 1));
}
template void __fastcall random_shuffle(CThing**, CThing**, RandFunc&);
}