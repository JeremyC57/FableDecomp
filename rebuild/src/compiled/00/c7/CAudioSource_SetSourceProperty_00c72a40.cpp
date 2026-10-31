// CAudioSource::SetSourceProperty(CItem* item) -- attach a sound buffer to the OpenAL source: translate the handle through the virtual lookup, clear the AL error state, then alSourcei(source, AL_BUFFER, id) and check the error
// Leaf: ?SetSourceProperty@CAudioSource@@
// Grade: RELOCATION_MATCH (wave-2 sec6): rebuilt on the c72800 CAudioContext model (g_AudioContext 0x0143BDB0, inline
//   GetError/Sourcei lazy resolvers, CheckError 0x00C4DBE0 returning long) with chunk 599's reference-local lever
//   (CAudioContext& context = g_AudioContext first): this in esi / buffer in edi as retail; returns PrepareBuffer's
//   negative result or CheckError's
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C72A40, length 170 bytes (+6 int3), ret 4.
// Owner note (audio row, true class CAudioSource): retail offsets -- virtual slot 19 (+0x4c) translates the buffer handle in
// place (returns < 0 on failure), the AL source name at +0x58. The OpenAL entry points are resolved lazily into globals:
// alGetError @ 0x0143BE90 ("alGetError" 0x012B87E8), alSourcei @ 0x0143BE0C ("alSourcei" 0x012B8E58), through the
// OpenAL loader object @ 0x0143BDB0 (module handle +0x04 = 0x0143BDB4, "use the router" byte +0x104 = 0x0143BEB4):
// the router's Resolve @ 0x00C4D740 (as the landed CAudioContextEAX dtor 0x00C71CF0) or GetProcAddress (import
// 0x0143FE44, held in ebx across both lookups). The loader's error check @ 0x00C4DBE0 runs last.
// Shape: each AL entry point is an inline wrapper whose parameters are evaluated before its lazy lookup
// `if (!p) p = flag ? Resolve(name) : GetProcAddress(module, name);` (the name pushed once for both arms), so the source
// and the translated handle sit in esi/edi across the lookup; calling the pointers inline keeps `this` live instead.
// The loader's inline OpenAL wrappers: the parameters are evaluated (into registers) before the lazy lookup.
extern "C" __declspec(dllimport) void* __stdcall GetProcAddress(void* module, const char* name);   // IAT 0x0143FE44
typedef long (__cdecl* PFN_alGetError)();
typedef void (__cdecl* PFN_alSourcei)(unsigned long source, long param, long value);
typedef void (__cdecl* PFN_alGetSourcei)(unsigned long source, long param, long* value);
typedef void (__cdecl* PFN_alSourceStop)(unsigned long source);
typedef void (__cdecl* PFN_alSourceRewind)(unsigned long source);

// Retail OpenAL wrapper (no RTTI; the global instance at 0x0143BDB0): the landed CAudioContext model (0x00C575A0 GetSourcei,
// 0x00C57AF0, 0x00C72AF0): module +4, lazily resolved entry points, alGetProcAddress flag +0x104.
struct CAudioContext {
    void*              __vftable;                   // +0x00
    void*              Module;                      // +0x04 (0x0143BDB4)
    unsigned char      _pad08[0x54];
    PFN_alSourcei      pfnSourcei;                  // +0x5c (0x0143BE0C) "alSourcei"
    unsigned char      _pad60[0x08];
    PFN_alGetSourcei   pfnGetSourcei;               // +0x68 (0x0143BE18) "alGetSourcei"
    unsigned char      _pad6c[0x10];
    PFN_alSourceStop   pfnSourceStop;               // +0x7c (0x0143BE2C) "alSourceStop"
    unsigned char      _pad80[4];
    PFN_alSourceRewind pfnSourceRewind;             // +0x84 (0x0143BE34) "alSourceRewind"
    unsigned char      _pad88[0x58];
    PFN_alGetError     pfnGetError;                 // +0xe0 (0x0143BE90) "alGetError"
    unsigned char      _pade4[0x20];
    bool               UseALGetProcAddress;         // +0x104 (0x0143BEB4)
    void* GetProcAddress(const char* name);         // 0x00C4D740
    void PlaySource(unsigned long source);          // 0x00C575F0 ("alSourcePlay", out of line)
    long CheckError();                              // 0x00C4DBE0
    long GetError()                                 // inline here ("alGetError")
    {
        if (!pfnGetError)
            pfnGetError = (PFN_alGetError)(UseALGetProcAddress ? GetProcAddress("alGetError")
                                                          : ::GetProcAddress(Module, "alGetError"));
        return pfnGetError();
    }
    void Sourcei(unsigned long source, long param, long value)   // inline here (out of line at 0x00C57550)
    {
        if (!pfnSourcei)
            pfnSourcei = (PFN_alSourcei)(UseALGetProcAddress ? GetProcAddress("alSourcei")
                                                        : ::GetProcAddress(Module, "alSourcei"));
        pfnSourcei(source, param, value);
    }
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

long CAudioSource::SetSourceProperty(CItem* item)
{
    CAudioContext& context = g_AudioContext;
    long result = PrepareBuffer(item, (unsigned long*)&item);
    if (result < 0)
        return result;
    context.GetError();
    context.Sourcei(SourceId, 0x1009 /* AL_BUFFER */, (long)item);
    return context.CheckError();
}