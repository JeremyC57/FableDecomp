#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#endif

static int g_calls = 0; static void* g_box = 0; static float g_radius = 0;
class C3DVector { public: float x, y, z; };
class CEngineWeatherRenderer { public: unsigned char _pad0[0x11a]; bool BoxVisibilityLocked; unsigned char _pad1[0x784 - 0x11b]; long SavedBoxVisibility[729];
    long ClipBoxToCamera(C3DVector* box, float radius) { ++g_calls; g_box = box; g_radius = radius; return 7; }
    long ClipLockedBoxToCamera(C3DVector* box, float radius, long index) { if (!BoxVisibilityLocked) SavedBoxVisibility[index] = ClipBoxToCamera(box, radius); return SavedBoxVisibility[index]; } };
int main()
{
    CEngineWeatherRenderer* r = (CEngineWeatherRenderer*)std::malloc(sizeof(CEngineWeatherRenderer)); std::memset(r, 0, sizeof(*r)); C3DVector b = { 1, 2, 3 };
    r->SavedBoxVisibility[5] = 3; r->BoxVisibilityLocked = true;
    bool ok = r->ClipLockedBoxToCamera(&b, 2.0f, 5) == 3 && g_calls == 0;
    r->BoxVisibilityLocked = false;
    ok = ok && r->ClipLockedBoxToCamera(&b, 2.0f, 5) == 7 && g_calls == 1 && g_box == &b && g_radius == 2.0f && r->SavedBoxVisibility[5] == 7;
    std::free(r);
    if (!ok) { std::printf("00b52120_TEST FAIL\n"); return 1; }
    std::printf("00b52120_TEST PASS\n"); return 0;
}