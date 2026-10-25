// Behaviour test for 0x00CB2F00 ImeUi::_PumpMessage: while a message is waiting, GetMessage + Translate + Dispatch;
// a WM_QUIT (GetMessage 0) is re-posted with its exit code and ends the pump.
#include <cstdio>

struct MSG_ { void* hwnd; unsigned int message; unsigned int wParam; int lParam; };

static unsigned int g_queue[8];
static int g_head = 0, g_tail = 0;
static int g_translated = 0, g_dispatched = 0, g_quitCode = -1, g_quitPosts = 0;
static int PeekMessage_(MSG_* m, void*, unsigned int, unsigned int, unsigned int remove)
{
    if (g_head == g_tail) return 0;
    if (remove == 0) { m->message = g_queue[g_head]; return 1; }
    return 0;
}
static int GetMessage_(MSG_* m, void*, unsigned int, unsigned int)
{
    unsigned int msg = g_queue[g_head++];
    m->message = msg;
    m->wParam = msg == 0x12 ? 42 : 0;
    return msg != 0x12;
}
static int TranslateMessage_(const MSG_*) { ++g_translated; return 1; }
static int DispatchMessage_(const MSG_*) { ++g_dispatched; return 0; }
static void PostQuitMessage_(int code) { ++g_quitPosts; g_quitCode = code; }

static void _PumpMessage()
{
    MSG_ msg;
    while (PeekMessage_(&msg, 0, 0, 0, 0)) {
        if (!GetMessage_(&msg, 0, 0, 0)) {
            PostQuitMessage_(msg.wParam);
            return;
        }
        TranslateMessage_(&msg);
        DispatchMessage_(&msg);
    }
}

int main()
{
    g_queue[g_tail++] = 0x100; g_queue[g_tail++] = 0x102; g_queue[g_tail++] = 0x101;
    _PumpMessage();
    if (g_translated != 3 || g_dispatched != 3 || g_quitPosts != 0 || g_head != 3) { std::printf("00cb2f00_TEST FAIL\n"); return 1; }
    g_queue[g_tail++] = 0x200; g_queue[g_tail++] = 0x12; g_queue[g_tail++] = 0x201;
    _PumpMessage();
    if (g_dispatched != 4 || g_quitPosts != 1 || g_quitCode != 42 || g_head != 5) { std::printf("00cb2f00_TEST FAIL\n"); return 1; }
    std::printf("00cb2f00_TEST PASS\n");
    return 0;
}