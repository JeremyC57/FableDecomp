// Behaviour test for CCodaFrameStreamer_00c49650::GetFreeFrameCount @ 0x00C49610: returns the done-frame mailbox's count, asked on the +0x1c member.
#include <cstdio>
#include <cstddef>
struct CMailboxQueue {
    unsigned char m_body[0x20];
    long GetCount() { return (long)m_body[0]; }
};
struct CStreamerBase_00c49650 {
    virtual void Update() { }
    virtual long GetFreeFrameCount() { return -1; }
    virtual ~CStreamerBase_00c49650() { }
};
struct CCodaFrameStreamer_00c49650 : CStreamerBase_00c49650 {
    void*         m_pSource;
    unsigned char m_frameQueue[0x14];
    CMailboxQueue m_doneQueue;
    unsigned long m_frames;
    virtual long GetFreeFrameCount();
};
long CCodaFrameStreamer_00c49650::GetFreeFrameCount()
{
    return m_doneQueue.GetCount();
}
int main()
{
    CCodaFrameStreamer_00c49650 s;
    s.m_doneQueue.m_body[0] = 5;
    CStreamerBase_00c49650* p = &s;
    bool ok = p->GetFreeFrameCount() == 5;
    s.m_doneQueue.m_body[0] = 0;
    ok = ok && p->GetFreeFrameCount() == 0;
    if (!ok) { std::printf("00c49610_TEST FAIL\n"); return 1; }
    std::printf("00c49610_TEST PASS\n");
    return 0;
}