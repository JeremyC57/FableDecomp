#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_text[128]; static bool g_fast = false; static long g_flag = 0; static int g_dtor = 0;
static const char g_text_0125c2c0[] = "telling engine that the camera has moved and"; static const char g_text_01258c1c[] = " "; static const char g_text_0125c2b8[] = " not "; static const char g_text_0125c2a8[] = "using fast mode";
struct CCharString { char* m_pData; CCharString(const char* t, long) { m_pData = g_text; std::strcpy(g_text, t); } CCharString& operator+=(const char* t) { std::strcat(g_text, t); return *this; } ~CCharString() { ++g_dtor; } };
struct CRenderer_00697860 { virtual void SetCameraMoved(bool f, long fl) { g_fast = f; g_flag = fl; } };
struct CEngineDevice_00697860 { unsigned char _pad[0x2c]; CRenderer_00697860* m_pRenderer; };
struct CEngine_00697860 { unsigned char _pad[0x28]; CEngineDevice_00697860* m_pDevice; };
static CEngine_00697860* g_pEngine_013b86a0 = 0;
struct CGameCameraManager { void TellEngineThatCameraHasMoved(bool fastMode) { CCharString message(g_text_0125c2c0, -1); message += fastMode ? g_text_01258c1c : g_text_0125c2b8; message += g_text_0125c2a8; g_pEngine_013b86a0->m_pDevice->m_pRenderer->SetCameraMoved(fastMode, 1); } };
int main() {
    CRenderer_00697860 r; CEngineDevice_00697860 dev; dev.m_pRenderer = &r; CEngine_00697860 eng; eng.m_pDevice = &dev; g_pEngine_013b86a0 = &eng; CGameCameraManager m;
    m.TellEngineThatCameraHasMoved(true); if (std::strcmp(g_text, "telling engine that the camera has moved and using fast mode") || !g_fast || g_flag != 1 || g_dtor != 1) { std::printf("00697860_TEST FAIL\n"); return 1; }
    m.TellEngineThatCameraHasMoved(false); if (std::strcmp(g_text, "telling engine that the camera has moved and not using fast mode") || g_fast || g_dtor != 2) { std::printf("00697860_TEST FAIL\n"); return 1; }
    std::printf("00697860_TEST PASS\n"); return 0;
}