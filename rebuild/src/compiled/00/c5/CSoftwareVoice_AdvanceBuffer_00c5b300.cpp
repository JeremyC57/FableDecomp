// CSoftwareVoice::AdvanceBuffer(CAudioSound* sound) -- once the source ring (+0xB4) has drained, move the voice on to the sound's next buffer: a single-buffer sound (+0x78) restarts when looping or stops the voice; a queued sound re-feeds a looping streamed slot, else dequeues the tail slot, reports the queue, and hands the voice the new slot at its due time, or clears the ring and the buffer when nothing is left
// Leaf: ?AdvanceBuffer@CSoftwareVoice@@QAEXPAUCAudioSound@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C5B300, 307 bytes, thiscall `ret 4`.
// Identity (descriptive method name; GameCODA / libpk have no PDB or RTTI): called twice (0x00C5BA05, 0x00C5BC32) by slot 9
//   of the CSoftwareVoice vtable 0x012B5C60 (0x00C5B7C0) with that slot's sound argument. The voice layout is the landed
//   CSoftwareVoice::StartSound 0x00C5B250 / SetBuffer 0x00C5A2E0 model (+0x0C buffer, +0x10 64-bit clock, ring +0xB4 with
//   data +0xB8 and queued frames +0xD0); the sound is the landed CAudioSound (flags +0x10 bit 8 loop, CircularQueue
//   +0x54, single buffer +0x78) of CCodaVoice::Play 0x00C59720 / CAudioSource::BeginSound 0x00C59BC0.
// Callees: CAudioRingBuffer::Clear 0x00C5CE40 (zeroes +4..+0x1C; landed under a wrong label), CSoundBuffer::GetDesc
//   0x00C3C4A0, CircularQueue::DequeueNext 0x00C44320, CAudioObject::ReportQueueInfo 0x00C432F0, CircularQueue::
//   GetCurrentEntry 0x00C35D10 (the second read only; the first is the inline capacity-checked read as in BeginSound),
//   virtual slot 18 (+0x48) SetBuffer.
// Shape: the loop bit is the byte `& 1` argument of CCodaVoice::Play (`shr ; and 0FFFFFF01h`); the ring test is an inline
//   accessor (`lea ecx,[esi+0B4h]` before its second data test); the queue's IsEmpty is the CAudioObject::Update inline.
//   CAudioSound (the voice rows' parameter spelling) and CAudioObject (the object's own rows, 0x00C43xxx) are one libpk
//   object; ReportQueueInfo keeps its bound CAudioObject spelling through a cast, as StartSound casts to CAudioSource.
class CSoundBuffer;
struct SBufferDesc {
    unsigned long  m_state;                             // +0x00
    unsigned long  m_numChannels;                       // +0x04
    unsigned long  m_sampleRate;                        // +0x08
    unsigned long  m_dataBytes;                         // +0x0c
    unsigned long  m_playOffset;                        // +0x10
    unsigned long  m_prefetchBytes;                     // +0x14
    unsigned char* m_pSource;                           // +0x18
};
class CSoundBuffer {
public:
    const SBufferDesc* GetDesc() const;                 // 0x00C3C4A0
};

class CircularQueue {                                   // the sound's buffer queue (+0x54)
public:
    CSoundBuffer**    m_items;                          // +0x00
    unsigned __int64* m_times;                          // +0x04
    unsigned long     m_capacity;                       // +0x08
    unsigned long     m_head;                           // +0x0c
    unsigned long     m_tail;                           // +0x10
    CSoundBuffer*     m_lastItem;                       // +0x14
    unsigned long     m_stats[3];                       // +0x18
    bool IsEmpty() const { return m_capacity != 0 && m_stats[0] == 0; }
    unsigned long DequeueNext();                        // 0x00C44320
    CSoundBuffer* GetCurrentEntry() const;              // 0x00C35D10
};

class CAudioObject {                                    // the same libpk object under its landed member spelling
public:
    void ReportQueueInfo();                             // 0x00C432F0
};

struct CAudioSound {
    unsigned char  _pad00[0x10];
    unsigned long  m_flags;                             // +0x10 (bit 8: loop)
    unsigned char  _pad14[0x54 - 0x14];
    CircularQueue  m_queue;                             // +0x54
    CSoundBuffer*  m_pBuffer;                           // +0x78
    bool IsLooping() const { return (m_flags >> 8) & 1; }
};

class CAudioRingBuffer {                                // vtable 0x012B8FC4
public:
    virtual ~CAudioRingBuffer();
    unsigned char* m_pData;                             // +0x04
    unsigned long  m_capacity;                          // +0x08
    unsigned long  m_frameBytes;                        // +0x0c
    unsigned long  m_channels;                          // +0x10
    unsigned long  m_readPos;                           // +0x14
    unsigned long  m_writePos;                          // +0x18
    unsigned long  m_queued;                            // +0x1c
    bool IsDrained() const { return m_pData != 0 && m_queued == 0; }
    void Clear();                                       // 0x00C5CE40
};

#pragma pack(push, 4)                                   // the 64-bit clock sits at +0x10 (4-byte packing)
class CSoftwareVoice {                                  // vtable 0x012B5C60
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17();
    virtual void SetBuffer(CSoundBuffer* buffer, unsigned char loop);   // slot 18 (+0x48): 0x00C5A2E0
    unsigned char    _pad04[8];
    CSoundBuffer*    m_pBuffer;                         // +0x0c
    __int64          m_clock;                           // +0x10
    unsigned char    _pad18[0xb4 - 0x18];
    CAudioRingBuffer m_ring;                            // +0xb4
    void AdvanceBuffer(CAudioSound* sound);
};
#pragma pack(pop)

void CSoftwareVoice::AdvanceBuffer(CAudioSound* sound)
{
    if (m_ring.m_pData == 0)
        return;
    if (!m_ring.IsDrained())
        return;
    if (sound->m_pBuffer != 0) {
        if (sound->IsLooping()) {
            SetBuffer(sound->m_pBuffer, 1);
            return;
        }
        m_ring.Clear();
        m_pBuffer = 0;
        return;
    }
    CircularQueue& queue = sound->m_queue;
    CSoundBuffer* entry = queue.m_capacity ? queue.m_items[queue.m_tail] : 0;
    if (sound->IsLooping() && entry->GetDesc()->m_prefetchBytes != 0) {
        SetBuffer(entry, (sound->m_flags >> 8) & 1);
        return;
    }
    queue.DequeueNext();
    ((CAudioObject*)sound)->ReportQueueInfo();
    if (!queue.IsEmpty() && queue.m_items[queue.m_tail] != 0 && (queue.m_tail != 0 || sound->IsLooping())) {
        m_clock = queue.m_times[queue.m_tail];
        SetBuffer(queue.GetCurrentEntry(), (sound->m_flags >> 8) & 1);
        return;
    }
    m_ring.Clear();
    m_pBuffer = 0;
}