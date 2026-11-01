// CAudioSource::UpdateStream(CAudioObject* sound, __int64 now) -- slot 9 of the OpenAL voice
// Grade: DIFFER(673v672) -- residue: ebx/ebp swap (retail keeps sound in ebp and queue in ebx, VC7.1 picks the reverse in every spelling) plus the knock-on temp (retail edx, ours edi) in the IsEmpty/items[tail] test; frame, arg-slot homing, bit test and all 5 inline AL resolvers match (22 probes). next: register-weight lever for sound vs &sound->m_queue (try the real CircularQueue/CAudioObject headers or a Peek()/IsEmpty accessor set from the PDB).
// Leaf: ?UpdateStream@CAudioSource@@
extern "C" __declspec(dllimport) void* __stdcall GetProcAddress(void* module, const char* name);   // IAT 0x0143FE44
typedef void (__cdecl* PFN_alSourcePlay)(unsigned long source);
typedef void (__cdecl* PFN_alGetSourcei)(unsigned long source, long param, long* value);
typedef void (__cdecl* PFN_alSourceUnqueueBuffers)(unsigned long source, long count, unsigned long* buffers);

struct CAudioContext {
    void*          __vftable;
    void*          Module;                               // +0x04 (0x0143BDB4)
    unsigned char  _pad08[0x60];
    PFN_alGetSourcei pfnGetSourcei;                      // +0x68 (0x0143BE18) "alGetSourcei" @ 0x012B8E64
    PFN_alSourcePlay pfnSourcePlay;                      // +0x6c (0x0143BE1C) "alSourcePlay" @ 0x012B8E74
    unsigned char  _pad70[0x20];
    PFN_alSourceUnqueueBuffers pfnSourceUnqueueBuffers;  // +0x90 (0x0143BE40) "alSourceUnqueueBuffers" @ 0x012B8EAC
    unsigned char  _pad94[0x70];
    bool           UseALGetProcAddress;                  // +0x104 (0x0143BEB4)
    void* GetProcAddress(const char* name);              // 0x00C4D740
    void SourcePlay(unsigned long source)                // out of line 0x00C575F0
    {
        if (!pfnSourcePlay)
            pfnSourcePlay = (PFN_alSourcePlay)(UseALGetProcAddress ? GetProcAddress("alSourcePlay")
                                                                   : ::GetProcAddress(Module, "alSourcePlay"));
        pfnSourcePlay(source);
    }
    __forceinline void GetSourcei(unsigned long source, long param, long* value)
    {
        if (!pfnGetSourcei)
            pfnGetSourcei = (PFN_alGetSourcei)(UseALGetProcAddress ? GetProcAddress("alGetSourcei")
                                                                   : ::GetProcAddress(Module, "alGetSourcei"));
        pfnGetSourcei(source, param, value);
    }
    void SourceUnqueueBuffers(unsigned long source, long count, unsigned long* buffers)
    {
        if (!pfnSourceUnqueueBuffers)
            pfnSourceUnqueueBuffers = (PFN_alSourceUnqueueBuffers)(UseALGetProcAddress ? GetProcAddress("alSourceUnqueueBuffers")
                                                                                       : ::GetProcAddress(Module, "alSourceUnqueueBuffers"));
        pfnSourceUnqueueBuffers(source, count, buffers);
    }
};
extern CAudioContext g_AudioContext;                     // 0x0143BDB0

class CSoundBuffer;
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
};
class CAudioObject {
public:
    unsigned char  _pad00[0x10];
    unsigned long  m_flags;                             // +0x10 (bits 0-6 type, bit 8 loop)
    unsigned char  _pad14[0x54 - 0x14];
    CircularQueue  m_queue;                             // +0x54
    bool IsLooping() const { return (m_flags >> 8) & 1; }
    void ReportQueueInfo();                             // 0x00C432F0
};

struct SVoiceRequests {                                 // +0x2c: pending-change bits (landed SetPositionB model)
    unsigned long m_bits0to6 : 7;
    unsigned long m_bit7 : 1;                           // 0x80: position B pending
    unsigned long m_bit8 : 1;
    unsigned long m_bit9 : 1;
    unsigned long m_rest : 22;
};
class CAudioSource {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07();
    virtual long Update(CAudioObject* sound, __int64 now);                 // slot 8: 0x00C72FE0
    virtual long UpdateStream(CAudioObject* sound, __int64 now);           // slot 9: 0x00C73050
    virtual long CheckPosition(__int64 position);                          // slot 10: 0x00C59770
    virtual bool SetPositionB(unsigned long low, unsigned long high);     // slot 11: 0x00C727B0
    unsigned char  _pad08[4];
    CSoundBuffer*  m_current;                           // +0x0c
    __int64        m_endPosition;                       // +0x10
    __int64        m_positionB;                         // +0x18
    __int64        m_positionC;                         // +0x20
    unsigned long  m_flags;                             // +0x28
    unsigned long  m_requests;                          // +0x2c (bit 7: position B pending)
    unsigned char  _pad30[0x28];
    unsigned long  SourceId;                            // +0x58
    unsigned long  m_next;                              // +0x5c
    unsigned long  m_queued;                            // +0x60
    void ApplySound(CAudioObject* sound, __int64 now);                    // 0x00C72E00
    long QueueNextBuffer(CircularQueue* queue, __int64 now, bool force);  // 0x00C72C40
    __forceinline bool IsPlaying()
    {
        long state;
        g_AudioContext.GetSourcei(SourceId, 0x1010 /* AL_SOURCE_STATE */, &state);
        return state == 0x1012 /* AL_PLAYING */;
    }
};

long CAudioSource::UpdateStream(CAudioObject* sound, __int64 now)
{
    ApplySound(sound, now);
    CircularQueue* queue = &sound->m_queue;
    unsigned long type = sound->m_flags & 0x7f;
    if (type == 1 || type == 0x10)
        g_AudioContext.SourcePlay(SourceId);
    if ((unsigned char)(m_requests >> 7) & 1 && m_positionB <= now) {
        SetPositionB(0, 0);
        return 8;
    }
    if (queue->m_capacity) {
        long processed;
        g_AudioContext.GetSourcei(SourceId, 0x1016 /* AL_BUFFERS_PROCESSED */, &processed);
        if (processed != 0) {
            for (; processed > 0; --processed) {
                --m_queued;
                unsigned long buffer;
                g_AudioContext.SourceUnqueueBuffers(SourceId, 1, &buffer);
                queue->DequeueNext();
                sound->ReportQueueInfo();
            }
            if (queue->IsEmpty() || queue->m_items[queue->m_tail] == 0 || (queue->m_tail == 0 && !sound->IsLooping()))
                goto stopped;
            m_current = queue->m_capacity ? queue->m_items[queue->m_tail] : 0;
        }
        if (QueueNextBuffer(queue, now, sound->IsLooping()) != 0)
            return 0;
        if (!IsPlaying() && m_queued > 0)
            g_AudioContext.SourcePlay(SourceId);
        return 2;
    } else {
        if (IsPlaying())
            return 2;
    }
stopped:
    m_current = 0;
    return 0;
}
