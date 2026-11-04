// Behaviour test for CEngine::ConvertScreenPosToWorldPosAtViewDistance (0x00B24400), g++ model of the retail logic.
#include <cstdio>
#include <cmath>
struct M { float E11,E12,E13,E21,E22,E23,E31,E32,E33,E41,E42,E43; };
struct Cam { float ow, oh, cx, cy; M inv; };
static bool Convert(bool active, const Cam& c, float d, float sx, float sy, float out[3])
{
    if (!active) return false;
    float x = (sx - c.cx) * c.ow * d, y = (c.cy - sy) * c.oh * d, z = d;
    out[0] = c.inv.E11 * x + (c.inv.E21 * y + c.inv.E31 * z) + c.inv.E41;
    out[1] = c.inv.E12 * x + c.inv.E22 * y + c.inv.E32 * z + c.inv.E42;
    out[2] = c.inv.E13 * x + c.inv.E23 * y + c.inv.E33 * z + c.inv.E43;
    return true;
}
int main()
{
    Cam c = { 0.01f, 0.02f, 320, 240, { 1,0,0, 0,1,0, 0,0,1, 5,6,7 } };
    float o[3] = { -1, -1, -1 };
    int ok = !Convert(false, c, 10, 0, 0, o) && o[0] == -1;
    ok &= Convert(true, c, 10, 420, 140, o);
    ok &= std::fabs(o[0] - (1.0f * 10 + 5)) < 1e-4f && std::fabs(o[1] - (2.0f * 10 + 6)) < 1e-4f && std::fabs(o[2] - 17) < 1e-4f;
    std::printf(ok ? "00b24400_TEST PASS\n" : "00b24400_TEST FAIL\n");
    return ok ? 0 : 1;
}