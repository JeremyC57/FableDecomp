// CircularQueue::Enqueue(CSoundBuffer* buffer, unsigned __int64 startTime) -- push a sound buffer at the head slot: the first buffer fixes the queue's format, later ones must match it (else error -14); a full head slot is error -15; the buffer is add-ref'd, stored with its 64-bit start time and the head wraps at the capacity
// Leaf: ?Enqueue@CircularQueue@@QAE_NPAUCSoundBuffer@@_K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C44460, 228 bytes, thiscall `ret 0Ch`, returns bool.
// Identity (descriptive, libpk has no PDB): the landed caller CAudioObject::PlaySource 0x00C43F80 declares this callee as
//   CircularQueue::Enqueue (bool, CSoundBuffer*, unsigned __int64) on its +0x54 queue; the landed DequeueNext 0x00C44320 /
//   DequeueItem 0x00C44270 share the layout. The catalog label "CCreatureAction_FireMissileWeapon::ProcessAnimFrame" is wrong.
// Callees: CSoundBuffer::GetDesc 0x00C3C4A0 (landed spelling, as declared by CAudioObject::CanAcquireVoice 0x00C43390; returns buffer + 0x10; 28-byte block copied whole), the buffer add-ref
//   0x00C3C5D0 (bool; ++refcount with an overflow error), CAudioSystem::SetError 0x00C3D090 on g_audioSystem 0x0143BB58.
// Layout: +0x00 item slots, +0x04 64-bit stamps, +0x08 capacity, +0x0C head, +0x10 tail, +0x14 last item, +0x18 the queue's
//   12-byte format (words 0 and 1 compared).
// Shape: the format test is an inline `matches` flag (retail's hoisted `mov al,1`, `xor al,al` on the first mismatch).
struct CWaveFormat {                            // the first 12 bytes of a buffer's format block
    unsigned long Word0;                        // +0x00
    unsigned long Word1;                        // +0x04
    unsigned long Word2;                        // +0x08
    bool Matches(const CWaveFormat& other) const
    {
        bool result = true;
        if (Word0 != other.Word0)
            result = false;
        if (Word1 != other.Word1)
            result = false;
        return result;
    }
};
struct SBufferDesc {                            // 28 bytes at CSoundBuffer +0x10
    CWaveFormat   Wave;                         // +0x00
    unsigned long Rest[4];                      // +0x0c
};
struct CSoundBuffer {
    const SBufferDesc* GetDesc() const;        // 0x00C3C4A0 (returns this + 0x10)
    bool AddRef();                              // 0x00C3C5D0
};
class CAudioSystem {
public:
    void SetError(long code);                   // 0x00C3D090
};
extern CAudioSystem g_audioSystem;              // 0x0143BB58

class CircularQueue {
public:
    CSoundBuffer**    m_items;                  // +0x00
    unsigned __int64* m_times;                  // +0x04
    unsigned long     m_capacity;               // +0x08
    unsigned long     m_head;                   // +0x0c
    unsigned long     m_tail;                   // +0x10
    CSoundBuffer*     m_lastItem;               // +0x14
    CWaveFormat       m_format;                 // +0x18
    bool Enqueue(CSoundBuffer* buffer, unsigned __int64 startTime);
};

bool CircularQueue::Enqueue(CSoundBuffer* buffer, unsigned __int64 startTime)
{
    SBufferDesc format = *buffer->GetDesc();
    bool matches = true;
    if (m_capacity != 0 && m_format.Word0 == 0)
        m_format = format.Wave;
    else {
        if (m_format.Word0 != format.Wave.Word0)
            matches = false;
        if (m_format.Word1 != format.Wave.Word1)
            matches = false;
        if (!matches) {
            g_audioSystem.SetError(-14);
            return false;
        }
    }
    if (m_items[m_head] != 0) {
        g_audioSystem.SetError(-15);
        return false;
    }
    if (!buffer->AddRef())
        return false;
    m_items[m_head] = buffer;
    m_times[m_head] = startTime;
    if (m_capacity == ++m_head)
        m_head = 0;
    return true;
}