// Behaviour test for CModeStrafe::GetBestFrameInAimingAnim (0x008E9460): direction -> elevation fraction -> frame fraction.
#include <cstdio>
#include <cmath>

static bool BestFrame(float x, float y, float z, long numFrames, float& frame)
{
    float length2 = x * x + y * y + z * z;
    if (!(length2 > 1e-4f * 1e-4f))
        return false;
    float cosAngle = z * (1.0f / (float)std::sqrt(length2));
    if (cosAngle < -1.0f)
        cosAngle = -1.0f;
    else if (cosAngle > 1.0f)
        cosAngle = 1.0f;
    float angle = (float)std::acos(cosAngle) * 0.15915493667125702f;
    if (angle < 0.011111111f)
        angle = 0.011111111f;
    else if (angle > 0.41944444f)
        angle = 0.41944444f;
    float result = ((float)numFrames - 1.0f) * (angle - 0.011111111f) * 2.4489796f / (float)numFrames;
    if (result < 0.0f)
        result = 0.0f;
    else if (result > 1.0f)
        result = 1.0f;
    frame = result;
    return true;
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-3f; }
int main()
{
    float frame = -5.0f;
    int ok = !BestFrame(0.0f, 0.0f, 0.0f, 10, frame) && frame == -5.0f;      // degenerate direction
    ok = ok && BestFrame(0.0f, 0.0f, 1.0f, 10, frame) && Near(frame, 0.0f);  // straight up: lowest angle
    ok = ok && BestFrame(0.0f, 0.0f, -1.0f, 10, frame) && Near(frame, 0.9f); // straight down: clamped high, (n-1)/n
    ok = ok && BestFrame(1.0f, 0.0f, 0.0f, 11, frame);                       // horizontal: quarter turn
    ok = ok && Near(frame, 10.0f / 11.0f * (0.25f - 0.011111111f) * 2.4489796f);
    std::printf(ok ? "008e9460_TEST PASS\n" : "008e9460_TEST FAIL\n");
    return ok ? 0 : 1;
}