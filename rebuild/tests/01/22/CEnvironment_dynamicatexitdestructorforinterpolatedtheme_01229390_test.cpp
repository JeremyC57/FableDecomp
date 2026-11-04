// Behaviour test for CEnvironment::RenderUpdate's `dynamic atexit destructor for 'interpolated_theme'' 0x01229390: the function-local static is built on
// first use only (one construction however often the owner runs) and the registered exit stub destroys exactly that object once.
#include <cstdio>
#include <cstdlib>
static int g_ctor = 0, g_dtor = 0;
static const void* g_dtorThis = 0;
struct CBlendedEnvironmentTheme {
    const char* Text;
    explicit CBlendedEnvironmentTheme(const char* t) : Text(t) { ++g_ctor; }
    ~CBlendedEnvironmentTheme() { ++g_dtor; g_dtorThis = this; }
};
static const CBlendedEnvironmentTheme* g_static = 0;
static const CBlendedEnvironmentTheme& Owner()
{
    static CBlendedEnvironmentTheme interpolated_theme("blend");
    g_static = &interpolated_theme;
    return interpolated_theme;
}
static void Check()
{
    if (g_ctor != 1 || g_dtor != 1 || g_dtorThis != g_static) { std::printf("01229390_TEST FAIL\n"); std::fflush(stdout); return; }
    std::printf("01229390_TEST PASS\n"); std::fflush(stdout);
}
int main()
{
    std::atexit(Check);           // registered first, so it runs after the static's exit stub
    Owner(); Owner(); Owner();
    if (g_ctor != 1 || g_dtor != 0) { std::printf("01229390_TEST FAIL\n"); return 1; }
    return 0;
}