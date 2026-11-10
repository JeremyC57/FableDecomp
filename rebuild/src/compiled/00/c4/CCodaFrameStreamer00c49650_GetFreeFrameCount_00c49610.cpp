// CCodaFrameStreamer_00c49650::GetFreeFrameCount() -- slot 1 of the GameCODA frame streamer: how many frames wait in its done-frame mailbox (+0x1c) to be filled again (the mailbox's locked GetCount)
// Leaf: ?GetFreeFrameCount@CCodaFrameStreamer_00c49650@@UAEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C49610, 8 bytes, thiscall, plain `ret` through the tail call: `add ecx,1Ch ; jmp 0x00C49C30`.
// Identity: slot 1 of the streamer vtable 0x012B8138 (the only reference, cell 0x012B813C): the vtable the landed constructor
//   0x00C49820 and destructor 0x00C49650 store (libpk .\CCodaFrameStreamer.cpp). The +0x1c member is the done-frame mailbox the
//   constructor builds with CMailboxQueue::CMailboxQueue 0x00C49B30 (over the PKCStaticQueue<CCodaFrame*> at +8) and posts every
//   new frame into (ProcessMessage 0x00C49B90). Its sibling slot 2 0x00C49620 returns `m_frames (+0x3c) - <this count>` (the
//   frames handed to the source); slot 6 is the deleting destructor 0x00C4A170.
// Callee: 0x00C49C30 = the mailbox's GetCount: `PKCProtectionInstance lock(this); return m_pQueue->GetCount();` (slot 2 of the
//   PKCQueue at +0x18), byte-identical to the landed PKCMailbox::GetCount 0x00C4BE10. It is landed under the propagated
//   CV_StatueMasterScript::GetAngleBasedOnTimeOfDay and bound by 0x00C49620 as @FableCount_c49620@4; respelled here as
//   CMailboxQueue::GetCount (the class the landed constructor/ProcessMessage bindings give the +0x1c member; NAME_FIXES.tsv).
// libpk has no PDB or RTTI: descriptive names.
class CCodaFrame;

struct CMailboxQueue {                              // +0x1c: critical section +0, PKCQueue<CCodaFrame*>* +0x18, semaphore +0x1c
    unsigned char m_body[0x20];
    long GetCount();                                // 0x00C49C30
};

struct CStreamerBase_00c49650 {                     // vtable 0x012B80AC
    virtual void Update();                          // slot 0 (0x00C49470 in the streamer)
    virtual long GetFreeFrameCount();               // slot 1
};

struct CCodaFrameStreamer_00c49650 : CStreamerBase_00c49650 {   // vtable 0x012B8138
    void*         m_pSource;                        // +0x04
    unsigned char m_frameQueue[0x14];               // +0x08 PKCStaticQueue<CCodaFrame*>
    CMailboxQueue m_doneQueue;                      // +0x1c
    unsigned long m_frames;                         // +0x3c
    virtual long GetFreeFrameCount();               // slot 1
};

long CCodaFrameStreamer_00c49650::GetFreeFrameCount()
{
    return m_doneQueue.GetCount();
}