#include "engine/CTCBoastingPosition.h"

char __fastcall IsPositionActivated(CTCBoastingPosition* position, int /*edx*/, long* out)
{
    if (position->PositionActivated) {
        *out = position->BoastIndex;
    }
    return position->PositionActivated;
}
