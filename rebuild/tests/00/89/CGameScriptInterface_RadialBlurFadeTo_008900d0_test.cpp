extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
// Behaviour test for 0x008900D0 CGameScriptInterface::RadialBlurFadeTo (C3DVector overload): the call is forwarded to the
// environment's spell-effect interpolator with priority 1 and the arguments in order; its key is returned.
class C3DVector { public: float X, Y, Z; };
struct CInterp { int priority; float t, a, b, c, d; C3DVector center, area;
    void* RadialBlurFadeTo(float time, int pri, C3DVector ce, float a_, C3DVector ar, float b_, float c_, float d_)
    { t = time; priority = pri; center = ce; a = a_; area = ar; b = b_; c = c_; d = d_; return this; } };
static void* RadialBlurFadeTo(CInterp& i, float time, C3DVector center, float a, C3DVector area, float b, float c, float d) { return i.RadialBlurFadeTo(time, 1, center, a, area, b, c, d); }
int main()
{
    CInterp i; C3DVector c1 = { 1, 2, 3 }, c2 = { 4, 5, 6 };
    void* k = RadialBlurFadeTo(i, 0.5f, c1, 7.0f, c2, 8.0f, 9.0f, 10.0f);
    CHECK(k == &i && i.priority == 1 && i.t == 0.5f && i.center.Z == 3 && i.a == 7.0f && i.area.X == 4 && i.b == 8.0f && i.c == 9.0f && i.d == 10.0f);
    if (fails == 0) printf("008900d0_TEST PASS\n"); else printf("008900d0_TEST FAIL\n");
    return fails;
}