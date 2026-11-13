// NCameraModes::CStringAndCageBase::UpdateXYVars() -- smooth string length and cage radius, honoring expansion delay.
// Grade: DIFFER(124v124) -- DesiredCageRadius load follows the first result reload; native orders those two instructions oppositely.
// Leaf: ?UpdateXYVars@CStringAndCageBase@NCameraModes@@AAEXXZ
// Retail: 0x00871fd0, 124 bytes, private thiscall.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NCAMERAMODES_CBASE_TYPED_COMPOSITES
#define FABLE_RETAIL_NCAMERAMODES_CBASE_MEMBERS
#define FABLE_RETAIL_NCAMERAMODES_CBASE_STRING_CAGE
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS_ROLLING_AVERAGE
#include "engine/NCameraModes_CStringAndCageBase.h"
#include "engine/NCameraModes_CTools.h"
void NCameraModes::CStringAndCageBase::UpdateXYVars()
{
    CurrentStringLength = GetCamTools().GetRollingAverage(CurrentStringLength,DesiredStringLength,3.0f);
    float new_cage_radius = DesiredCageRadius;
    if (TimerForExpansion > 0) {
        new_cage_radius = DesiredStringLength;
        --TimerForExpansion;
    }
    CurrentCageRadius = GetCamTools().GetRollingAverage(CurrentCageRadius,new_cage_radius,3.0f);
}
