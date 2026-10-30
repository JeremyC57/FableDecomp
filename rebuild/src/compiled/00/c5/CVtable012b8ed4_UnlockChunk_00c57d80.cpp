// CVtable_012b8ed4::UnlockChunk(void* chunk) -- hand a written chunk to the OpenAL source: unqueue every buffer the source has processed (alGetSourcei AL_BUFFERS_PROCESSED, then UnqueueBuffer per buffer), queue the new chunk (0x00C57B60), and restart the source (alSourcePlay) unless alGetSourcei AL_SOURCE_STATE reports AL_PLAYING
// Leaf: ?UnlockChunk@CVtable_012b8ed4@@UAEXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C57D80, 239 bytes, thiscall `ret 4` (the chunk argument is not read).
// Identity (descriptive; GameCODA has no PDB or RTTI): the slot after LockChunk of the OpenAL streaming buffer vtable
//   0x012B8ED4 (reference 0x012B8EF4; derived table 0x012B8E20 at 0x012B8E40), the class of the landed IsChunkReady
//   0x00C57AF0; it is the counterpart of the DirectSound CDSoundStreamBuffer::UnlockChunk 0x00C57110 (the same slot of
//   0x012B8D90). The landed UnqueueBuffer 0x00C57C90 names this row as its caller. Catalog gapscan name is a placeholder.
// Callees: the inline CAudioContext::GetSourcei on g_AudioContext 0x0143BDB0 (entry +0x68, "alGetSourcei" 0x012B8E64,
//   resolved through CAudioContext::GetProcAddress 0x00C4D740 or the GetProcAddress import, hoisted into ebx), the
//   out-of-line CAudioContext::SourcePlay 0x00C575F0 (descriptive), UnqueueBuffer 0x00C57C90 (landed on the descriptive
//   COpenALVoice over this same object: reached through a cast so it keeps its one spelling), the chunk queue step
//   0x00C57B60 (descriptive).
// Shape: `while (processed-- > 0)` gives the memory-counter loop and the trailing decrement; AL_PLAYING is 0x1012.
extern "C" __declspec(dllimport) void* __stdcall GetProcAddress(void* module, const char* name);   // IAT 0x0143FE44
typedef void (__cdecl* PFN_alGetSourcei)(unsigned long source, long param, long* value);

struct CAudioContext {
    void*            __vftable;                     // +0x00
    void*            Module;                        // +0x04 (0x0143BDB4)
    unsigned char    _pad08[0x60];
    PFN_alGetSourcei pfnGetSourcei;                 // +0x68 (0x0143BE18)
    unsigned char    _pad6c[0x98];
    bool             UseALGetProcAddress;           // +0x104 (0x0143BEB4)
    void* GetProcAddress(const char* name);         // 0x00C4D740
    void SourcePlay(unsigned long source);          // 0x00C575F0 (alSourcePlay, out of line)
    void GetSourcei(unsigned long source, long param, long* value)   // inline here (out of line at 0x00C575A0)
    {
        if (!pfnGetSourcei)
            pfnGetSourcei = (PFN_alGetSourcei)(UseALGetProcAddress ? GetProcAddress("alGetSourcei")
                                                        : ::GetProcAddress(Module, "alGetSourcei"));
        pfnGetSourcei(source, param, value);
    }
};
extern CAudioContext g_AudioContext;                // 0x0143BDB0

struct COpenALVoice {
    void UnqueueBuffer();                           // 0x00C57C90
};

class CVtable_012b8ed4 {                            // vtable 0x012B8ED4
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07();
    virtual void UnlockChunk(void* chunk);          // 0x00C57D80
    unsigned char  _pad04[0x600];
    unsigned long  m_604;                           // +0x604
    unsigned long  m_source;                        // +0x608 OpenAL source name
    void QueueChunk();                              // 0x00C57B60 (its defining row's leaf)
};

void CVtable_012b8ed4::UnlockChunk(void* chunk)
{
    long processed;
    g_AudioContext.GetSourcei(m_source, 0x1016, &processed);
    while (processed-- > 0)
        ((COpenALVoice*)this)->UnqueueBuffer();
    QueueChunk();
    long state;
    g_AudioContext.GetSourcei(m_source, 0x1010, &state);
    if (state != 0x1012)
        g_AudioContext.SourcePlay(m_source);
}