// test for 00416365 PostChangeResolution
#include <stdio.h>
class CDisplayEngine {
public:
    void PostChangeResolution(unsigned long width, unsigned long height, unsigned long depth);   // 0x00434A20
};
class CWorld {
public:
    void PostChangeResolution(unsigned long width, unsigned long height, unsigned long depth);   // 0x0049D820
};
class CMainGameComponent {
public:
    void PostChangeResolution(unsigned long width, unsigned long height, unsigned long depth);
    unsigned char   _pad00[0x24];
    CWorld*         PWorld;                              // +0x24 scoped_ptr<CWorld>
    CDisplayEngine* PDisplayEngine;                      // +0x28 scoped_ptr<CDisplayEngine>
};
void CMainGameComponent::PostChangeResolution(unsigned long width, unsigned long height, unsigned long depth)
{
    PDisplayEngine->PostChangeResolution(width, height, depth);
    PWorld->PostChangeResolution(width, height, depth);
}

static int g_order, g_d, g_w; static unsigned long g_args[6];
void CDisplayEngine::PostChangeResolution(unsigned long a, unsigned long b, unsigned long c) { g_d = ++g_order; g_args[0] = a; g_args[1] = b; g_args[2] = c; }
void CWorld::PostChangeResolution(unsigned long a, unsigned long b, unsigned long c) { g_w = ++g_order; g_args[3] = a; g_args[4] = b; g_args[5] = c; }
int main()
{
    static CMainGameComponent m; CWorld w; CDisplayEngine d; m.PWorld = &w; m.PDisplayEngine = &d;
    m.PostChangeResolution(640, 480, 32);
    int ok = g_d == 1 && g_w == 2 && g_args[0] == 640 && g_args[1] == 480 && g_args[2] == 32 && g_args[3] == 640 && g_args[5] == 32;
    printf(ok ? "00416365_TEST PASS\n" : "00416365_TEST FAIL\n");
    return 0;
}