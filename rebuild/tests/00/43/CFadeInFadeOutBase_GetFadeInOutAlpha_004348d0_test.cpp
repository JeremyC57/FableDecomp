#include <cstdio>
#include <cstdlib>
#include <cstring>
static const float g_fadeEpsilon_0129ba3c = 1e-4f;
struct CFadeInFadeOutBase { bool m_active; unsigned char _pad01[3]; float m_fadeOutDuration; float m_fadeInDuration; bool m_fadingIn; bool m_fadingOut; unsigned char _pad0e[2]; float m_fadeOutElapsed; float m_fadeInElapsed;
    float GetFadeInOutAlpha() { if (m_fadingOut) { if (m_fadeOutDuration < g_fadeEpsilon_0129ba3c) return 1.0f; return m_fadeOutElapsed / m_fadeOutDuration; }
        if (m_fadingIn) { if (m_fadeInDuration < g_fadeEpsilon_0129ba3c) return 1.0f; return m_fadeInElapsed / m_fadeInDuration; } if (m_active) return 1.0f; return 0.0f; } };
int main() {
    CFadeInFadeOutBase f; std::memset(&f, 0, sizeof f); f.m_fadingOut = true; f.m_fadeOutDuration = 2.0f; f.m_fadeOutElapsed = 0.5f; if (f.GetFadeInOutAlpha() != 0.25f) { std::printf("004348d0_TEST FAIL\n"); return 1; }
    f.m_fadeOutDuration = 0.0f; if (f.GetFadeInOutAlpha() != 1.0f) { std::printf("004348d0_TEST FAIL\n"); return 1; }
    f.m_fadingOut = false; f.m_fadingIn = true; f.m_fadeInDuration = 4.0f; f.m_fadeInElapsed = 1.0f; if (f.GetFadeInOutAlpha() != 0.25f) { std::printf("004348d0_TEST FAIL\n"); return 1; }
    f.m_fadingIn = false; if (f.GetFadeInOutAlpha() != 0.0f) { std::printf("004348d0_TEST FAIL\n"); return 1; } f.m_active = true; if (f.GetFadeInOutAlpha() != 1.0f) { std::printf("004348d0_TEST FAIL\n"); return 1; }
    std::printf("004348d0_TEST PASS\n"); return 0;
}