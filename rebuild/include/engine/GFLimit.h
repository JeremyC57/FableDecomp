#ifndef FABLE_ENGINE_GFLIMIT_H
#define FABLE_ENGINE_GFLIMIT_H
#include "rebuild_abi.h"
// Original template: both PDB name sets and FableWin body at donor 0x02d78c40.
template<class T, class L, class U>
inline void FABLE_FASTCALL GFLimit(T& value, const L& lower, const U& upper)
{
    if (value < lower) value = lower;
    else if (value > upper) value = upper;
}
#endif
