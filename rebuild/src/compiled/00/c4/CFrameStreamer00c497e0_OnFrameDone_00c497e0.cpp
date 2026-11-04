// CFrameStreamer_00c497e0::OnFrameDone(void*, void*, CFrameStreamer_00c497e0* streamer) -- stream callback: pop a frame from the free stack at +0x48; if there is one, free its data block, clear it and post the frame (by address) to the mailbox queue at +0x1c
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C497E0, length 58 bytes.
// Leaf: OnFrameDone
// Vendor naming V2 (plan phase 1, one address, one name): 0x00C4A0E0 ?Pop@CLockedRingBuffer@@QAEPAUCCodaFrame@@XZ (was ?Pop@CFrameStack@@QAEPAUSFrame@@XZ); 0x00C36090 ?StdcallForward_00c3cde0@@YGXPAX@Z (was ?FreeBlock_00c36090@@YGXPAX@Z); 0x00C49B90 ?ProcessMessage_CMailboxQueue_00c49b90@CMailboxQueue_00c49b90@@QAEJPAX@Z (was ?ProcessMessage@CMailboxQueue@@QAEJPAX@Z)
// Callees: the scoped-lock Pop @ 0x00C4A0E0 (landed), the `__stdcall` free thunk @ 0x00C36090 (landed), the mailbox queue's
// ProcessMessage(void*) @ 0x00C49B90 (landed). The only reference to this body is `push offset 0x00C497E0` inside the
// audio streamer's initialiser @ 0x00C49A2F, which registers it with the stream handle as (handle, [esi+3Ch], callback,
// userdata = the streamer): a `__stdcall` callback with three arguments (`ret 0Ch`) whose first two are unused. The chunk
// label "CTCActionUseScriptedHook::SetEntranceAsLocked" is propagated and rejected; the class is named by address.
// Shape: the popped frame is an address-taken local, so VC7.1 homes it in the dead third-argument slot (`mov [esp+10h],eax`)
// and reloads it after the free call.
void __stdcall StdcallForward_00c3cde0(void* block);   // 0x00C36090

struct CCodaFrame {
    unsigned char _pad[0xc];
    void* m_pData;                             // +0x0c
};

struct CLockedRingBuffer {
    CCodaFrame* Pop();                             // 0x00C4A0E0
};

struct CMailboxQueue_00c49b90 {                         // mailbox at +0x18, semaphore at +0x1c (landed 0x00C49B90)
    unsigned char _body[0x20];
    long ProcessMessage_CMailboxQueue_00c49b90(void* message);        // 0x00C49B90
};

struct CFrameStreamer_00c497e0 {
    unsigned char _pad[0x1c];
    CMailboxQueue_00c49b90 m_doneQueue;                 // +0x1c
    unsigned char _pad3c[0xc];
    CLockedRingBuffer   m_freeFrames;                // +0x48
    static void __stdcall OnFrameDone(void* handle, void* buffer, CFrameStreamer_00c497e0* streamer);
};

void __stdcall CFrameStreamer_00c497e0::OnFrameDone(void*, void*, CFrameStreamer_00c497e0* streamer)
{
    CCodaFrame* frame = streamer->m_freeFrames.Pop();
    if (frame) {
        StdcallForward_00c3cde0(frame->m_pData);
        frame->m_pData = 0;
        streamer->m_doneQueue.ProcessMessage_CMailboxQueue_00c49b90(&frame);
    }
}