#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SRect { long left, top, right, bottom; }; static long g_text = 0, g_flags = 0; static SRect g_rect;
struct IFont { virtual long DrawText(long text, const SRect* rect, long flags) = 0; }; struct CFont : IFont { virtual long DrawText(long text, const SRect* rect, long flags) { g_text = text; g_rect = *rect; g_flags = flags; return 99; } };
struct CFontOwner { unsigned char _pad[0x1d4]; IFont* m_pFont; long DrawTextInRect(long text, long left, long top, long right, long bottom) { SRect rect; rect.left = left; rect.top = top; rect.right = right; rect.bottom = bottom; return m_pFont->DrawText(text, &rect, 1); } };

int main() {
    CFont font; CFontOwner o; o.m_pFont = &font; if (o.DrawTextInRect(77, 1, 2, 3, 4) != 99 || g_text != 77 || g_rect.left != 1 || g_rect.top != 2 || g_rect.right != 3 || g_rect.bottom != 4 || g_flags != 1) { std::printf("0098c560_TEST FAIL\n"); return 1; }
    std::printf("0098c560_TEST PASS\n"); return 0;
}
