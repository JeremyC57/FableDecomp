// Leaf: ?GFGetSign@@YIJM@Z
// Flags:
#define FABLE_RETAIL_NAV_ROUTE_SIGN_PASS30
#include "rebuild_abi.h"
#include "engine/GFAngles.h"

long FABLE_FASTCALL GFGetSign(float value)
{
    return value < 0.0f ? -1 : 1;
}
