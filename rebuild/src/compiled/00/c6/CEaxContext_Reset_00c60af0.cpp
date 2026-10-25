// CEaxContext::Reset(SEaxEvent** ppEvents) -- drop the EAX context's queued notifications, re-invalidate every source (InvalidateAll) unless a full resend is already pending (sign bit of +0x3b4), and hand back the fresh event queue; returns InvalidateAll's status (0 when skipped)
// Leaf: ?Reset@CEaxContext@@QAEJPAPAUSEaxEvent@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C60AF0, 47 bytes, thiscall `ret 4`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 442 "ResetMessageList_00c60af0 = context reset (its model says 0xC60A30
//   returns a node, it returns a long status)"): the callees are agent442's landed CEaxContext::ClearEvents 0x00C60410 and
//   CEaxContext::InvalidateAll 0x00C60A30 (`long`: 0 or the first failing source's MarkAllDirty status; it sets the
//   +0x3b4 resend bit itself and may queue a type-2 environment event at +0x3c4). The out-parameter receives the +0x3c4
//   queue like CEaxSource::Set 0x00C61B40 (`ClearEvents ; ... ; *ppEvents = m_pContext->m_pEvents`). No direct callers.
//   The landed ResetMessageList_00c60af0 / CMessageOwner names (and the manifest's
//   "CCreatureAction_BanditKingLunge::OnAnimationLoaded") are propagated; descriptive names (libpk: no PDB/RTTI).
// Shape: the result is declared (and zeroed in callee-saved edi) ahead of the ClearEvents call; the resend flag is
//   tested on its sign bit (`test eax,eax ; js`); the queue head is re-read after the call and stored through the
//   out-pointer.
struct SEaxEvent;

class CEaxContext {                                 // vtable 0x012B9340 (the vptr is not modelled here)
public:
    unsigned char _vptr[4];                         // +0x000
    unsigned char _pad004[0x3b0];                   // +0x004
    unsigned long m_sourceFlags;                    // +0x3b4 sign bit: every source must re-send its properties
    unsigned char _pad3b8[0xc];                     // +0x3b8
    SEaxEvent*    m_pEvents;                        // +0x3c4 queued notifications
    void ClearEvents();                             // 0x00C60410
    long InvalidateAll();                           // 0x00C60A30
    long Reset(SEaxEvent** ppEvents);
};

long CEaxContext::Reset(SEaxEvent** ppEvents)
{
    long result = 0;
    ClearEvents();
    if (!(m_sourceFlags & 0x80000000))
        result = InvalidateAll();
    *ppEvents = m_pEvents;
    return result;
}
