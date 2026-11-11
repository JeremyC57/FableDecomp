// CAudioObject::AttachConfiguredBackend(CSoundBuffer* buffer, unsigned long flags, const SVoiceCaps* hardware, const SVoiceCaps* software, CVoice** ppVoice) -- libpk audio: get a voice for the object's buffer from the hardware capability record when the object and buffer formats are supported there (3D, streamed or 2D hardware voice), else from the software record; a voice that cannot rebind to the object is released; returns 0 with the voice, the voice manager's error, -14 (streamed buffer the hardware cannot take) or -9 / -11 (record with / without an id has no voices)
// Leaf: ?AttachConfiguredBackend@CAudioObject@@QAEJPAUCSoundBuffer@@KPBUSVoiceCaps@@1PAPAVCVoice@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C43B30, 503 bytes (+ int3 padding), thiscall, `ret 14h`, long status.
// Identity (descriptive; libpk has no PDB/RTTI): the two callers are the derived objects' slot-8 voice creators (landed
//   0x00C418C0 / 0x00C448A0, which name this helper AttachConfiguredBackend and pass the global audio flags 0x0139C9FC and
//   the 2D or 3D hardware / software records of g_voiceCaps 0x0139C988: {0x0139C9A0, 0x0139C9D0} and {0x0139C988, 0x0139C9B8}).
//   The catalog "CShaderConstantRange_ValidateAndLoad" is propagated.
// Callees: CSoundBuffer::GetDesc 0x00C3C4A0; the object's Is3D (slot 1); on g_pVoiceManager 0x0143BB5C: ApplySlotB 0x00C4D4B0
//   (3D hardware), slot 2 (0x00C3E330 -> ApplySlotA 0x00C4D440, the streamed-format arm), ApplySlotA 0x00C4D440 (2D hardware),
//   AcquireVoiceB 0x00C4D5F0 (3D software), AcquireVoiceA 0x00C4D520 (2D software), Release 0x00C4D070; the voice's Rebind
//   (slot 3). Callee names are the landed descriptive spellings.
// Layout: CAudioObject m_caps +0x0C, m_queue.m_capacity +0x5C, m_pCurrentBuffer +0x78 (the landed CAudioObject rows);
//   CSoundBuffer m_state +0x2C (bit 2 streamed, bit 3 descriptive "seekable"); SVoiceCaps id / available / object caps /
//   format caps.
struct SBufferDesc {
    unsigned long m_mode;                   // +0x00
    unsigned long m_channels;               // +0x04
};

struct CSoundBuffer {
    virtual void v00();
    unsigned char m_pad04[0x2c - 4];
    unsigned long m_state;                  // +0x2c (bit 2: streamed)
    bool IsStreamed() const { return (m_state >> 2) & 1; }
    bool IsSeekable() const { return (m_state >> 3) & 1; }
    const SBufferDesc* GetDesc() const;     // 0x00C3C4A0
};

struct SVoiceCaps {
    unsigned long m_id;                     // +0x00
    unsigned long m_available;              // +0x04
    unsigned long m_objectCaps;             // +0x08
    unsigned long m_formatCaps;             // +0x0c
    unsigned long m_words[2];               // +0x10
};

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
    virtual void v00();
    virtual void v01();
    virtual long AcquireStreamVoice(CAudioObject* object, CVoice** ppVoice);   // slot 2 (0x00C3E330)
    long ApplySlotA(CAudioObject* object, CVoice** ppVoice);      // 0x00C4D440 (2D hardware)
    long ApplySlotB(CAudioObject* object, CVoice** ppVoice);      // 0x00C4D4B0 (3D hardware)
    long AcquireVoiceA(CAudioObject* object, CVoice** ppVoice);   // 0x00C4D520 (2D software)
    long AcquireVoiceB(CAudioObject* object, CVoice** ppVoice);   // 0x00C4D5F0 (3D software)
    void Release(CVoice* voice);                                  // 0x00C4D070
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
    virtual ~CAudioObject();                // slot 0 (0x00C3CFC0)
    virtual bool Is3D() const;              // slot 1
    unsigned long m_node[2];                // +0x04
    unsigned long m_caps;                   // +0x0c
    unsigned char m_pad10[0x50 - 0x10];
    CVoice*       m_pVoice;                 // +0x50
    CircularQueue m_queue;                  // +0x54
    CSoundBuffer* m_pCurrentBuffer;         // +0x78
    long AttachConfiguredBackend(CSoundBuffer* buffer, unsigned long flags, const SVoiceCaps* hardware,
                                 const SVoiceCaps* software, CVoice** ppVoice);
};

long CAudioObject::AttachConfiguredBackend(CSoundBuffer* buffer, unsigned long flags, const SVoiceCaps* hardware,
                                           const SVoiceCaps* software, CVoice** ppVoice)
{
    *ppVoice = 0;
    unsigned long unsupported = m_caps & ~hardware->m_objectCaps;
    bool streamed = buffer && buffer->IsStreamed();
    if (streamed && unsupported)
        return -14;
    bool useSoftware = unsupported != 0
        || (buffer && (((flags & 8) && !buffer->IsStreamed() && !(flags & 0x10))
                       || (buffer->GetDesc()->m_mode & ~hardware->m_formatCaps)));
    CVoiceManager* manager = g_pVoiceManager;
    CVoice* voice;
    if (!useSoftware) {
        if (hardware->m_available) {
            long result;
            if (Is3D())
                result = manager->ApplySlotB(this, &voice);
            else if (buffer->IsStreamed() && buffer->IsSeekable()
                     && (buffer->GetDesc()->m_mode == 1 || buffer->GetDesc()->m_mode == 0x40))
                result = manager->AcquireStreamVoice(this, &voice);
            else
                result = manager->ApplySlotA(this, &voice);
            if (result)
                return result;
            if (m_pCurrentBuffer || m_queue.m_capacity) {
                result = voice->Rebind(this);
                if (result) {
                    manager->Release(voice);
                    return result;
                }
            }
            *ppVoice = voice;
            return 0;
        }
        if (streamed)
            return hardware->m_id ? -9 : -11;
    }
    if (software->m_available) {
        long result;
        if (Is3D())
            result = manager->AcquireVoiceB(this, &voice);
        else
            result = manager->AcquireVoiceA(this, &voice);
        if (result)
            return result;
        if (m_pCurrentBuffer || m_queue.m_capacity) {
            result = voice->Rebind(this);
            if (result) {
                manager->Release(voice);
                return result;
            }
        }
        *ppVoice = voice;
        return 0;
    }
    return software->m_id ? -9 : -11;
}