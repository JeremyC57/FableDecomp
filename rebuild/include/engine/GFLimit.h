#ifndef FABLE_ENGINE_GFLIMIT_H
#define FABLE_ENGINE_GFLIMIT_H
#include "rebuild_abi.h"
// Original template: both PDB name sets and FableWin body at donor 0x02d78c40.
template<class T, class L, class U>
#if defined(FABLE_RETAIL_WEAPON_TRAIL_DRAW_PASS29) && defined(_MSC_VER)
// Inline-budget: retail 0076e9b1 calls 0048dae0; visibility of the clamp keeps
// the weapon-trail index calculations in their native order around the call.
__declspec(noinline)
#else
inline
#endif
void FABLE_FASTCALL GFLimit(T& value, const L& lower, const U& upper)
{
    if (value < lower) value = lower;
    else if (value > upper) value = upper;
}
#ifdef FABLE_RETAIL_CTCTALK_VOICED_GROUP_PASS24
template <class T>
const T& FABLE_FASTCALL GFLimitReturn(const T& value, const T& lower, const T& upper);
#endif
#endif
