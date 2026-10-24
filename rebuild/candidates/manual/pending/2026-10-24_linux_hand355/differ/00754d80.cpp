// std::for_each(std::pair<float, const CAnimationEntry*>* first, std::pair<float, const CAnimationEntry*>* last, CGetBestTurningAnim f) -- picks the turning animation whose angle is closest to the wanted turn angle
// Leaf: for_each
// Grade: DIFFER(88v92) -- retail keeps m_bestAngle on the x87 stack across the loop (`fld [f+4]` before, `fstp st(0) ; fld [edx]` on a hit, `fstp [f+4]` after, plus a dead `fxch st(1)` pair); ours stores it with integer moves each hit
// Retail: Fable.exe (TLC, VC7.1) @ 0x00754D80, 92 bytes, `ret 10h`. Caller: 0x00753743 (CAnimateThing turning-animation setup).
// SGI for_each, /Gr `__fastcall` (hidden functor result ecx, first edx, last + the 12-byte functor on the stack).
// Tried: plain member stores, an inline by-value SetBest(entry, angle), a memberwise user copy ctor (worse copy-out),
// an FP-typed store (`(float)fabs(...)`) -- none makes VC7.1 register-promote the float member.
extern "C" double __cdecl fabs(double);
#pragma intrinsic(fabs)
namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
template <class InputIter, class Function>
Function __fastcall for_each(InputIter first, InputIter last, Function f)
{
    for ( ; first != last; ++first)
        f(*first);
    return f;
}
}
class CAnimationEntry;
class CGetBestTurningAnim {
public:
    float m_wantedAngle;                          // +0x00
    float m_bestAngle;                            // +0x04
    const CAnimationEntry* m_pBest;               // +0x08
    CGetBestTurningAnim(float wantedAngle) : m_wantedAngle(wantedAngle), m_bestAngle(0), m_pBest(0) {}
    void operator()(const std::pair<float, const CAnimationEntry*>& anim)
    {
        if (!m_pBest || fabs(anim.first - m_wantedAngle) < fabs(m_bestAngle - m_wantedAngle)) {
            m_pBest = anim.second;
            m_bestAngle = anim.first;
        }
    }
};
template CGetBestTurningAnim __fastcall std::for_each(std::pair<float, const CAnimationEntry*>*, std::pair<float, const CAnimationEntry*>*, CGetBestTurningAnim);
