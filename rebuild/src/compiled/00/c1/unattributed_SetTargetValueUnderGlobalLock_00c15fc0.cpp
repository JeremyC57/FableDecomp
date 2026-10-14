// store a long through the pointer at +0x08 while holding the audio library's global lock (an 8-byte scoped lock object built from the global @ 0x0143BB14)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C15FC0, length 48 bytes.
// Callees: 0x00C0FFC0 CQueueLock::CQueueLock(void*), 0x00C0FFE0 CQueueLock::~CQueueLock
// The manifest label (C3DMeshFilePrimitiveChunk::SetMaterialIndex) is propagated; the lock class is the one the
// landed audio bodies use (0x00C39830 CMailboxQueue::DispatchMessage names it CQueueLock), here over a global
// lock target instead of a queue. The target pointer at +0x08 is the only member touched.
//

struct CLockTarget;
extern CLockTarget g_audioLock;              // 0x0143BB14
struct CQueueLock {                          // 8-byte scoped lock: target + thread id
    CLockTarget* m_pTarget;
    long m_threadId;
    CQueueLock(CLockTarget* target);         // 0x00C0FFC0
    ~CQueueLock();                           // 0x00C0FFE0
};
struct CLockedValueOwner_00c15fc0 {
    unsigned char _pad[0x08];
    long* m_pTarget;                         // +0x08
    void SetTargetValueUnderGlobalLock(long value);
};
void CLockedValueOwner_00c15fc0::SetTargetValueUnderGlobalLock(long value)
{
    CQueueLock lock(&g_audioLock);
    *m_pTarget = value;
}