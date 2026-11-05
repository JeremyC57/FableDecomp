// Leaf: ?CTCBoastingPosition_IsPositionActivated@@YIDPAVCTCBoastingPosition@@HPAJ@Z
#include "engine/CTCBoastingPosition.h"

char __fastcall CTCBoastingPosition_IsPositionActivated(CTCBoastingPosition* position, int /*edx*/, long* out)
{
    if (position->PositionActivated) {
        *out = position->BoastIndex;
    }
    return position->PositionActivated;
}
