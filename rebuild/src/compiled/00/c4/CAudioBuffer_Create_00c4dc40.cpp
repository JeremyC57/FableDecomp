// CAudioBuffer::Create(const SAudioBufferDesc* desc) -- create the OpenAL buffer for a PCM description: more than two channels fails (-14); the AL error is cleared, the context suspended (unless it already was), one buffer generated into the handle and the context processed again; an AL error then fails (-5); the data is uploaded in the AL format picked from sample width and channel count, and an upload error deletes the buffer (bracketed by the same suspend/process) and fails (-14); otherwise the description is kept, the created flag set and the live-buffer count bumped
// Leaf: ?Create@CAudioBuffer@@
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

struct SAudioBufferDesc {
    long          SampleFormat;                          // +0x00 1: 16-bit PCM, 2: 8-bit PCM
    unsigned long Channels;                              // +0x04
    long          Frequency;                             // +0x08
    long          Size;                                  // +0x0c
    unsigned char _pad10[8];
    const void*   Data;                                  // +0x18
};

struct CAudioBuffer {
    unsigned long           Buffer;                      // +0x00 AL buffer name
    unsigned long           _pad04;
    unsigned long           Flags;                       // +0x08 (bit 0: created)
    const SAudioBufferDesc* Desc;                        // +0x0c
    long Create(const SAudioBufferDesc* desc);
};

static inline long GetALFormat(long sampleFormat, unsigned long channels)
{
    switch (sampleFormat) {
    case 1:
        return channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
    case 2:
        return channels == 1 ? AL_FORMAT_MONO8 : AL_FORMAT_STEREO8;
    }
    return 0;
}

long CAudioBuffer::Create(const SAudioBufferDesc* desc)
{
    if (desc->Channels > 2)
        return AUDIO_INVALID_FORMAT;
    g_AudioContext.GetError();
    bool wasSuspended = g_AudioContext.Suspended;
    g_AudioContext.Suspend();
    g_AudioContext.GenBuffers(1, &Buffer);
    if (!wasSuspended)
        g_AudioContext.Process();
    if (g_AudioContext.CheckError() < 0)
        return AUDIO_OUT_OF_MEMORY;
    g_AudioContext.BufferData(Buffer, GetALFormat(desc->SampleFormat, desc->Channels), desc->Data, desc->Size, desc->Frequency);
    if (g_AudioContext.CheckError() < 0) {
        bool suspended = g_AudioContext.Suspended;
        g_AudioContext.Suspend();
        g_AudioContext.DeleteBuffers(1, &Buffer);
        if (!suspended)
            g_AudioContext.Process();
        return AUDIO_INVALID_FORMAT;
    }
    Flags |= 1;
    Desc = desc;
    ++g_AudioContext.RefCount;
    return AUDIO_OK;
}