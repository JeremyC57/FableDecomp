// CAudioBuffer::Destroy() -- release an OpenAL buffer object: drop its reference on the OpenAL context, clear the AL error, and for each AL buffer it created (flag bit 0: +0, bit 1: +4) delete it with the context suspended (unless it already was) and clear the AL error again; finally poison both handles with 0xC0DAF00D and clear the flags and the description
// Leaf: ?Destroy@CAudioBuffer@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C4D9B0, 546 bytes, thiscall, plain `ret`.
// Identity (descriptive; libpk / GameCODA OpenAL back end, no PDB / RTTI): the counterpart of the landed CAudioBuffer::Create
//   0x00C4DC40 (same object: +0 buffer name, +8 flags, +0xc description; Create bumps the context's reference count, Destroy
//   drops it first); the only caller is the sound buffer's SetupEventNotification 0x00C3C7A0 (landed), which binds
//   ?Destroy@CDeviceBuffer_00c3c7a0@@QAEXXZ (and 0x00C3C720 ?Release@CResourceLink_00c4d9b0@@): NAME_FIXES. The magic
//   0xC0DAF00D ("CODA FOOD") marks a dead handle. The catalog row name is CDeviceBuffer_00c3c7a0::Destroy.
// Callees: CAudioContext::Release 0x00C56420 (landed), the inline OpenAL wrappers of the context (out-of-line copies landed:
//   GetError 0x00C4D960, Suspend 0x00C4D790, Process 0x00C4D7D0, DeleteBuffers 0x00C4D8C0; GetProcAddress 0x00C4D740),
//   KERNEL32 GetProcAddress (IAT 0x0143FE44); the context global g_AudioContext 0x0143BDB0.
// Model: the OpenAL wrapper class exactly as the landed sibling CAudioBuffer::Create 0x00C4DC40 spells it (CAudioContext; the
//   engine/CSalOpenAL.h header is a second, opaque model of the same object: header debt in NOTES.md).
extern "C" __declspec(dllimport) void* __stdcall GetProcAddress(void* module, const char* name);   // IAT 0x0143FE44

typedef void (__cdecl* PFN_alcSuspendContext)(void* context);
typedef void (__cdecl* PFN_alcProcessContext)(void* context);
typedef void (__cdecl* PFN_alGenBuffers)(long count, unsigned long* buffers);
typedef void (__cdecl* PFN_alDeleteBuffers)(long count, const unsigned long* buffers);
typedef void (__cdecl* PFN_alBufferData)(unsigned long buffer, long format, const void* data, long size, long frequency);
typedef long (__cdecl* PFN_alGetError)();

enum {
    AL_NO_ERROR = 0,
    AL_OUT_OF_MEMORY = 0xA005,
    AL_FORMAT_MONO8 = 0x1100,
    AL_FORMAT_MONO16 = 0x1101,
    AL_FORMAT_STEREO8 = 0x1102,
    AL_FORMAT_STEREO16 = 0x1103
};
enum EAudioResult {
    AUDIO_OK = 0,
    AUDIO_OUT_OF_MEMORY = -5,
    AUDIO_FAILED = -9,
    AUDIO_INVALID_FORMAT = -14
};

// Retail OpenAL wrapper (no RTTI; vtable 0x012B8C18, the global instance at 0x0143BDB0): the landed CAudioContext layout.
struct CAudioContext {
    void*                 __vftable;                     // +0x00
    void*                 Module;                        // +0x04
    unsigned char         _pad08[0x1c - 0x08];
    PFN_alcSuspendContext pfnSuspendContext;             // +0x1c
    PFN_alcProcessContext pfnProcessContext;             // +0x20
    unsigned char         _pad24[0x2c - 0x24];
    PFN_alGenBuffers      pfnGenBuffers;                 // +0x2c
    PFN_alDeleteBuffers   pfnDeleteBuffers;              // +0x30
    void*                 pfnOther_34;                   // +0x34
    PFN_alBufferData      pfnBufferData;                 // +0x38
    unsigned char         _pad3c[0xe0 - 0x3c];
    PFN_alGetError        pfnGetError;                   // +0xe0
    unsigned char         _pade4[0x100 - 0xe4];
    void*                 Context;                       // +0x100
    bool                  UseALGetProcAddress;           // +0x104
    unsigned char         _pad105[0x1b0 - 0x105];
    bool                  Suspended;                     // +0x1b0
    unsigned char         _pad1b1[3];
    long                  RefCount;                      // +0x1b4
    void Release();                                      // 0x00C56420 (drop a reference; the last one unloads OpenAL)
    void* GetProcAddress(const char* name);              // 0x00C4D740 (resolve through alGetProcAddress)
    long GetError()                                      // 0x00C4D960
    {
        if (!pfnGetError)
            pfnGetError = (PFN_alGetError)(UseALGetProcAddress ? GetProcAddress("alGetError")
                                                               : ::GetProcAddress(Module, "alGetError"));
        return pfnGetError();
    }
    void Suspend()                                       // 0x00C4D790
    {
        if (!pfnSuspendContext)
            pfnSuspendContext = (PFN_alcSuspendContext)::GetProcAddress(Module, "alcSuspendContext");
        pfnSuspendContext(Context);
        Suspended = true;
    }
    void Process()                                       // 0x00C4D7D0
    {
        if (!pfnProcessContext)
            pfnProcessContext = (PFN_alcProcessContext)::GetProcAddress(Module, "alcProcessContext");
        pfnProcessContext(Context);
        Suspended = false;
    }
    void GenBuffers(long count, unsigned long* buffers)  // 0x00C4D870
    {
        if (!pfnGenBuffers)
            pfnGenBuffers = (PFN_alGenBuffers)(UseALGetProcAddress ? GetProcAddress("alGenBuffers")
                                                                   : ::GetProcAddress(Module, "alGenBuffers"));
        pfnGenBuffers(count, buffers);
    }
    void DeleteBuffers(long count, const unsigned long* buffers)   // 0x00C4D8C0
    {
        if (!pfnDeleteBuffers)
            pfnDeleteBuffers = (PFN_alDeleteBuffers)(UseALGetProcAddress ? GetProcAddress("alDeleteBuffers")
                                                                         : ::GetProcAddress(Module, "alDeleteBuffers"));
        pfnDeleteBuffers(count, buffers);
    }
    void BufferData(unsigned long buffer, long format, const void* data, long size, long frequency)   // 0x00C4D910
    {
        if (!pfnBufferData)
            pfnBufferData = (PFN_alBufferData)(UseALGetProcAddress ? GetProcAddress("alBufferData")
                                                                   : ::GetProcAddress(Module, "alBufferData"));
        pfnBufferData(buffer, format, data, size, frequency);
    }
    long CheckError();                                   // 0x00C4DBE0
};
extern CAudioContext g_AudioContext;                     // 0x0143BDB0


struct SAudioBufferDesc;

struct CAudioBuffer {
    unsigned long           Buffer;                      // +0x00 AL buffer name (flag bit 0)
    unsigned long           SecondBuffer;                // +0x04 AL buffer name (flag bit 1)
    unsigned long           Flags;                       // +0x08 (bit 0 / bit 1: the buffer was created)
    const SAudioBufferDesc* Desc;                        // +0x0c
    void Destroy();
};

void CAudioBuffer::Destroy()
{
    g_AudioContext.Release();
    g_AudioContext.GetError();
    if (Flags & 1) {
        bool wasSuspended = g_AudioContext.Suspended;
        g_AudioContext.Suspend();
        g_AudioContext.DeleteBuffers(1, &Buffer);
        if (!wasSuspended)
            g_AudioContext.Process();
        g_AudioContext.GetError();
    }
    if (Flags & 2) {
        bool wasSuspended = g_AudioContext.Suspended;
        g_AudioContext.Suspend();
        g_AudioContext.DeleteBuffers(1, &SecondBuffer);
        if (!wasSuspended)
            g_AudioContext.Process();
        g_AudioContext.GetError();
    }
    SecondBuffer = 0xC0DAF00D;
    Buffer = 0xC0DAF00D;
    Flags = 0;
    Desc = 0;
}