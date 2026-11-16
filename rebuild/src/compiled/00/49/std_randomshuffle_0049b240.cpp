// std::random_shuffle(CCharString*, CCharString*, RandFunc&) -- STLport random shuffle of a CCharString array: each string from first+1 swaps (through a copy) with first[rand(i - first + 1)], RandFunc drawing through GFFloatRandom on the caller's seed
// Flags: /Gr
// Leaf: ??$random_shuffle@PAVCCharString@@URandFunc@@@std@@YIXPAVCCharString@@0AAURandFunc@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049B240, 133 bytes, `ret 4` (__fastcall: first in ecx -> ebp, last in edx
//   (spilled to a frame word), the RandFunc reference on the stack; the 12-byte frame holds last, the int -> float
//   conversion word and the swap temporary).
// Callees: 0x004978A0 GFFloatRandom(float, unsigned long*), 0x00BFEA70 __ftol2, 0x0099EC30 CCharString copy ctor,
//   0x0099EFB0 CCharString::operator=, 0x0099EAE0 ~CCharString.
// Callers: CProgressDisplay::InitialiseTextures 0x0049AA23 / 0x0049AA32 (two vector<CCharString> shuffles with one
//   RandFunc frame local).
// Identity: FableWin.pdb names `random_shuffle<vector<CCharString>::iterator, RandFunc>` and `??RRandFunc@@QAEJJ@Z`;
//   Ego_r.pdb RandFunc { unsigned long* Seed; }.
// Shape: STLport `if (first == last) return; for (i = first + 1; i != last; ++i) iter_swap(i, first + rand((i - first)
//   + 1));` with the inline `RandFunc::operator()` and STLport's `__iter_swap_aux` (`_Tp tmp = *a; *a = *b; *b = tmp;`).
// Name (round 4, NF40): the STLport template instance in namespace std, `random_shuffle<CCharString*, RandFunc>` (FableWin.pdb
//   random_shuffle<vector<..>::iterator, RandFunc>; the caller 0x0049A0B0 already binds this spelling through stlport/fable_containers.h).
// Label correction: catalogued as `VectorShuffle_Fisher`; the body is STLport `random_shuffle<CCharString*, RandFunc>`.
float __fastcall GFFloatRandom(float range, unsigned long* seed);    // 0x004978A0

class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

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

template void __fastcall random_shuffle<CCharString*, RandFunc>(CCharString*, CCharString*, RandFunc&);
}
