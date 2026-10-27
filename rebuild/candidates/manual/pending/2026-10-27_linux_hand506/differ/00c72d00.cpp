// CAudioSource::BindSound(CAudioSound* sound) -- vtable slot 3 of the OpenAL voice: stop the OpenAL source when it is playing or paused; a sound with a whole-sound buffer item attaches it (SetSourceProperty; on failure the voice's slot 2 runs and the error is returned); a streamed sound resets the queue cursor; otherwise the source's buffer is cleared (alSourcei AL_BUFFER 0); 0 on success
// Grade: DIFFER(256v256) -- register allocation only: retail keeps the GetProcAddress import in ebp, the result in ebx (0 on the clear-buffer path, the SetSourceProperty result in edi with its own epilogue) and the zero in edx; VC7.1 here gives ebx / ebp / edi and shares the result epilogue. Tried: early returns (263v256), single return (this, 256v256), optimize("s") (139v256)
// Leaf: ?BindSound@CAudioSource@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C72D00, 256 bytes, thiscall `ret 4`; the state local is the `push ecx` frame slot.
// Identity: slot 3 of the OpenAL voice vtables 0x012BEE40 (reference 0x012BEE4C) and 0x012B8CC8 / 0x012B8D48 (0x012B8CD4 /
//   0x012B8D54), the class of the landed CAudioSource rows (SourceId +0x58, queue cursor +0x5C / +0x60 as in QueueNextBuffer
//   0x00C72C40). Class and method names are descriptive (libpk: no PDB/RTTI). Catalogued as gap-scan sub_00c72d00.
// Callees: on the global OpenAL context 0x0143BDB0 the inline GetSourcei ("alGetSourcei" into +0x68) and SourceStop ("alSourceStop"
//   0x012B8E84 into +0x7c) resolvers (GetProcAddress import hoisted into ebp), CAudioSource::SetSourceProperty @ 0x00C72A40, the
//   voice's slot 2, CAudioContext::Sourcei @ 0x00C57550.
extern "C" __declspec(dllimport) void* __stdcall GetProcAddress(void* module, const char* name);   // IAT 0x0143FE44
typedef void (__cdecl* PFN_alGetSourcei)(unsigned long source, long param, long* value);
typedef void (__cdecl* PFN_alSourceStop)(unsigned long source);
typedef void (__cdecl* PFN_alSourceRewind)(unsigned long source);

// Retail OpenAL wrapper (no RTTI; the global instance at 0x0143BDB0): the landed CAudioContext model (0x00C575A0 GetSourcei,
// 0x00C57AF0, 0x00C72AF0): module +4, lazily resolved entry points, alGetProcAddress flag +0x104.
struct CAudioContext {
    void*              __vftable;                   // +0x00
    void*              Module;                      // +0x04 (0x0143BDB4)
    unsigned char      _pad08[0x60];
    PFN_alGetSourcei   pfnGetSourcei;               // +0x68 (0x0143BE18) "alGetSourcei"
    unsigned char      _pad6c[0x10];
    PFN_alSourceStop   pfnSourceStop;               // +0x7c (0x0143BE2C) "alSourceStop"
    unsigned char      _pad80[4];
    PFN_alSourceRewind pfnSourceRewind;             // +0x84 (0x0143BE34) "alSourceRewind"
    unsigned char      _pad88[0x7c];
    bool               UseALGetProcAddress;         // +0x104 (0x0143BEB4)
    void* GetProcAddress(const char* name);         // 0x00C4D740
    void PlaySource(unsigned long source);          // 0x00C575F0 ("alSourcePlay", out of line)
    void Sourcei(unsigned long source, long param, long value);   // 0x00C57550 ("alSourcei", out of line)
    void GetSourcei(unsigned long source, long param, long* value)   // inline here (out of line at 0x00C575A0)
    {
        if (!pfnGetSourcei)
            pfnGetSourcei = (PFN_alGetSourcei)(UseALGetProcAddress ? GetProcAddress("alGetSourcei")
                                                                : ::GetProcAddress(Module, "alGetSourcei"));
        pfnGetSourcei(source, param, value);
    }
    void SourceStop(unsigned long source)
    {
        if (!pfnSourceStop)
            pfnSourceStop = (PFN_alSourceStop)(UseALGetProcAddress ? GetProcAddress("alSourceStop")
                                                              : ::GetProcAddress(Module, "alSourceStop"));
        pfnSourceStop(source);
    }
    void SourceRewind(unsigned long source)
    {
        if (!pfnSourceRewind)
            pfnSourceRewind = (PFN_alSourceRewind)(UseALGetProcAddress ? GetProcAddress("alSourceRewind")
                                                                  : ::GetProcAddress(Module, "alSourceRewind"));
        pfnSourceRewind(source);
    }
};
extern CAudioContext g_AudioContext;                // 0x0143BDB0

struct CItem;
// GameCODA voice base (no PDB/RTTI; names descriptive): the landed CCodaVoice (ctor 0x00C59BA0, vtable 0x012B5C10).
class CCodaVoice {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();                             // slot 2 (+0x08)
    virtual long BindSound(class CAudioSound* sound);   // slot 3 (+0x0c): 0x00C72D00 in the OpenAL voice
    virtual bool SupportsEAX();                     // slot 4 (+0x10)
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08() = 0;
    virtual void v09() = 0;
    virtual long CheckPosition(__int64 position);   // slot 10: 0x00C59770
    virtual bool SetPositionB(unsigned long low, unsigned long high) = 0;   // slot 11
    virtual void SetPositionC(__int64 position);    // slot 12: 0x00C597B0
    virtual void v13() = 0;
    virtual void Rewind(long keepState) = 0;        // slot 14 (+0x38): 0x00C72800 in the OpenAL voice
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual long PrepareBuffer(CItem* item, unsigned long* bufferId);   // slot 19 (+0x4C)
    unsigned long  m_08;                            // +0x08 (VC7.1 pads the vfptr to 8 in a class holding an __int64)
    unsigned long  m_0c;                            // +0x0c cleared with the queue cursor for a streamed sound
    __int64        m_endPosition;                   // +0x10
    unsigned long  m_positionBLow;                  // +0x18
    unsigned long  m_positionBHigh;                 // +0x1c
    __int64        m_positionC;                     // +0x20
    unsigned long  m_flags;                         // +0x28
    unsigned long  m_requests;                      // +0x2c
};

class CAudioSound {                                 // the sound a voice plays (landed InitializeProperties 0x00C72AF0: flags +0xc)
public:
    unsigned char  _pad00[0x5c];
    unsigned long  m_streamed;                      // +0x5c non-zero: streamed through the buffer queue
    unsigned char  _pad60[0x18];
    CItem*         m_staticItem;                    // +0x78 the whole-sound buffer item, if any
};

class CAudioSource : public CCodaVoice {            // vtable 0x012BEE40 (also 0x012B8CC8 / 0x012B8D48)
public:
    virtual long BindSound(CAudioSound* sound);     // slot 3: 0x00C72D00
    virtual void Rewind(long keepState);            // slot 14: 0x00C72800
    long SetSourceProperty(CItem* item);            // 0x00C72A40 (catalog name: PrepareBuffer + alSourcei AL_BUFFER)
    unsigned char  _pad30[0x28];
    unsigned long  SourceId;                        // +0x58 OpenAL source name
    unsigned long  m_next;                          // +0x5c next queue slot
    unsigned long  m_queued;                        // +0x60 buffers queued
};

long CAudioSource::BindSound(CAudioSound* sound)
{
    long result = 0;
    long state;
    g_AudioContext.GetSourcei(SourceId, 0x1010 /* AL_SOURCE_STATE */, &state);
    if (state == 0x1012 /* AL_PLAYING */ || state == 0x1013 /* AL_PAUSED */)
        g_AudioContext.SourceStop(SourceId);
    if (sound->m_staticItem) {
        result = SetSourceProperty(sound->m_staticItem);
        if (result < 0)
            v02();
    } else {
        if (sound->m_streamed) {
            m_next = 0;
            m_queued = 0;
            m_0c = 0;
            return 0;
        }
        g_AudioContext.Sourcei(SourceId, 0x1009 /* AL_BUFFER */, 0);
    }
    return result;
}
