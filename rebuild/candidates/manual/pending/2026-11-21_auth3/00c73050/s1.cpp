// CAudioSource::UpdateStream(CAudioObject* sound, __int64 now) -- per-tick update of a STREAMING OpenAL source (slot 9): re-apply the sound's
//   properties; (re)start the AL source for sound types 1 / 0x10; when the position-B request is pending and due, clear it and return 8; for a
//   sound with a buffer queue, unqueue every processed AL buffer (advancing the sound's CircularQueue and reporting it), stop (m_current = 0,
//   return 0) when the queue is empty / its tail item is null / it wrapped on a non-looping sound, else track the tail item, queue the next
//   buffer (0 when QueueNextBuffer fails) and restart the source when it stalled with buffers queued (2); a queueless sound returns 2 while
//   the AL source plays, else stops (0).
// Leaf: ?UpdateStream@CAudioSource@@UAEJPAVCAudioObject@@_J@Z
// Flags: /d2inlT145
// Inline-budget: /d2inlT145 (smallest) -- retail expands CAudioContext::SourcePlay / GetSourcei / SourceUnqueueBuffers and both
//   CAudioSource::IsPlaying sites inline (0x00C73080, 0x00C730FC, 0x00C7315F, 0x00C73220, 0x00C73295) but calls the out-of-line
//   SourcePlay 0x00C575F0 at the last site (0x00C73283). T144 and below call CAudioSource::IsPlaying out of line (`mov ecx,esi ; call ;
//   test al,al` at 0x00C73220: DIFFER(600v672); no flag / T100: 525v672); T270 and above also expand that last SourcePlay (DIFFER(720v672)). Window T145..T250.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C73050, 672 bytes, thiscall, ret 0Ch (the 64-bit time by value).
// Callees: CAudioSource::ApplySound 0x00C72E00, CAudioContext::GetProcAddress 0x00C4D740 (resolver arm), GetProcAddress IAT 0x0143FE44,
//   the AL entry points cached in g_AudioContext 0x0143BDB0 (+0x68 alGetSourcei, +0x6C alSourcePlay, +0x90 alSourceUnqueueBuffers),
//   CircularQueue::DequeueNext 0x00C44320, CAudioObject::ReportQueueInfo 0x00C432F0, CAudioSource::QueueNextBuffer 0x00C72C40 (landed),
//   CAudioContext::SourcePlay 0x00C575F0 (landed, out of line), virtual slot 11 SetPositionB (+0x2C; 0x00C727B0 landed).
// Identity: slot 9 of the OpenAL source vtables (slot 8 is the landed CAudioSource::Update 0x00C72FE0, which runs the same 0x00C72E00
//   update). libpk / GameCODA OpenAL layer: no PDB / RTTI record (Ego_d, Ego_r: none), so class, method and member names are descriptive,
//   following the landed CAudioSource / CAudioContext rows (catalogue label: Audio_PlaySourceAndProcessQueue, autoname-t5).
// Layout: no shared header for CAudioSource / CAudioObject / CircularQueue / CAudioContext; local models as every landed CAudioSource row.
// Shape: the AL resolvers name the resolved address (`void* proc = ...; pfnX = (T)proc;`) and call through the member. That body gives the
//   retail bytes of all three out-of-line copies (0x00C575A0, 0x00C576E0, 0x00C575F0) and of every landed inline user (0x00C57C90,
//   0x00C57D80, 0x00C57AF0, 0x00C72620, 0x00C72800, 0x00C72A40); here its extra join lowers the queue pointer's colouring priority
//   (23 -> 15) below the tail-item temporary's, which then gets retail's edx (0x00C731D2 / D5 / F4) instead of edi.
//   `(unsigned char)(m_requests >> 7) & 1` is retail's `shr eax,7 ; test al,1` (0x00C730C1; without the cast DIFFER(669v672)).
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
            pfnSourcePlay = (PFN_alSourcePlay)(UseALGetProcAddress ? GetProcAddress("alSourcePlay") : ::GetProcAddress(Module, "alSourcePlay"));
        pfnSourcePlay(source);
    }
    void GetSourcei(unsigned long source, long param, long* value)
    {
        if (!pfnGetSourcei)
            pfnGetSourcei = (PFN_alGetSourcei)(UseALGetProcAddress ? GetProcAddress("alGetSourcei") : ::GetProcAddress(Module, "alGetSourcei"));
        pfnGetSourcei(source, param, value);
    }
    void SourceUnqueueBuffers(unsigned long source, long count, unsigned long* buffers)
    {
        if (!pfnSourceUnqueueBuffers) {
            void* proc = UseALGetProcAddress ? GetProcAddress("alSourceUnqueueBuffers") : ::GetProcAddress(Module, "alSourceUnqueueBuffers");
            pfnSourceUnqueueBuffers = (PFN_alSourceUnqueueBuffers)proc;
        }
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
    bool IsPlaying()
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
            if (queue->IsEmpty() || queue->m_items[queue->m_tail] == 0 || (queue->m_tail == 0 && !sound->IsLooping())) {
                m_current = 0;
                return 0;
            }
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
    m_current = 0;
    return 0;
}
