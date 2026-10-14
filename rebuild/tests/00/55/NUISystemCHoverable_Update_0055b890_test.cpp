#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
static char g_seq[64] = "";
struct IComponentA { virtual void va00() { } unsigned char _state[0x10]; }; struct IComponentB { virtual void vb00() { } }; struct CComponentRoot { virtual void v000() { } };
static float g_dt = 0; static const float g_eps = 1e-4f, g_zero = 0.0f;
struct CChangingStateComponent : CComponentRoot, IComponentA, IComponentB { unsigned char _body[0x30 - 0x1c]; float m_hoverTimer, m_storedX, m_storedY, m_hoverX, m_hoverY; void Update(float dt) { std::strcat(g_seq, "U"); g_dt = dt; } };
struct CHoverable : CChangingStateComponent { virtual void OnHoverMoved() { std::strcat(g_seq, "M"); }
    void Update(float dt) { CChangingStateComponent::Update(dt); if (!((float)std::fabs(m_hoverX - m_storedX) <= g_eps) || !((float)std::fabs(m_hoverY - m_storedY) <= g_eps) || m_hoverTimer - dt == g_zero) OnHoverMoved(); } };

int main() {
    CHoverable h; h.m_hoverTimer = 5; h.m_storedX = 1; h.m_storedY = 2; h.m_hoverX = 1; h.m_hoverY = 2; h.Update(1.0f); if (std::strcmp(g_seq, "U") || g_dt != 1.0f) { std::printf("0055b890_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; h.m_hoverX = 1.5f; h.Update(1.0f); if (std::strcmp(g_seq, "UM")) { std::printf("0055b890_TEST FAIL\n"); return 1; } g_seq[0] = 0; h.m_hoverX = 1; h.m_hoverY = 2.5f; h.Update(1.0f); if (std::strcmp(g_seq, "UM")) { std::printf("0055b890_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; h.m_hoverY = 2; h.Update(5.0f); if (std::strcmp(g_seq, "UM")) { std::printf("0055b890_TEST FAIL\n"); return 1; } g_seq[0] = 0; h.m_hoverX = 1.00001f; h.Update(1.0f); if (std::strcmp(g_seq, "U")) { std::printf("0055b890_TEST FAIL\n"); return 1; }
    std::printf("0055b890_TEST PASS\n"); return 0;
}