// Behaviour test for 0x004179EE CMainGameComponent::ChangeMaxTextureSize: the globals take the unsigned minimum, graphics re-initialised, both owners told.
#include <cstdio>
template <class T> inline const T& Min(const T& a, const T& b) { return b < a ? b : a; }
static int g_capW = 2048, g_capH = 1024;
static unsigned int g_maxW = 0, g_maxH = 0;
static int g_inits = 0, g_a = 0, g_b = 0;
static void ChangeMaxTextureSize(int size)
{
    g_maxW = Min<unsigned int>(size, g_capW);
    g_maxH = Min<unsigned int>(size, g_capH);
    ++g_inits; g_a = size; g_b = size;
}
int main()
{
    ChangeMaxTextureSize(1536);
    bool ok = g_maxW == 1536 && g_maxH == 1024 && g_inits == 1 && g_a == 1536 && g_b == 1536;
    ChangeMaxTextureSize(256);
    ok = ok && g_maxW == 256 && g_maxH == 256;
    ChangeMaxTextureSize(-1);                      // unsigned compare: -1 is huge, so the caps win
    ok = ok && g_maxW == 2048 && g_maxH == 1024;
    if (!ok) { std::printf("004179ee_TEST FAIL\n"); return 1; }
    std::printf("004179ee_TEST PASS\n");
    return 0;
}
