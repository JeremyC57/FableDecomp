// Mass::BackplaneFriction(float strength, float radius) -- reduce linear and angular speeds by friction.
// Leaf: ?BackplaneFriction@Mass@@QAEXMM@Z
// Flags:
// Retail: 0x008eebe0, 131 bytes, member call with two float arguments.
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_NORMALISE
#define FABLE_RETAIL_MASS_BACKPLANEFRICTION
#include "engine/Mass.h"
void Mass::BackplaneFriction(float strength, float radius)
{
    float speed = Vel.GetMagnitude();
    Vel *= speed <= strength ? 0.0f : 1.0f - strength / speed;
    float angular = strength / radius;
    float aspeed = (float)fabs(AVel);
    float scale;
    if (aspeed <= angular) scale = 0.0f; else scale = (1.0f - angular / aspeed);
    AVel *= scale;
}