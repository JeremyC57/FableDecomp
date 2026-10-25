// CAudioObject::PlaySource(CSoundBuffer* buffer, unsigned __int64 startTime) -- queue a sound buffer on the audio object: when the object can take it, create its hardware buffer (a failed HRESULT goes to the audio system's error latch) and push it with its start time onto the +0x54 buffer queue
// Leaf: ?PlaySource@CAudioObject@@QAE_NPAUCSoundBuffer@@_K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C43F80, 71 bytes (+ int3 padding), __thiscall `ret 0Ch`, returns bool.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 439 "0xC43F80 = CAudioObject::PlaySource"): `this` is the libpk audio object --
//   it calls CAudioObject::CanUseBuffer 0x00C43D30 on itself (agent439's CanUseBuffer row names this call site, 0x00C43F89, its
//   queueing caller) and pushes onto the CircularQueue member at +0x54 (DequeueNext 0x00C44320). Its only caller is the API
//   forwarder 0x00C35A10, catalogued _global::AudioObject_PlaySource. Descriptive names (no PDB covers libpk).
// Label correction: the landed row was a `__fastcall` with a dummy edx argument, _global::Weapon_ProcessAnimFrameIfReady (a
//   propagated forge auto-name), over invented CWeapon/CObj/CSub/CLogger types; the same callees and global, typed here.
// Callees: CanUseBuffer 0x00C43D30, CSoundBuffer::CreateHardwareBuffer 0x00C3C750 (returns an HRESULT), CAudioSystem::SetError
//   0x00C3D090 on g_audioSystem 0x0143BB58, CircularQueue::Enqueue 0x00C44460 (stores the buffer at m_items[m_head] and the 64-bit
//   start time at m_times[m_head], `ret 0Ch`).
struct CSoundBuffer {
    long CreateHardwareBuffer();                // 0x00C3C750
};

class CAudioSystem {
public:
    void SetError(long code);                   // 0x00C3D090
};
extern CAudioSystem g_audioSystem;              // 0x0143BB58

class CircularQueue {                           // the audio object's buffer queue (+0x54)
public:
    bool Enqueue(CSoundBuffer* buffer, unsigned __int64 startTime);   // 0x00C44460
    CSoundBuffer**    m_items;                  // +0x00
    unsigned __int64* m_times;                  // +0x04
    unsigned long     m_capacity;               // +0x08
    unsigned long     m_head;                   // +0x0c
    unsigned long     m_tail;                   // +0x10
    CSoundBuffer*     m_lastItem;               // +0x14
    unsigned long     m_stats[3];               // +0x18
};

class CVoice;

class CAudioObject {
public:
    void*         m_vptr;                       // +0x00 (vtable 0x012B5A88)
    unsigned long m_node[2];                    // +0x04
    unsigned long m_caps;                       // +0x0c
    unsigned char m_pad10[0x50 - 0x10];
    CVoice*       m_pVoice;                     // +0x50
    CircularQueue m_queue;                      // +0x54
    CSoundBuffer* m_pCurrentBuffer;             // +0x78
    bool CanUseBuffer(CSoundBuffer* buffer);    // 0x00C43D30
    bool PlaySource(CSoundBuffer* buffer, unsigned __int64 startTime);
};

bool CAudioObject::PlaySource(CSoundBuffer* buffer, unsigned __int64 startTime)
{
    if (CanUseBuffer(buffer)) {
        long hr = buffer->CreateHardwareBuffer();
        if (hr < 0) {
            g_audioSystem.SetError(hr);
            return false;
        }
        return m_queue.Enqueue(buffer, startTime);
    }
    return false;
}
