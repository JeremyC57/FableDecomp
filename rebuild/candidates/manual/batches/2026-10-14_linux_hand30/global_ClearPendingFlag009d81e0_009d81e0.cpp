// _global::ClearPendingFlag_009d81e0() -- clear a global byte flag if it is set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D81E0, length 17 bytes.
// Callees: none. The flag is the global byte at 0x013964A8. The chunk label "CDrawMouseCursor::Draw" is propagated over three unrelated bodies
// (this one, 0x006571D0 and 0x00CB2EE0); this body takes no arguments (`ret`), reads and clears a global, and sits in the 0x009Dxxxx engine
// module between two static-initialiser destructors, so it is named for what it does. Shape: `mov al,[flag] ; test al,al ; je ; mov byte ptr [flag],0`.
extern bool g_pendingFlag_013964a8;                // 0x013964A8

void ClearPendingFlag_009d81e0()
{
    if (g_pendingFlag_013964a8)
        g_pendingFlag_013964a8 = false;
}
