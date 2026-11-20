// CPixelFormat::FindFormatInfoIndexFromD3DFormat(_D3DFORMAT format) -- index of the format's record in GD3DFormats, -1 at the end marker
// Leaf: ?FindFormatInfoIndexFromD3DFormat@CPixelFormat@@SIJW4_D3DFORMAT@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E3620, `__fastcall` (format in ecx); the walk scans GD3DFormats 0x0129BA40 by a
//   36-byte stride, the cursor at record +8 (0x0129BA48): the format dword at cursor-8, the -1 end marker at the next cursor.
// Naming (NF172, 2026-11-20, q11745): own leaf from the free stand-in ?FindFormatInfoIndexFromD3DFormat@@YIJH@Z to Ego_r.pdb 0x00B35470 /
//   FableWin.pdb 0x0308CAB0 ?FindFormatInfoIndexFromD3DFormat@CPixelFormat@@SIJW4_D3DFORMAT@@@Z (public static __fastcall), declared by
//   engine/CPixelFormat.h NF136 opt-in FABLE_RETAIL_CPIXELFORMAT_FINDFORMATINFOINDEX; its one landed caller 0x009BE420 binds it already.
//   The body is unchanged: the table cursor keeps the row's existing g_formatCursor (0x0129BA48, one of the descriptive spellings this
//   table already has; not respelled here).
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CPIXELFORMAT_FINDFORMATINFOINDEX
#include "engine/Direct3D9Types.h"
#include "engine/CPixelFormat.h"

extern int g_formatCursor[]; // symbol address == 0x129ba48

long __fastcall CPixelFormat::FindFormatInfoIndexFromD3DFormat(_D3DFORMAT fmt)
{
    long index = 0;
    int* edx = &g_formatCursor[0];
top:
    if (edx[-2] == (int)fmt)
        return index;
    edx += 9;              // advance to next record cursor
    ++index;
    if (edx[0] != -1)      // marker at new cursor (== old edx + 0x24)
        goto top;
    return -1;
}
