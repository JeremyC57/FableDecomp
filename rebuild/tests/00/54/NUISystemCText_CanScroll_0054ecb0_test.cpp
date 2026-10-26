// Behaviour test for NUISystem::CText::CanScroll (0x0054ECB0): after the refresh call, scrolling down measures
// the UI-scaled text height plus the position against the bottom edge; otherwise the position against the top
// edge; zero when there is no room either way.
#include <cstdio>
class C2DVector { public: float X, Y; C2DVector() { } };
class C2DBoxF { public: float TLX, TLY, BRX, BRY; };
static const float g_zero = 0.0f;
static float g_scaleY = 1.0f; static int g_scaleCalls = 0;
namespace NUISystem { class CManager { public: C2DVector GetUIScale() { ++g_scaleCalls; C2DVector v; v.X = 9.0f; v.Y = g_scaleY; return v; } }; }
static NUISystem::CManager g_manager;
struct CFrontEndManager { static NUISystem::CManager* GetInstance() { return &g_manager; } };
static int g_refreshes = 0; static void* g_refreshContext = 0; static int g_positionCalls = 0;
namespace NUISystem {
class CText {
public:
    virtual C2DVector GetPosition() { ++g_positionCalls; C2DVector v; v.X = -1.0f; v.Y = m_posY; return v; }
    virtual void vslot135(void* context) { ++g_refreshes; g_refreshContext = context; }
    unsigned char _pad04[0xd0 - 0x04];
    float         m_textHeight;
    unsigned char _padd4[0x148 - 0xd4];
    void*         m_pScrollContext;
    float         m_posY;
    float CanScroll(bool down, const C2DBoxF& bounds);
};
}
float NUISystem::CText::CanScroll(bool down, const C2DBoxF& bounds)
{
    vslot135(m_pScrollContext);
    if (down) {
        C2DVector extent;
        extent.Y = m_textHeight;
        extent.Y *= CFrontEndManager::GetInstance()->GetUIScale().Y;
        if (extent.Y + GetPosition().Y > bounds.BRY)
            return bounds.BRY - (extent.Y + GetPosition().Y);
    } else {
        if (GetPosition().Y < bounds.TLY)
            return bounds.TLY - GetPosition().Y;
    }
    return g_zero;
}
static int fail(const char* why) { std::printf("0054ecb0_TEST FAIL: %s\n", why); return 1; }
int main()
{
    static NUISystem::CText text; static int context; text.m_pScrollContext = &context; text.m_textHeight = 100.0f; text.m_posY = 50.0f;
    C2DBoxF bounds = { 0.0f, 10.0f, 640.0f, 120.0f };
    // 1. down: 100 * 1.0 + 50 = 150 > 120 -> 120 - 150 = -30
    float r = text.CanScroll(true, bounds);
    if (r != -30.0f) return fail("down overflow");
    if (g_refreshes != 1 || g_refreshContext != &context || g_scaleCalls != 1 || g_positionCalls != 2) return fail("down call pattern");
    // 2. down with the UI scale halving the height: 50 + 50 = 100 <= 120 -> zero
    g_scaleY = 0.5f;
    if (text.CanScroll(true, bounds) != 0.0f) return fail("down fits");
    // 3. up: position 50 >= top 10 -> zero
    g_positionCalls = 0;
    if (text.CanScroll(false, bounds) != 0.0f || g_positionCalls != 1 || g_scaleCalls != 2) return fail("up fits");
    // 4. up: position -5 < 10 -> 10 - (-5) = 15
    text.m_posY = -5.0f;
    if (text.CanScroll(false, bounds) != 15.0f || g_refreshes != 4) return fail("up overflow");
    std::printf("0054ecb0_TEST PASS\n");
    return 0;
}