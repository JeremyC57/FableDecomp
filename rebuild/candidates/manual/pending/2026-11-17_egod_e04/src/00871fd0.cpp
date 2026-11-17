// NCameraModes::CStringAndCageBase::UpdateXYVars() -- ease the current string length toward the desired one and the current cage radius toward the desired radius (toward the desired string length instead while the expansion timer runs, counting it down), each a rolling average of weight 3
// Leaf: ?UpdateXYVars@CStringAndCageBase@NCameraModes@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) 0x00871FD0, 124 bytes, private thiscall `ret`.
// Names: Ego_r.pdb / Ego_d.pdb ?UpdateXYVars@CStringAndCageBase@NCameraModes@@AAEXXZ (private); Ego_d camera-mode object: the one
//   function-scope float local new_cage_radius. Callees: NCameraModes::CBase::GetCamTools() const 0x0084D080 (result only used as
//   the object of the inline const template CTools::GetRollingAverage<float>, Ego_d / FableWin
//   ??$GetRollingAverage@M@CTools@NCameraModes@@QBEMABM0M@Z, expanded: (current * 3 + new) * 0.25 in the frame across the call).
// Shape: FITTED (chosen by grading; FableWin /Od size not reproduced): the helper's return expression is parenthesised (opt-in FABLE_RETAIL_NCAMERAMODES_CTOOLS_ROLLING_AVERAGE_PAREN with _ROLLING_OUTOFLINE:
//   the inline out-of-class body after the class): the extra
//   PAREN tuple lets c2 schedule `fld [esi+0ACh]` (0x00871FF5) ahead of the first result's dword copy `mov eax,[esp+4]`
//   (0x00871FFB); with the unparenthesised body those two swap (the residue of every earlier pass).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NCAMERAMODES_CBASE_TYPED_COMPOSITES
#define FABLE_RETAIL_NCAMERAMODES_CBASE_MEMBERS
#define FABLE_RETAIL_NCAMERAMODES_CBASE_STRING_CAGE
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS_ROLLING_AVERAGE
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS_ROLLING_OUTOFLINE
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS_ROLLING_AVERAGE_PAREN
#include "engine/NCameraModes_CStringAndCageBase.h"
#include "engine/NCameraModes_CTools.h"
void NCameraModes::CStringAndCageBase::UpdateXYVars()
{
    CurrentStringLength = GetCamTools().GetRollingAverage(CurrentStringLength, DesiredStringLength, 3.0f);
    float new_cage_radius = DesiredCageRadius;
    if (TimerForExpansion > 0) {
        new_cage_radius = DesiredStringLength;
        --TimerForExpansion;
    }
    CurrentCageRadius = GetCamTools().GetRollingAverage(CurrentCageRadius, new_cage_radius, 3.0f);
}
