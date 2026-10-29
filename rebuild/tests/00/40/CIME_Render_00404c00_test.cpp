#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
static unsigned char g_alpha = 0; struct SThemeRecord { unsigned char _pad[3]; unsigned char m_alpha; }; static SThemeRecord g_theme = { { 0, 0, 0 }, 0x77 };
struct CThemeProvider { virtual SThemeRecord* GetTheme() = 0; }; struct CProvider : CThemeProvider { virtual SThemeRecord* GetTheme() { std::strcat(g_seq, "T"); return &g_theme; } };
static void ImeUI_RenderCompositionString(bool a, bool b) { std::strcat(g_seq, "R"); std::strcat(g_seq, a ? "1" : "0"); std::strcat(g_seq, b ? "1" : "0"); }
struct CSprite { void DrawWithRenderManager() { std::strcat(g_seq, "S"); } };
namespace NUISystem { class CManager { public: unsigned char _pad[0xb8]; CSprite* m_pImeSprite; CSprite* GetImeSprite() { return m_pImeSprite; } static CManager* __fastcall GetInstance(); }; } static NUISystem::CManager g_manager; NUISystem::CManager* NUISystem::CManager::GetInstance() { return &g_manager; }
struct CIME { unsigned char _pad[8]; bool m_active; unsigned char _pad09[0x43]; CThemeProvider* m_pTheme;
    void Render() { if (m_active) { if (m_pTheme) g_alpha = m_pTheme->GetTheme()->m_alpha; else g_alpha = 0; bool flags = true; ImeUI_RenderCompositionString(flags, flags); CSprite* sprite = NUISystem::CManager::GetInstance()->GetImeSprite(); if (sprite) sprite->DrawWithRenderManager(); } } };

int main() {
    CIME ime; std::memset(&ime, 0, sizeof ime); CProvider prov; CSprite sprite; ime.m_active = true; ime.m_pTheme = &prov; g_manager.m_pImeSprite = &sprite; g_alpha = 9;
    ime.Render(); if (g_alpha != 0x77 || std::strcmp(g_seq, "TR11S")) { std::printf("00404c00_TEST FAIL\n"); return 1; } g_seq[0] = 0; ime.m_pTheme = 0; g_manager.m_pImeSprite = 0; ime.Render(); if (g_alpha != 0 || std::strcmp(g_seq, "R11")) { std::printf("00404c00_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; ime.m_active = false; ime.Render(); if (g_seq[0]) { std::printf("00404c00_TEST FAIL\n"); return 1; }
    std::printf("00404c00_TEST PASS\n"); return 0;
}