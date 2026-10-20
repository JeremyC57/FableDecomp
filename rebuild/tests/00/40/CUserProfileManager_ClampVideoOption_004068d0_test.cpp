// Behaviour test for CUserProfileManager::ClampVideoOption (004068d0).
#include <cstdio>
static void ClampToLimit(float* out, float limit, float secondary)
{
    if (limit != -1.0f) { if (*out > limit) *out = limit; }
    else if (secondary != -1.0f) { if (*out > secondary) *out = secondary; }
    else *out = 0.0f;
}
static void ClampVideoOption(float* out, float value, float fallback, float limit, float secondary)
{
    if (value != -1.0f) *out = value;
    else if (fallback != -1.0f) { *out = fallback; ClampToLimit(out, limit, secondary); }
    else ClampToLimit(out, limit, secondary);
}
int main()
{
    float r = 9;
    ClampVideoOption(&r, 3, 1, 2, 5);   bool ok = r == 3;            // explicit value wins
    ClampVideoOption(&r, -1, 4, 2, 5);  ok = ok && r == 2;           // fallback clamped to limit
    ClampVideoOption(&r, -1, 4, -1, 5); ok = ok && r == 4;           // under the secondary limit
    ClampVideoOption(&r, -1, 7, -1, 5); ok = ok && r == 5;
    ClampVideoOption(&r, -1, 7, -1, -1); ok = ok && r == 0;          // no limits: zero
    r = 8; ClampVideoOption(&r, -1, -1, 6, -1); ok = ok && r == 6;   // current value clamped
    if (!ok) { std::printf("004068d0_TEST FAIL\n"); return 1; }
    std::printf("004068d0_TEST PASS\n");
    return 0;
}