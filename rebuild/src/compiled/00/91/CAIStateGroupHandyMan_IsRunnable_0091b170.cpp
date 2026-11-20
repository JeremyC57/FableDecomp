// CAIStateGroup_HandyMan::IsRunnable() -- runnable in work hours when a work spot can be acquired
// Leaf: ?IsRunnable@CAIStateGroup_HandyMan@@UAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091B170, 24 bytes (virtual slot 11).
// NF131 (2026-11-20, q4946): the stand-in CAIStateGroup_HandyMan_Methods over the flat generated header and the free stand-ins
//   helper_91a6f0 / tail_91b010 give way to the retail class (engine/CAIStateGroup_HandyMan.h FABLE_RETAIL_CAISTATEGROUP_HANDYMAN, on
//   engine/CAIStateGroup_WorkBase.h + the retail engine/CAIStateGroupBase.h). Own name: ?IsRunnable@CAIStateGroup_HandyMan@@UAE_NXZ
//   (Ego_r.pdb 0x0093EA50 + FableWin.pdb 0x02BCB360), was ?IsRunnable@CAIStateGroup_HandyMan_Methods@@QAE_NXZ. Callees:
//   ?IsInWorkHours@?$CAIStateGroup_WorkBase@VCAIStateGroupBase@@@@IBE_NXZ 0x0091A6F0 (Ego_r 0x0093E1A0 + FableWin 0x02BA9910) and the tail
//   call ?AcquireWorkSpot@CAIStateGroup_HandyMan@@AAE_NXZ 0x0091B010 (Ego_r 0x0093E8C0 + FableWin 0x02BCB790; the landed self row's spelling).
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_HANDYMAN
#include "engine/CAIStateGroup_HandyMan.h"

bool CAIStateGroup_HandyMan::IsRunnable()
{
    if (IsInWorkHours())
        return AcquireWorkSpot();
    return false;
}
