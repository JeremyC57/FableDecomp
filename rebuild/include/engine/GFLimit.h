#ifndef FABLE_ENGINE_GFLIMIT_H
#define FABLE_ENGINE_GFLIMIT_H
#include "rebuild_abi.h"
#ifdef FABLE_RETAIL_GET_OPINIONS_PASS30
template<class T>
inline const T& FABLE_FASTCALL GFLimitReturn(const T& value, const T& lower, const T& upper)
{
    return value < lower ? lower : value > upper ? upper : value;
}
#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
template<> inline const float& FABLE_FASTCALL GFLimitReturn(const float& value, const float& lower, const float& upper)
{
    return value < lower ? lower : value > upper ? upper : value;
}
#ifdef _MSC_VER
#pragma optimize("",on)
#endif
#endif
// Original template: both PDB name sets and FableWin body at donor 0x02d78c40.
#ifdef FABLE_RETAIL_HERO_APPEARANCE_UPDATE_PASS30
// UpdateAppearanceStats calls the clamp owner and expands the lower limit.
template<class T, class L, class U>
void FABLE_FASTCALL GFLimit(T& value, const L& lower, const U& upper);

template<class T, class L>
inline void FABLE_FASTCALL GFLimitLower(T& value, const L& lower)
{
    if (value < lower)
        value = lower;
}
#else
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
#endif
#ifdef FABLE_RETAIL_CTCTALK_VOICED_GROUP_PASS24
template <class T>
const T& FABLE_FASTCALL GFLimitReturn(const T& value, const T& lower, const T& upper);
#endif
#ifdef FABLE_RETAIL_SUMMONER_ORB_LIMIT_UPPER
// FableWin 01A93F40 compares the referenced floats and stores the upper bound.
template<class T, class U>
inline void FABLE_FASTCALL GFLimitUpper(T& value, const U& upper)
{
    if (value > upper) value = upper;
}
#endif
#endif
