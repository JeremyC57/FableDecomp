// CAudioObject::ReportQueueInfo() -- when a queue callback is set, read the +0x54 CircularQueue's state record and hand it to the callback with its context
// Leaf: ?ReportQueueInfo@CAudioObject@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C432F0, 64 bytes, thiscall, `ret`.
// Identity: libpk's CAudioObject (the landed sibling model of FlushQueue 0x00C437C0 / ReleaseQueue 0x00C43810: vtable 0x012B5A88,
//   +0x54 CircularQueue, `__stdcall` queue callback +0xD4 with context +0xD8). The body is those siblings' callback block on its own
//   (no Reset / Clear / Release): it reports the queue's current state. Name: descriptive (no PDB, RTTI or GameCODA 2.10 record covers
//   libpk's CAudioObject: CODAApiDbg / CAGEDbg / ToolkitDbg and the CAGE / Toolkit sources have no CAudioObject, CircularQueue or
//   queue-info name); ReportQueueInfo is the spelling every landed caller binds (0x00C43A10 CAudioObject::Update after FlushQueue,
//   0x00C5B300 CSoftwareVoice::AdvanceBuffer, 0x00C73050 CAudioSource::UpdateStream, each after dequeuing a buffer).
// Callees: CircularQueue::GetInfo 0x00C442F0 (returns the record it filled; the FlushQueue sibling's spelling), the callback at +0xD4.
// Shape: a 0x14-byte frame for the record, `this` in esi; the callback is `push context ; push &info ; push this ; call [esi+0D4h]`.
// Name fix (NF217, 2026-11-21, q12379): was the placeholder CMarkedBufferOwner_00c43810::ReportMark (?ReportMark@CMarkedBufferOwner_00c43810
//   @@QAEXXZ) over the local CMarkedBuffer_00c43810 / SBufferMark_00c43810 stand-ins; now the landed CAudioObject / CircularQueue /
//   SQueueInfo model (no shared header exists for any of them), and the GetInfo call binds ?GetInfo@CircularQueue@@QAEAAUSQueueInfo@@AAU2@@Z
//   (0x00C437C0's spelling; was ?GetMark@CMarkedBuffer_00c43810@@QAEXAAUSBufferMark_00c43810@@@Z).
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
    void ReportQueueInfo();
};

void CAudioObject::ReportQueueInfo()
{
    if (m_pfnQueueCallback) {
        SQueueInfo info;
        m_queue.GetInfo(info);
        m_pfnQueueCallback(this, &info, m_pQueueContext);
    }
}
