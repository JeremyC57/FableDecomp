#include <cstdio>
#include <cstring>
#include <cstdlib>

static int g_base = 0; static float g_x = 3.0f, g_y = 4.0f;
struct CSprite { virtual ~CSprite() { } unsigned char pad[0x30]; void Initialise() { ++g_base; } };
struct CMouseCursor : CSprite { float px, py; void Initialise() { CSprite::Initialise(); px = g_x; py = g_y; } };
int main()
{
    CMouseCursor c; c.px = 0; c.py = 0; c.Initialise(); if (g_base != 1 || c.px != 3.0f || c.py != 4.0f) { std::printf("0055c560_TEST FAIL\n"); return 1; }
    g_x = -1.0f; c.Initialise(); if (c.px != -1.0f || c.py != 4.0f) { std::printf("0055c560_TEST FAIL\n"); return 1; }
    std::printf("0055c560_TEST PASS\n");
    return 0;
}
