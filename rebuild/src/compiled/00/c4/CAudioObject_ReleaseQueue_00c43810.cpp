// CAudioObject::ReleaseQueue() -- drop the audio object's queued buffers: Reset the object, release every buffer in its +0x54 CircularQueue, report the emptied queue's state through the `__stdcall` queue callback at +0xd4 (with the context at +0xd8) when one is set, then free the queue's arrays
// Leaf: ?ReleaseQueue@CAudioObject@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C43810, length 76 bytes, thiscall.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 439 "0xC43810 Shutdown = ReleaseQueue; the CMarkedBufferOwner models are
//   CAudioObject (+0x50 voice, +0x54 CircularQueue)"): agent439's landed CAudioObject::SetBuffer 0x00C43E20 calls this as
//   `if (m_queue.m_capacity) ReleaseQueue();` (rebuild/oracles/reloc_bindings.tsv already binds ?ReleaseQueue@CAudioObject@@QAEXXZ
//   to 0x00C43810), and its first call is CAudioObject::Reset 0x00C436C0 on `this`. The +0x54 member is the audio object's
//   CircularQueue (agent439: items +0, 64-bit times +4, capacity +8, head +0xc, tail +0x10, last item +0x14, stats +0x18):
//   Clear 0x00C443D0 releases each queued buffer (CSoundBuffer::Release 0x00C3C5F0) and zeroes the slots and counters,
//   GetInfo 0x00C442F0 copies {capacity, items, times, tail, last item} into a 20-byte record, and Release 0x00C44550 clears
//   and frees the arrays when the capacity is non-zero. Descriptive names (no PDB covers libpk).
// Label correction: the landed row was CMarkedBufferOwner_00c43810::Shutdown over a CMarkedBuffer_00c43810 model (the chunk
//   label "CExclusiveAssert::~CExclusiveAssert" is propagated); the same callees, typed as the audio object's.
// Shape: a 0x14-byte frame for the record, the queue address kept in edi; the callback is `push context ; push &info ;
//   push this ; call [esi+0D4h]`.
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
    void Clear();                                   // 0x00C443D0
    void GetInfo(SQueueInfo& info);                 // 0x00C442F0
    void Release();                                 // 0x00C44550
    CSoundBuffer**    m_items;                      // +0x00
    unsigned __int64* m_times;                      // +0x04
    unsigned long     m_capacity;                   // +0x08
    unsigned long     m_head;                       // +0x0c
    unsigned long     m_tail;                       // +0x10
    CSoundBuffer*     m_lastItem;                   // +0x14
    unsigned long     m_stats[3];                   // +0x18
};

class CAudioObject;
typedef void (__stdcall* TQueueCallback)(CAudioObject* object, SQueueInfo* info, void* context);

class CVoice;

class CAudioObject {
public:
    void*          m_vptr;                          // +0x00 (vtable 0x012B5A88)
    unsigned char  m_pad04[0x50 - 4];
    CVoice*        m_pVoice;                        // +0x50
    CircularQueue  m_queue;                         // +0x54
    unsigned char  m_pad78[0xd4 - 0x78];
    TQueueCallback m_pfnQueueCallback;              // +0xd4
    void*          m_pQueueContext;                 // +0xd8
    void Reset();                                   // 0x00C436C0
    void ReleaseQueue();
};

void CAudioObject::ReleaseQueue()
{
    Reset();
    m_queue.Clear();
    if (m_pfnQueueCallback) {
        SQueueInfo info;
        m_queue.GetInfo(info);
        m_pfnQueueCallback(this, &info, m_pQueueContext);
    }
    m_queue.Release();
}
