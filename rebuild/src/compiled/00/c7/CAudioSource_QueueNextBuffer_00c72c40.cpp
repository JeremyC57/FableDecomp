// CAudioSource::QueueNextBuffer(CircularQueue* queue, __int64 now, bool force) -- feed the next due stream buffer to the OpenAL source: when the source still has room and the queue slot is valid (slot 0 only when forced) and its time has come, let the source prepare the buffer, queue it with alSourceQueueBuffers and advance the slot
// Leaf: ?QueueNextBuffer@CAudioSource
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C72C40, length 186 bytes, ret 10h. Callees: CircularQueue
//   GetItemAtIndex 0x00C44430 / GetItem 0x00C442C0 / GetTime 0x00C442D0, the source's virtual slot 19 (+0x4C,
//   PrepareBuffer(item, &bufferId)), and on the global OpenAL context 0x0143BDB0: GetError 0x00C4D960 (clears the
//   error state), SourceQueueBuffers 0x00C57690, CheckError 0x00C4DBE0. Callers 0x00C73036, 0x00C73213.
// Layout (descriptive; libpk OpenAL source, no PDB/RTTI): the landed CAudioSource model (SourceId +0x58) plus the
//   stream cursor: next queue slot +0x5C, buffers queued +0x60. CircularQueue as landed 0x00C44430 (items +0,
//   per-slot __int64 times +4, capacity +8).
// Shape: one positive `&&`/`||` guard around a block that ends in its own `return 0`, and a positive `time <= now`
//   test inside (jg first on the high word): both `return 0` epilogues sit at the end, ebx is pushed only in the
//   block; the buffer id local lives in the dead `force` argument slot; `m_next > 0` is
//   the unsigned `ja`; the negative results of PrepareBuffer and CheckError are returned as they are, every other exit returns 0.
struct CItem;

class CircularQueue {
public:
    CItem**           m_items;             // +0x00
    __int64*          m_times;             // +0x04
    unsigned long     m_capacity;          // +0x08
    bool GetItemAtIndex(unsigned long index);   // 0x00C44430
    CItem* GetItem(unsigned long index);        // 0x00C442C0
    __int64 GetTime(unsigned long index);       // 0x00C442D0
};

struct CAudioContext {
    long GetError();                                                                // 0x00C4D960
    void SourceQueueBuffers(unsigned long source, long count, const unsigned long* buffers);   // 0x00C57690
    long CheckError();                                                              // 0x00C4DBE0
};
extern CAudioContext g_AudioContext;       // 0x0143BDB0

struct CAudioSource {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual long PrepareBuffer(CItem* item, unsigned long* bufferId);    // slot 19 (+0x4C)
    unsigned char  _pad04[0x54];
    unsigned long  SourceId;               // +0x58
    unsigned long  m_next;                 // +0x5C
    unsigned long  m_queued;               // +0x60
    long QueueNextBuffer(CircularQueue* queue, __int64 now, bool force);
};

long CAudioSource::QueueNextBuffer(CircularQueue* queue, __int64 now, bool force)
{
    if (m_queued != queue->m_capacity && queue->GetItemAtIndex(m_next) && (m_next > 0 || force)) {
        CItem* item = queue->GetItem(m_next);
        if (queue->GetTime(m_next) <= now) {
            unsigned long bufferId;
            long result = PrepareBuffer(item, &bufferId);
            if (result < 0)
                return result;
            g_AudioContext.GetError();
            g_AudioContext.SourceQueueBuffers(SourceId, 1, &bufferId);
            result = g_AudioContext.CheckError();
            if (result < 0)
                return result;
            ++m_queued;
            m_next = (m_next + 1) % queue->m_capacity;
        }
        return 0;
    }
    return 0;
}