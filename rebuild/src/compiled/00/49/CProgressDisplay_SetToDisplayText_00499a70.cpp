// CProgressDisplay::SetToDisplayText(bool enabled) -- vtable slot 5: store the flag; when enabled pick the next text tag, else
// clear the text tag (operator=(const char*) with the shared empty literal at 0x0122D70E) and reset the tag index.
// Leaf: ?SetToDisplayText@CProgressDisplay@@UAEX_N@Z
// Retail: Fable.exe @ 0x00499A70, 47 bytes, `ret 4`. Name: FableWin + Ego_r S_PUB32 ?SetToDisplayText@CProgressDisplay@@UAEX_N@Z
// (public virtual; RTTI vtable 0x012388B4 slot 5). Callees: CProgressDisplay::CalculateNextTextTag @ 0x00497B60 (private, both PDBs
// ?CalculateNextTextTag@CProgressDisplay@@AAEXXZ), CCharString::operator=(const char*) @ 0x0099EFE0.
// Layout: engine/CProgressDisplay.h (Ego_r): DisplayText +0x7C, TextTagIndex +0x44, TextTag +0x4C.
#include "engine/CProgressDisplay.h"

extern const char g_FableEmptyText_0122D70E[];   // 0x0122D70E: the pooled empty literal (spelling kept from HEAD)

void CProgressDisplay::SetToDisplayText(bool enabled)
{
    DisplayText = enabled;
    if (enabled)
    {
        CalculateNextTextTag();
        return;
    }

    TextTag = g_FableEmptyText_0122D70E;
    TextTagIndex = 0;
}
