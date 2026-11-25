// Behaviour test for 0x00697860 CGameCameraManager::TellEngineThatCameraHasMoved: builds "telling engine that the camera has moved and" +
// (" " | " not ") + "using fast mode" (retail 0x0125C2C0 / 0x01258C1C / 0x0125C2B8 / 0x0125C2A8), then calls the 3D engine's slot 56
// (CIEngine::CutscenePreload) with (fastMode, true) through PMainGameComponent +0x28 -> +0x2c, and destroys the string.
// NF418 (2026-11-25): linked behaviour test: the row function is declared here, not defined, so the test links the row object and the
// callees below are defined under the row's spellings; it asserts the literal text the row passes (a mutated literal fails).
#include <cstdio>
#include <cstring>
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE
#define FABLE_RETAIL_CDISPLAYENGINE_GET3DENGINE
#define FABLE_RETAIL_CIENGINE
#include "engine/CIEngine.h"
#include "engine/CDisplayEngine.h"
#include "engine/CMainGameComponent.h"
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length);
    const CCharString& operator+=(const char* text);
    ~CCharString();
};
class CGameCameraManager { public: void TellEngineThatCameraHasMoved(bool fastMode); };
static char g_text[256]; static long g_len = 0; static int g_dtor = 0, g_calls = 0, g_textAtCall = 0; static bool g_a = false, g_b = false;
CCharString::CCharString(const char* t, long n) { m_pData = g_text; std::strcpy(g_text, t); g_len = n; }
const CCharString& CCharString::operator+=(const char* t) { std::strcat(g_text, t); return *this; }
CCharString::~CCharString() { ++g_dtor; }
static void __fastcall Slot56(void* self, int, bool a, bool b) { ++g_calls; g_a = a; g_b = b; g_textAtCall = g_dtor; (void)self; }
static void* g_vtbl[64]; static void* g_engine[4]; static char g_display[0x100]; static char g_mgc[0x100];
CMainGameComponent* CMainGameComponent::PMainGameComponent = (CMainGameComponent*)g_mgc;   // the protected static, defined here
int main()
{
    g_vtbl[56] = (void*)&Slot56; g_engine[0] = g_vtbl;
    *(void**)(g_display + 0x2c) = g_engine; *(void**)(g_mgc + 0x28) = g_display;
    CGameCameraManager m;
    m.TellEngineThatCameraHasMoved(true);
    if (std::strcmp(g_text, "telling engine that the camera has moved and using fast mode") || g_len != -1 || g_calls != 1 || !g_a || !g_b || g_dtor != 1 || g_textAtCall != 0) { std::printf("00697860_TEST FAIL 1 [%s]\n", g_text); return 1; }
    m.TellEngineThatCameraHasMoved(false);
    if (std::strcmp(g_text, "telling engine that the camera has moved and not using fast mode") || g_calls != 2 || g_a || !g_b || g_dtor != 2) { std::printf("00697860_TEST FAIL 2 [%s]\n", g_text); return 1; }
    std::printf("00697860_TEST PASS\n"); return 0;
}
