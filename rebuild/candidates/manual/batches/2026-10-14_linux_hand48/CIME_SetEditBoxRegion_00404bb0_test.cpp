#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SRect { long left, top, right, bottom; }; static long g_calls = 0, g_colour = 0; static void* g_font = 0; static SRect g_rect;
static void ImeUi_SetCompStringAppearance(void* font, long colour, const SRect* rect) { ++g_calls; g_font = font; g_colour = colour; g_rect = *rect; }
struct CIME { unsigned char _pad[8]; bool m_active; unsigned char _pad09[0x13]; void* m_pFont;
    void SetEditBoxRegion(long x, long y, long width, long height) { if (m_active) { SRect rect; rect.left = x; rect.top = y; rect.right = x + width; rect.bottom = y + height; ImeUi_SetCompStringAppearance(m_pFont, -1, &rect); } } };

int main() {
    CIME ime; std::memset(&ime, 0, sizeof ime); int font; ime.m_pFont = &font; ime.m_active = true; ime.SetEditBoxRegion(10, 20, 30, 40);
    if (g_calls != 1 || g_font != &font || g_colour != -1 || g_rect.left != 10 || g_rect.top != 20 || g_rect.right != 40 || g_rect.bottom != 60) { std::printf("00404bb0_TEST FAIL\n"); return 1; } ime.m_active = false; ime.SetEditBoxRegion(1, 2, 3, 4); if (g_calls != 1) { std::printf("00404bb0_TEST FAIL\n"); return 1; }
    std::printf("00404bb0_TEST PASS\n"); return 0;
}
