extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
// Behaviour test for 0x00890110 CGameScriptInterface::RadialBlurFadeTo (float overload): forwarded to the spell-effect
// interpolator with priority 1 and the six floats in order; its key is returned.
struct CInterp { int priority; float v[7];
    void* RadialBlurFadeTo(float t, int p, float a, float b, float c, float d, float e, float f)
    { priority = p; v[0] = t; v[1] = a; v[2] = b; v[3] = c; v[4] = d; v[5] = e; v[6] = f; return this; } };
static void* RadialBlurFadeTo(CInterp& i, float t, float a, float b, float c, float d, float e, float f) { return i.RadialBlurFadeTo(t, 1, a, b, c, d, e, f); }
int main()
{
    CInterp i; void* k = RadialBlurFadeTo(i, 1, 2, 3, 4, 5, 6, 7);
    CHECK(k == &i && i.priority == 1);
    for (int n = 0; n < 7; ++n) CHECK(i.v[n] == (float)(n + 1));
    if (fails == 0) printf("00890110_TEST PASS\n"); else printf("00890110_TEST FAIL\n");
    return fails;
}