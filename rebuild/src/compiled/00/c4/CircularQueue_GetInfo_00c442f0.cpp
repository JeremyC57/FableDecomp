// CircularQueue::GetInfo(SQueueInfo& info) -- copy the audio queue's state (capacity, items, times, tail, last item) into the caller's 20-byte record and return it
// Leaf: ?GetInfo@CircularQueue@@QAEAAUSQueueInfo@@AAU2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C442F0, length 35 bytes, thiscall `ret 4`; callers 0x00C35AAC, 0x00C43308
//   (CAudioObject::ReportQueueInfo 0x00C432F0), 0x00C437E7 (CAudioObject::FlushQueue 0x00C437C0), 0x00C43837
//   (CAudioObject::ReleaseQueue 0x00C43810).
// Identity (NF218, 2026-11-21, q12385; descriptive: no PDB covers libpk): the +0x54 CircularQueue of libpk's CAudioObject
//   (agent439 layout: items +0, 64-bit times +4, capacity +8, head +0xc, tail +0x10, last item +0x14, stats +0x18). The body
//   copies {capacity, items, times, tail, last item} into the record the landed callers declare as SQueueInfo and pass to the
//   queue callback; eax still holds the record at `ret 4`, so the reference is returned (the spelling the landed callers bind).
//   Was CMarkedBuffer_00c44550::GetMark over a placeholder SBufferMark model (the chunk label
//   "CTCPhysicsRigidBody::GetFacingVector" is a propagated BSim name). Model: the landed sibling 0x00C437C0's, token for token.
// Callees: none.
// Shape: five word copies through edx with the record pointer in eax, in source statement order (+8, +0x14, +0x10, +0, +4);
//   a by-value return of a named local makes VC7.1 load all five words into callee-saved registers first (46 v 35).
struct SQueueInfo {                                 // 20 bytes, filled by CircularQueue::GetInfo 0x00C442F0
    unsigned long m_capacity;                       // +0x00 (queue +0x08)
    void*         m_items;                          // +0x04 (queue +0x00)
    void*         m_times;                          // +0x08 (queue +0x04)
    unsigned long m_tail;                           // +0x0c (queue +0x10)
    void*         m_lastItem;                       // +0x10 (queue +0x14)
};

class CSoundBuffer;

class CircularQueue {                               // the audio object's buffer queue (+0x54)
public:
    SQueueInfo& GetInfo(SQueueInfo& info);
    CSoundBuffer**    m_items;                      // +0x00
    unsigned __int64* m_times;                      // +0x04
    unsigned long     m_capacity;                   // +0x08
    unsigned long     m_head;                       // +0x0c
    unsigned long     m_tail;                       // +0x10
    CSoundBuffer*     m_lastItem;                   // +0x14
    unsigned long     m_stats[3];                   // +0x18
};

SQueueInfo& CircularQueue::GetInfo(SQueueInfo& info)
{
    info.m_capacity = m_capacity;
    info.m_lastItem = m_lastItem;
    info.m_tail = m_tail;
    info.m_items = m_items;
    info.m_times = m_times;
    return info;
}
