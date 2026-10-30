// Behaviour test for the DXUT ImeUi.cpp statics SendControlKeys (0x00CB26B0), CancelCompString (0x00CB2710),
// DrawCaret (0x00CB2820) and SetSupportLevel (0x00CB2D90), against a recording message sink.
#include <cstdio>
static int g_msgs[64][2]; static int g_n; static bool bIsSendingKeyMessage;
static long SendKeyMsg(int, unsigned int msg, unsigned int wp) { bIsSendingKeyMessage = true; g_msgs[g_n][0] = msg; g_msgs[g_n][1] = wp; g_n++; bIsSendingKeyMessage = false; return 0; }
static void SendControlKeys(unsigned int vk, unsigned int num)
{
    if (num == 0) return;
    for (unsigned int i = 0; i < num; i++) SendKeyMsg(0, 0x100, vk);
    SendKeyMsg(0, 0x101, vk);
}
static unsigned long g_dwIMELevel, g_IMECursorChars; static bool g_bInsertMode; static int g_cc;
static void CancelCompString(int hwnd, bool bUseBackSpace, int iNewStrLen)
{
    if (g_dwIMELevel != 3) return;
    int cc = g_cc;
    SendControlKeys(0x27, cc - g_IMECursorChars);
    if (bUseBackSpace || g_bInsertMode) iNewStrLen = 0;
    if (iNewStrLen < cc) { for (int i = 0; i < cc - iNewStrLen; i++) { SendKeyMsg(hwnd, 0x100, 8); SendKeyMsg(hwnd, 0x102, 8); } SendKeyMsg(hwnd, 0x101, 8); }
    else iNewStrLen = cc;
    SendControlKeys(0x25, iNewStrLen);
}
static bool g_bCaretDraw; static int g_rect[5];
static void DrawRect(int x1, int y1, int x2, int y2, unsigned long c) { g_rect[0] = x1; g_rect[1] = y1; g_rect[2] = x2; g_rect[3] = y2; g_rect[4] = (int)c; }
static void DrawCaret(unsigned long x, unsigned long y, unsigned long height)
{
    const unsigned char caretWidth = 2, caretYMargin = 1;
    if (g_bCaretDraw) DrawRect(x, y + caretYMargin, x + caretWidth, y + height - caretYMargin, 0xff);
}
static unsigned long g_hkl; static int g_finalized, g_colored;
static void SetSupportLevel(unsigned long dwImeLevel)
{
    if (dwImeLevel < 2 || 3 < dwImeLevel) return;
    if ((unsigned short)(g_hkl & 0x3ff) == 0x12) dwImeLevel = 3;
    g_dwIMELevel = dwImeLevel; g_finalized++; g_colored++;
}
int main()
{
    SendControlKeys(0x27, 2);
    if (g_n != 3 || g_msgs[0][0] != 0x100 || g_msgs[2][0] != 0x101 || bIsSendingKeyMessage) { std::printf("00cb26b0_TEST FAIL\n"); return 1; }
    g_n = 0; g_dwIMELevel = 3; g_cc = 3; g_IMECursorChars = 1;
    CancelCompString(0, true, 2);   // 2 x RIGHT + up, 3 x (down, char) + up, no LEFT
    if (g_n != 3 + 7 || g_msgs[3][1] != 8 || g_msgs[4][0] != 0x102) { std::printf("00cb26b0_TEST FAIL\n"); return 1; }
    g_bCaretDraw = true; DrawCaret(10, 20, 30);
    if (g_rect[0] != 10 || g_rect[1] != 21 || g_rect[2] != 12 || g_rect[3] != 49) { std::printf("00cb26b0_TEST FAIL\n"); return 1; }
    g_hkl = 0x0412; SetSupportLevel(2);
    if (g_dwIMELevel != 3 || g_finalized != 1) { std::printf("00cb26b0_TEST FAIL\n"); return 1; }
    SetSupportLevel(5);
    if (g_finalized != 1) { std::printf("00cb26b0_TEST FAIL\n"); return 1; }
    std::printf("00cb26b0_TEST PASS\n"); return 0;
}