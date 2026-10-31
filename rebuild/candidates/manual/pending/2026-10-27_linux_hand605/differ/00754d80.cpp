// std::for_each(std::pair<float, const CAnimationEntry*>* first, std::pair<float, const CAnimationEntry*>* last, CGetBestTurningAnim f) -- picks the turning animation whose angle is closest to the wanted turn angle
// Grade: DIFFER(92v92) 9 lines (chunk 605, was 17): the functor body reads m_bestAngle into a float local, selects into it and writes it back unconditionally (`float best = m_bestAngle; if (...) { m_pBest = ...; best = anim.first; } m_bestAngle = best;`): the store is now FP (`fstp st(0) ; fld [edx]` on a hit, as retail) and the dead fxch pair is gone; left: VC7.1 does not hoist the per-iteration `fld [esp+14h]` / sink the `fstp [esp+14h]` out of the loop (retail keeps the angle on the x87 stack across the loop and compares with `fld st(1)`), so our compare reloads the member from memory. Tried in chunk 605: named angle local (94/20), plain double fabs compare (88), angle local + double fabs (90), best/angle locals (104), const best + member store in the branch (88), (float) casts on the best-local form (96), reversed compare (16 lines), assignment order (9), /Oa /Ow /Op /G7 (no gain); functor moved into the anonymous namespace (PDB `CGetBestTurningAnim@?A0x247890bc`) with no byte change.
// Leaf: ??$for_each@PAU?$pair@MPBVCAnimationEntry@@@std@@VCGetBestTurningAnim@?A0x
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
class C3DVector;
class CAnimationEntry;
namespace {
class CGetBestTurningAnim {
public:
    float m_wantedAngle;                          // +0x00
    float m_bestAngle;                            // +0x04
    const CAnimationEntry* m_pBest;               // +0x08
    CGetBestTurningAnim(const C3DVector& facing, const C3DVector& wanted);
    void operator()(const std::pair<float, const CAnimationEntry*>& anim)
    {
        float best = m_bestAngle;
        if (!m_pBest || fabs(anim.first - m_wantedAngle) < fabs(best - m_wantedAngle)) {
            m_pBest = anim.second;
            best = anim.first;
        }
        m_bestAngle = best;
    }
};
}
template CGetBestTurningAnim __fastcall std::for_each(std::pair<float, const CAnimationEntry*>*, std::pair<float, const CAnimationEntry*>*, CGetBestTurningAnim);
