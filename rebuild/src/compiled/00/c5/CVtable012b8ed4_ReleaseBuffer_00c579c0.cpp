// CVtable_012b8ed4::ReleaseBuffer() -- vtable slot 5 of the OpenAL streaming buffer: when the source exists, stop it, detach its buffer (alSourcei AL_BUFFER 0) and delete it; when the buffers exist, delete them (alDeleteBuffers); free the buffer-name array and clear the count
// Leaf: ?ReleaseBuffer@CVtable_012b8ed4@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C579C0, 297 bytes, thiscall, plain `ret`; slot 5 of vtable 0x012B8ED4 and of the
//   derived 0x012B8E20 (its only references: 0x012B8EE8 / 0x012B8E34).
// Identity (descriptive: libpk / GameCODA have no PDB or RTTI): the class is the landed OpenAL streaming buffer
//   CVtable_012b8ed4 (IsChunkReady 0x00C57AF0 = slot 6: source +0x608, buffer count +0x618); slot 5 of its DirectSound
//   counterpart 0x012B8D98 is the landed ReleaseBuffer 0x00C57040 (stop and release the DirectSound buffer), so this slot
//   gets the same method name. The deleting destructor (slot 0, 0x00C57D60) frees through CPKObject::operator delete
//   0x00C3C370, the libpk root's, which is also what frees the buffer-name array here. The manifest label
//   "CAudioSource_Cleanup" is a guess.
// Callees: CAudioContext::SourceStop 0x00C57640 (out of line), the inline CAudioContext::Sourcei / DeleteSources /
//   DeleteBuffers wrappers (Sourcei / DeleteSources have landed out-of-line copies 0x00C57550 / 0x00C57500; each wrapper resolves
//   "alSourcei" 0x012B8E58 / "alDeleteSources" 0x012B8E48 / "alDeleteBuffers" 0x012B87C8 on first use through
//   CAudioContext::GetProcAddress 0x00C4D740 or the GetProcAddress import 0x0143FE44, then call the cdecl entry point),
//   all on the global context 0x0143BDB0; CPKObject::operator delete 0x00C3C370. AL_BUFFER = 0x1009.
// Shape: the three wrappers are inline members of the context (the entry point stays in eax for the call); SourceStop is
//   a call; the source name is read after it, the delete-buffers arguments before the resolve.
extern "C" __declspec(dllimport) void* __stdcall GetProcAddress(void* module, const char* name);   // IAT 0x0143FE44

typedef void (__cdecl* PFN_alDeleteBuffers)(long count, const unsigned long* buffers);
typedef void (__cdecl* PFN_alDeleteSources)(long count, const unsigned long* sources);
typedef void (__cdecl* PFN_alSourcei)(unsigned long source, long param, long value);

// Retail OpenAL wrapper (no RTTI; the global instance at 0x0143BDB0): the landed CAudioContext model (0x00C57550 Sourcei,
// 0x00C57500 DeleteSources, 0x00C575A0 GetSourcei): module +4, lazily resolved entry points, alGetProcAddress flag +0x104.
struct CAudioContext {
    void*               __vftable;                  // +0x00
    void*               Module;                     // +0x04 (0x0143BDB4)
    unsigned char       _pad08[0x28];
    PFN_alDeleteBuffers pfnDeleteBuffers;           // +0x30 (0x0143BDE0)
    unsigned char       _pad34[0x14];
    PFN_alDeleteSources pfnDeleteSources;           // +0x48 (0x0143BDF8)
    unsigned char       _pad4c[0x10];
    PFN_alSourcei       pfnSourcei;                 // +0x5c (0x0143BE0C)
    unsigned char       _pad60[0xa4];
    bool                UseALGetProcAddress;        // +0x104 (0x0143BEB4)
    void* GetProcAddress(const char* name);         // 0x00C4D740
    void SourceStop(unsigned long source);          // 0x00C57640
    void Sourcei(unsigned long source, long param, long value)            // inline here (out of line at 0x00C57550)
    {
        if (!pfnSourcei)
            pfnSourcei = (PFN_alSourcei)(UseALGetProcAddress ? GetProcAddress("alSourcei")
                                                        : ::GetProcAddress(Module, "alSourcei"));
        pfnSourcei(source, param, value);
    }
    void DeleteSources(long count, const unsigned long* sources)           // inline here (out of line at 0x00C57500)
    {
        if (!pfnDeleteSources)
            pfnDeleteSources = (PFN_alDeleteSources)(UseALGetProcAddress ? GetProcAddress("alDeleteSources")
                                                                    : ::GetProcAddress(Module, "alDeleteSources"));
        pfnDeleteSources(count, sources);
    }
    void DeleteBuffers(long count, const unsigned long* buffers)           // inline here
    {
        if (!pfnDeleteBuffers)
            pfnDeleteBuffers = (PFN_alDeleteBuffers)(UseALGetProcAddress ? GetProcAddress("alDeleteBuffers")
                                                                    : ::GetProcAddress(Module, "alDeleteBuffers"));
        pfnDeleteBuffers(count, buffers);
    }
};
extern CAudioContext g_AudioContext;                // 0x0143BDB0

struct CPKObject {                                  // vtable 0x012B5868: the libpk root
    virtual ~CPKObject() { }
    static void __cdecl operator delete(void* p);   // 0x00C3C370
};

// The OpenAL streaming buffer of the libpk audio driver (vtable 0x012B8ED4, no RTTI; the landed IsChunkReady model):
// the OpenAL counterpart of the DirectSound CDSoundStreamBuffer (vtable 0x012B8D98), slot for slot.
class CVtable_012b8ed4 : public CPKObject {
public:
    virtual bool Initialize(const void* device, void* window, bool multithreaded);   // slot 1: 0x00C57460
    virtual void v02();
    virtual unsigned char CreateAndPlay(unsigned long a, unsigned long b);           // slot 3: 0x00C57800
    virtual bool Create(unsigned long a);                                            // slot 4: 0x00C572C0
    virtual void ReleaseBuffer();                                                    // slot 5: 0x00C579C0
    virtual bool IsChunkReady();                                                     // slot 6: 0x00C57AF0
    unsigned char  _pad04[0x600];
    unsigned long* m_pBuffers;                      // +0x604 OpenAL buffer names
    unsigned long  m_source;                        // +0x608 OpenAL source name
    unsigned char  _pad60c[8];
    bool           m_sourceCreated;                 // +0x614
    bool           m_buffersCreated;                // +0x615
    unsigned char  _pad616[2];
    unsigned long  m_bufferCount;                   // +0x618
    unsigned long  m_queuedCount;                   // +0x61c
};

void CVtable_012b8ed4::ReleaseBuffer()
{
    if (m_sourceCreated) {
        g_AudioContext.SourceStop(m_source);
        g_AudioContext.Sourcei(m_source, 0x1009, 0);            // AL_BUFFER
        g_AudioContext.DeleteSources(1, &m_source);
        m_sourceCreated = false;
    }
    if (m_buffersCreated) {
        g_AudioContext.DeleteBuffers(m_bufferCount, m_pBuffers);
        m_buffersCreated = false;
    }
    if (m_pBuffers) {
        operator delete(m_pBuffers);                             // CPKObject::operator delete
        m_pBuffers = 0;
    }
    m_bufferCount = 0;
}