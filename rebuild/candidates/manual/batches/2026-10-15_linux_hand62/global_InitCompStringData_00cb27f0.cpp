// InitCompStringData() -- IME UI: clears the composition string, its attribute string and the two composition cursor positions
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB27F0, length 39 bytes.
// Leaf: InitCompStringData
// No callees: two intrinsic memsets (`rep stosd`, 0x80 and 0x40 dwords) and two dword stores sharing the eax zero.
// Identity: the manifest label "CPSCOrbit::DatabaseCopy" is propagated and rejected (no `this`, ecx is
// clobbered as the stosd count). The body sits among the DXUT ImeUi.cpp functions (manifest
// ImeUi_ProcessComposition 0x00CB2710, ImeUi_SetCompStringAppearance 0x00CB2880) and its callers
// (0x00CB4A6F, 0x00CB4B14, 0x00CB4CCD) are cases of the IME message handler; at 0x00CB4A6F the call is
// followed by `*trapped = TRUE` (`mov byte ptr [esi],1`) -- DXUT's WM_IME_STARTCOMPOSITION case,
// `InitCompStringData(); *trapped = TRUE;`. The globals match ImeUi.cpp's UNICODE build:
// TCHAR g_szCompositionString[256] (0x200 bytes @ 0x0143CF10), BYTE g_szCompAttrString[256] (@ 0x0143E518),
// DWORD g_IMECursorBytes / g_IMECursorChars (@ 0x0143E894 / 0x0143E898).
// The original is file-static in ImeUi.cpp; it is external here so the TU emits it.
#include <string.h>

extern unsigned short g_szCompositionString[256];  // 0x0143CF10 (TCHAR, UNICODE)
extern unsigned char  g_szCompAttrString[256];     // 0x0143E518
extern unsigned long  g_IMECursorBytes;            // 0x0143E894
extern unsigned long  g_IMECursorChars;            // 0x0143E898

void InitCompStringData()
{
    memset(g_szCompositionString, 0, sizeof(g_szCompositionString));
    memset(g_szCompAttrString, 0, sizeof(g_szCompAttrString));
    g_IMECursorBytes = 0;
    g_IMECursorChars = 0;
}
