// CAudioObject::SetBuffer(CSoundBuffer* buffer) -- make a sound buffer the audio object's single current buffer: drop any queued buffers, reset the object, refuse a buffer it cannot use, then (when it changes) create the buffer's device copy, take a reference on it, release the old buffer and store the new one (+0x78); a voice that cannot rebind to the object is released
// Leaf: ?SetBuffer@CAudioObject@@QAE_NPAUCSoundBuffer@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C43E20, 153 bytes (+ int3 padding), __fastcall `ret 4`, returns bool.
// Library: libpk audio (no PDB/RTTI): descriptive names. Callers: the `__stdcall` API wrapper 0x00C35990 (call) and
//   0x00C35D72 (tail jump). The manifest's "CExclusiveAssert_UpdateSelectedWeapon" is propagated.
// Callees: ReleaseQueue 0x00C43810 (landed as `Shutdown`), Reset 0x00C436C0, CanUseBuffer 0x00C43D30, the buffer's
//   CreateHardwareBuffer 0x00C3C750 / AddRef 0x00C3C5D0 / Release 0x00C3C5F0, SetError 0x00C3D090, the voice's slot 3
//   (owner; non-zero = cannot rebind) and the voice manager's Release 0x00C4D070 (g_pVoiceManager 0x0143BB5C).
// Shape: the guard of ReleaseQueue is the queue's capacity (+0x5C); the unchanged-buffer test and the changed block
//   compile the same as `if (m_pCurrentBuffer != buffer) { ... }` or an early `return true`.
struct CSoundBuffer {
    long CreateHardwareBuffer();            // 0x00C3C750
    bool AddRef();                          // 0x00C3C5D0
    void Release();                         // 0x00C3C5F0
};

class CAudioSystem {
public:
    void SetError(long code);               // 0x00C3D090
};
extern CAudioSystem g_audioSystem;          // 0x0143BB58

class CAudioObject;

class CVoice {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual long Rebind(CAudioObject* owner);   // slot 3
};

class CVoiceManager {
public:
    void Release(CVoice* voice);            // 0x00C4D070
};
extern CVoiceManager* g_pVoiceManager;      // 0x0143BB5C

class CircularQueue {
public:
    CSoundBuffer**    m_items;              // +0x00
    unsigned __int64* m_times;              // +0x04
    unsigned long     m_capacity;           // +0x08
    unsigned long     m_head;               // +0x0c
    unsigned long     m_tail;               // +0x10
    CSoundBuffer*     m_lastItem;           // +0x14
    unsigned long     m_stats[3];           // +0x18
};

class CAudioObject {
public:
    void*         m_vptr;                   // +0x00 (vtable 0x012B5A88)
    unsigned long m_node[2];                // +0x04
    unsigned long m_caps;                   // +0x0c
    unsigned char m_pad10[0x50 - 0x10];
    CVoice*       m_pVoice;                 // +0x50
    CircularQueue m_queue;                  // +0x54
    CSoundBuffer* m_pCurrentBuffer;         // +0x78
    void ReleaseQueue();                    // 0x00C43810
    void Reset();                           // 0x00C436C0
    bool CanUseBuffer(CSoundBuffer* buffer);    // 0x00C43D30
    bool SetBuffer(CSoundBuffer* buffer);
};

bool CAudioObject::SetBuffer(CSoundBuffer* buffer)
{
    if (m_queue.m_capacity)
        ReleaseQueue();
    Reset();
    if (buffer && !CanUseBuffer(buffer))
        return false;
    if (m_pCurrentBuffer != buffer) {
        if (buffer) {
            long result = buffer->CreateHardwareBuffer();
            if (result < 0) {
                g_audioSystem.SetError(result);
                return false;
            }
            if (!buffer->AddRef())
                return false;
        }
        if (m_pCurrentBuffer)
            m_pCurrentBuffer->Release();
        m_pCurrentBuffer = buffer;
        if (m_pVoice && m_pVoice->Rebind(this)) {
            g_pVoiceManager->Release(m_pVoice);
            m_pVoice = 0;
        }
    }
    return true;
}