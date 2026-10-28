// CAudioObject::FlushQueue() -- reset the audio object, release every buffer in its +0x54 CircularQueue and, when a queue callback is set, report the emptied queue's state to it (the queue's arrays are kept)
// Leaf: ?FlushQueue@CAudioObject@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C437C0, 69 bytes, thiscall, `ret`.
// Identity: libpk's CAudioObject (sweep1 class model: vtable 0x012B5A88, +0x54 CircularQueue, `__stdcall` queue callback
//   +0xD4 with context +0xD8). This body is the landed CAudioObject::ReleaseQueue 0x00C43810 without its closing
//   CircularQueue::Release (the arrays stay allocated), so the name is descriptive (no PDB covers libpk). Callers
//   0x00C35D3B and 0x00C43AFC. Label correction: the manifest row "CDisplayManager::DestroyDevice" is propagated.
// Callees: CAudioObject::Reset 0x00C436C0, CircularQueue::Clear 0x00C443D0, CircularQueue::GetInfo 0x00C442F0 (returns the
//   record it filled, landed definition), the callback at +0xD4.
// Shape: the landed ReleaseQueue model: a 0x14-byte frame for the record, the queue address kept in edi; the callback is
//   `push context ; push &info ; push this ; call [esi+0D4h]`.
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
    SQueueInfo& GetInfo(SQueueInfo& info);          // 0x00C442F0
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
    void FlushQueue();
};

void CAudioObject::FlushQueue()
{
    Reset();
    m_queue.Clear();
    if (m_pfnQueueCallback) {
        SQueueInfo info;
        m_queue.GetInfo(info);
        m_pfnQueueCallback(this, &info, m_pQueueContext);
    }
}