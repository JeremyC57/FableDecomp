// Behaviour test for CModeHoverBase::InitialiseMovementSpeeds (0x008EA500): tier scale per animation-name suffix.
#include <cstdio>
#include <cstring>
#include <cmath>

static bool EndsWith(const char* s, const char* suffix)
{
    long offset = (long)std::strlen(s) - (long)std::strlen(suffix);
    if (offset >= 0)
        return std::strcmp(s + offset, suffix) == 0;
    return false;
}
static float Scale(float animSpeed, float defSpeed)
{
    if (animSpeed < 1e-4f || defSpeed < 1e-4f)
        return 1.0f;
    return defSpeed / animSpeed;
}
static float StateSpeed(const char* name, float animSpeed, float slow, float medium, float fast)
{
    float scale = 0.0f;
    if (EndsWith(name, "_SLOW")) scale = slow;
    else if (EndsWith(name, "_MEDIUM")) scale = medium;
    else if (EndsWith(name, "_FAST")) scale = fast;
    return animSpeed * scale;
}
int main()
{
    float slow = Scale(2.0f, 4.0f);      // 2
    float medium = Scale(0.0f, 4.0f);    // degenerate: 1
    float fast = Scale(5.0f, 10.0f);     // 2
    int ok = slow == 2.0f && medium == 1.0f && fast == 2.0f
        && StateSpeed("HOVER_SLOW", 3.0f, slow, medium, fast) == 6.0f
        && StateSpeed("HOVER_MEDIUM", 3.0f, slow, medium, fast) == 3.0f
        && StateSpeed("X_FAST", 1.5f, slow, medium, fast) == 3.0f
        && StateSpeed("HOVER", 3.0f, slow, medium, fast) == 0.0f
        && StateSpeed("_SLO", 3.0f, slow, medium, fast) == 0.0f;
    std::printf(ok ? "008ea500_TEST PASS\n" : "008ea500_TEST FAIL\n");
    return ok ? 0 : 1;
}