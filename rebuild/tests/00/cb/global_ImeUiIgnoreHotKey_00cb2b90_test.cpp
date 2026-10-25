// Behaviour test for 0x00CB2B90 ImeUi::ImeUi_IgnoreHotKey over the retail 22-entry hot-key table (DXUT's list
// without the later ATOK Ctrl+Tab entry): VK_PROCESSKEY key-downs only; VK_JUNJA always, VK_RIGHT in a CHT candidate
// list, else the Ctrl/Shift/Alt state + real key must be in the table.
#include <cstdio>

struct MSG_ { void* hwnd; unsigned int message; unsigned int wParam; int lParam; };
struct SImeHotKey { bool m_bCtrl, m_bShift, m_bAlt; unsigned int m_uVk; };

static SImeHotKey aHotKeys[22] = {
    { false, false, false, 0x5D }, { true, false, false, '8' }, { true, false, false, 'Y' },
    { true, false, false, 0x2E }, { true, false, false, 0x76 }, { true, false, false, 0x78 },
    { true, false, false, 0x79 }, { true, false, false, 0x7A }, { true, false, false, 0x7B },
    { false, false, false, 0x71 }, { false, false, false, 0x72 }, { false, false, false, 0x73 },
    { false, false, false, 0x74 }, { false, false, false, 0x79 }, { false, true, false, 0x75 },
    { false, true, false, 0x76 }, { false, true, false, 0x77 }, { true, true, false, 0x79 },
    { true, true, false, 0x7A }, { true, false, false, 0x1C }, { true, false, false, 0x20 },
    { true, false, true, 0xBC },
};
static bool g_bInitialized = true, g_bCandList = false;
static unsigned int g_hklCurrent = 0x04090409;
static unsigned int g_realVk = 0;
static bool g_ctrl = false, g_shift = false, g_alt = false;
static unsigned int ImmGetVirtualKey_(void*) { return g_realVk; }
static short GetKeyState_(int vk)
{
    bool down = vk == 0x11 ? g_ctrl : vk == 0x10 ? g_shift : vk == 0x12 ? g_alt : false;
    return down ? (short)0x8001 : 0;
}

static bool ImeUi_IgnoreHotKey(const MSG_* pmsg)
{
    if (!g_bInitialized || !pmsg)
        return false;
    if (pmsg->wParam == 0xE5 && (pmsg->message == 0x0100 || pmsg->message == 0x0104)) {
        unsigned int uVkReal = ImmGetVirtualKey_(pmsg->hwnd);
        if (uVkReal == 0x17)
            return true;
        if (uVkReal == 0x27 && g_bCandList && (unsigned short)(g_hklCurrent & 0xffff) == 0x0404)
            return true;
        bool bCtrl = (GetKeyState_(0x11) & 0x8000) ? true : false;
        bool bShift = (GetKeyState_(0x10) & 0x8000) ? true : false;
        bool bAlt = (GetKeyState_(0x12) & 0x8000) ? true : false;
        for (int i = 0; i < (int)(sizeof(aHotKeys) / sizeof(aHotKeys[0])); i++)
            if (aHotKeys[i].m_bCtrl == bCtrl && aHotKeys[i].m_bShift == bShift && aHotKeys[i].m_bAlt == bAlt &&
                aHotKeys[i].m_uVk == uVkReal)
                return true;
    }
    return false;
}

static bool Try(unsigned int msg, unsigned int wp, unsigned int vk, bool c, bool s, bool a)
{
    MSG_ m = { 0, msg, wp, 0 };
    g_realVk = vk; g_ctrl = c; g_shift = s; g_alt = a;
    return ImeUi_IgnoreHotKey(&m);
}

int main()
{
    bool ok = true;
    ok = ok && !ImeUi_IgnoreHotKey(0);
    ok = ok && Try(0x100, 0xE5, 0x17, false, false, false);           // VK_JUNJA
    ok = ok && !Try(0x101, 0xE5, 0x17, false, false, false);          // key-up: ignored
    ok = ok && !Try(0x100, 'A', 0x17, false, false, false);           // not VK_PROCESSKEY
    ok = ok && !Try(0x100, 0xE5, 0x27, false, false, false);          // VK_RIGHT, no CHT candidate list
    g_bCandList = true; g_hklCurrent = 0x04040404;
    ok = ok && Try(0x104, 0xE5, 0x27, false, false, false);           // VK_RIGHT in a CHT candidate list
    g_bCandList = false;
    ok = ok && Try(0x100, 0xE5, 0x5D, false, false, false);           // VK_APPS
    ok = ok && !Try(0x100, 0xE5, 0x5D, true, false, false);           // Ctrl+VK_APPS is not listed
    ok = ok && Try(0x100, 0xE5, 'Y', true, false, false);             // Ctrl+Y
    ok = ok && Try(0x100, 0xE5, 0x77, false, true, false);            // Shift+F8
    ok = ok && Try(0x100, 0xE5, 0xBC, true, false, true);             // Ctrl+Alt+','
    ok = ok && !Try(0x100, 0xE5, 0x09, true, false, false);           // Ctrl+Tab: not in this drop's table
    g_bInitialized = false;
    ok = ok && !Try(0x100, 0xE5, 0x17, false, false, false);
    if (!ok) { std::printf("00cb2b90_TEST FAIL\n"); return 1; }
    std::printf("00cb2b90_TEST PASS\n");
    return 0;
}