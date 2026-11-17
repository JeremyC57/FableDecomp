// CCodaFrameStreamer::OnFrameDone(void*, void*, CCodaFrameStreamer* streamer) -- stream callback: pop a frame from the free stack at +0x48; if there is one, free its data block, clear it and post the frame (by address) to the mailbox queue at +0x1c
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C497E0, length 58 bytes.
// Leaf: ?OnFrameDone@CCodaFrameStreamer@@SGXPAX0PAV1@@Z
// Vendor naming V2 (plan phase 1, one address, one name): 0x00C4A0E0 ?Pop@CLockedRingBuffer@@QAEPAUCCodaFrame@@XZ (was ?Pop@CFrameStack@@QAEPAUSFrame@@XZ); 0x00C36090 ?StdcallForward_00c3cde0@@YGXPAX@Z (was ?FreeBlock_00c36090@@YGXPAX@Z); 0x00C49B90 ?ProcessMessage_CMailboxQueue_00c49b90@CMailboxQueue_00c49b90@@QAEJPAX@Z (was ?ProcessMessage@CMailboxQueue@@QAEJPAX@Z)
// Callees: the scoped-lock Pop @ 0x00C4A0E0 (landed), the `__stdcall` free thunk @ 0x00C36090 (landed), the mailbox queue's
// ProcessMessage(void*) @ 0x00C49B90 (landed). The only reference to this body is `push offset 0x00C497E0` inside the
// audio streamer's initialiser @ 0x00C49A2F, which registers it with the stream handle as (handle, [esi+3Ch], callback,
// userdata = the streamer): a `__stdcall` callback with three arguments (`ret 0Ch`) whose first two are unused. The chunk
// label "CTCActionUseScriptedHook::SetEntranceAsLocked" is propagated and rejected; the class is named by address.
// Shape: the popped frame is an address-taken local, so VC7.1 homes it in the dead third-argument slot (`mov [esp+10h],eax`)
// and reloads it after the free call.
// agentNF57 (q10495): moved onto libpk/CCodaFrameStreamer.h (overlay opt-in FABLE_RETAIL_CCODAFRAMESTREAMER_ONFRAMEDONE: the static
//   OnFrameDone and CLockedRingBuffer::Pop). The class key of CCodaFrame is `class` (the header, from the native CCodaFrame.cpp trace;
//   the landed SubmitFrame 0x00C49470 binds PAVCCodaFrame), so Pop 0x00C4A0E0 binds ?Pop@CLockedRingBuffer@@QAEPAVCCodaFrame@@XZ.
//   The streamer is the header's CCodaFrameStreamer (its ctor 0x00C49820 registers this callback; +0x1C is its CMailboxQueue
//   m_freeFrames, +0x48 its CLockedRingBuffer m_inFlight, the frame's +0x0C its m_hBuffer). Was the CFrameStreamer_00c497e0 /
//   CMailboxQueue_00c49b90 / struct CCodaFrame stand-ins.
#define FABLE_RETAIL_CCODAFRAMESTREAMER_PASS23
#define FABLE_RETAIL_CCODAFRAMESTREAMER_ONFRAMEDONE
#include "libpk/CCodaFrameStreamer.h"

void __stdcall StdcallForward_00c3cde0(void* block);   // 0x00C36090

void __stdcall CCodaFrameStreamer::OnFrameDone(void*, void*, CCodaFrameStreamer* streamer)
{
    CCodaFrame* frame = streamer->m_inFlight.Pop();
    if (frame) {
        StdcallForward_00c3cde0(frame->m_hBuffer);
        frame->m_hBuffer = 0;
        streamer->m_freeFrames.ProcessMessage(&frame);
    }
}
