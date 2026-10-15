// CFrameStreamer_00c497e0::OnFrameDone(void*, void*, CFrameStreamer_00c497e0* streamer) -- stream callback: pop a frame from the free stack at +0x48; if there is one, free its data block, clear it and post the frame (by address) to the mailbox queue at +0x1c
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C497E0, length 58 bytes.
// Leaf: OnFrameDone
// Callees: the scoped-lock Pop @ 0x00C4A0E0 (landed), the `__stdcall` free thunk @ 0x00C36090 (landed), the mailbox queue's
// ProcessMessage(void*) @ 0x00C49B90 (landed). The only reference to this body is `push offset 0x00C497E0` inside the
// audio streamer's initialiser @ 0x00C49A2F, which registers it with the stream handle as (handle, [esi+3Ch], callback,
// userdata = the streamer): a `__stdcall` callback with three arguments (`ret 0Ch`) whose first two are unused. The chunk
// label "CTCActionUseScriptedHook::SetEntranceAsLocked" is propagated and rejected; the class is named by address.
// Shape: the popped frame is an address-taken local, so VC7.1 homes it in the dead third-argument slot (`mov [esp+10h],eax`)
// and reloads it after the free call.
void __stdcall FreeBlock_00c36090(void* block);   // 0x00C36090

struct SFrame {
    unsigned char _pad[0xc];
    void* m_pData;                             // +0x0c
};

struct CFrameStack {
    SFrame* Pop();                             // 0x00C4A0E0
};

struct CMailboxQueue {                         // mailbox at +0x18, semaphore at +0x1c (landed 0x00C49B90)
    unsigned char _body[0x20];
    long ProcessMessage(void* message);        // 0x00C49B90
};

struct CFrameStreamer_00c497e0 {
    unsigned char _pad[0x1c];
    CMailboxQueue m_doneQueue;                 // +0x1c
    unsigned char _pad3c[0xc];
    CFrameStack   m_freeFrames;                // +0x48
    static void __stdcall OnFrameDone(void* handle, void* buffer, CFrameStreamer_00c497e0* streamer);
};

void __stdcall CFrameStreamer_00c497e0::OnFrameDone(void*, void*, CFrameStreamer_00c497e0* streamer)
{
    SFrame* frame = streamer->m_freeFrames.Pop();
    if (frame) {
        FreeBlock_00c36090(frame->m_pData);
        frame->m_pData = 0;
        streamer->m_doneQueue.ProcessMessage(&frame);
    }
}