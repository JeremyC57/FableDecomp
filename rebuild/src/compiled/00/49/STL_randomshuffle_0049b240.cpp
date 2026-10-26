// std::random_shuffle(CCharString*, CCharString*, RandFunc&) -- STLport random shuffle of a CCharString array: each string from first+1 swaps (through a copy) with first[rand(i - first + 1)], RandFunc drawing through GFFloatRandom on the caller's seed
// Flags: /Gr
// Leaf: random_shuffle
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
// Label correction: catalogued as `VectorShuffle_Fisher`; the body is STLport `random_shuffle<CCharString*, RandFunc>`.
float __fastcall GFFloatRandom(float range, unsigned long* seed);    // 0x004978A0

class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

struct RandFunc {
    unsigned long* Seed;                            // +0x00
    long operator()(long n) { return (long)GFFloatRandom((float)n, Seed); }
};

template <class _Tp>
inline void __iter_swap_aux(_Tp* __a, _Tp* __b)
{
    _Tp __tmp = *__a;
    *__a = *__b;
    *__b = __tmp;
}

void __fastcall random_shuffle(CCharString* first, CCharString* last, RandFunc& rand)
{
    if (first == last) return;
    for (CCharString* i = first + 1; i != last; ++i)
        __iter_swap_aux(i, first + rand((i - first) + 1));
}