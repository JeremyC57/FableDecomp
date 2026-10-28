// Behaviour test: _HyperlinkProc (0x0099D660) -- WM_DESTROY restores the procedure and font and drops the props,
// WM_MOUSEMOVE sets the hover flag + capture on entry and clears it when the cursor leaves the window rectangle,
// WM_SETCURSOR shows the hand (arrow fallback) and returns TRUE, everything else goes to the original procedure.
#include <cstdio>
static int g_HyperlinkHovered;
static bool g_captured, g_handAvailable;
static int g_forwarded, g_props, g_cursor, g_invalidates;
static const int RECT_L = 0, RECT_T = 0, RECT_R = 100, RECT_B = 20;
static long Proc(unsigned int message, long lParam)
{
    switch (message) {
    case 0x0002: g_props = 0; break;
    case 0x0200:
        if (!g_captured) { g_HyperlinkHovered = 1; ++g_invalidates; g_captured = true; }
        else {
            int x = (unsigned short)lParam, y = (unsigned short)((unsigned long)lParam >> 16);
            if (!(x >= RECT_L && x < RECT_R && y >= RECT_T && y < RECT_B)) { g_HyperlinkHovered = 0; ++g_invalidates; g_captured = false; }
        }
        break;
    case 0x0020:
        g_cursor = g_handAvailable ? 32649 : 32512;
        return 1;
    }
    ++g_forwarded;
    return 7;
}
int main()
{
    g_props = 4;
    bool ok = Proc(0x0002, 0) == 7 && g_props == 0 && g_forwarded == 1;
    ok = ok && Proc(0x0200, 5 | (5 << 16)) == 7 && g_HyperlinkHovered == 1 && g_captured;
    ok = ok && Proc(0x0200, 50 | (10 << 16)) == 7 && g_HyperlinkHovered == 1 && g_captured;
    ok = ok && Proc(0x0200, 500 | (10 << 16)) == 7 && g_HyperlinkHovered == 0 && !g_captured && g_invalidates == 2;
    g_handAvailable = true; ok = ok && Proc(0x0020, 0) == 1 && g_cursor == 32649;
    g_handAvailable = false; ok = ok && Proc(0x0020, 0) == 1 && g_cursor == 32512;
    ok = ok && Proc(0x000F, 0) == 7 && g_forwarded == 5;
    if (!ok) { std::printf("0099d660_TEST FAIL\n"); return 1; }
    std::printf("0099d660_TEST PASS\n");
    return 0;
}