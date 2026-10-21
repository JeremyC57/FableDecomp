// Grade: DIFFER(451v446) -- retail caches the GetProcAddress dllimport in ebp; lane does not
// CAudioBuffer::Create(const SAudioBufferDesc* desc) -- create the OpenAL buffer for a PCM description: more than two channels fails (-14); the AL error is cleared, the context suspended (unless it already was), one buffer generated into the handle and the context processed again; an AL error then fails (-5); the data is uploaded in the AL format picked from sample width and channel count, and an upload error deletes the buffer (bracketed by the same suspend/process) and fails (-14); otherwise the description is kept, the created flag set and the live-buffer count bumped
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C4DC40, 444 bytes (448 with padding), `ret 4`.
// Callees: the OpenAL entry-point resolver object @ 0x0143BDB0 (as the landed CAudioContextEAX dtor 0x00C71CF0):
//   Resolve(name) 0x00C4D740 (alGetProcAddress path, chosen by the byte at +0x104), GetProcAddress import @ 0x0143FE44
//   (the module at +4), CheckError 0x00C4DBE0, BufferData 0x00C4D910, and the out-of-line context wrappers
//   SuspendContext 0x00C4D790 / DeleteBuffers 0x00C4D8C0 / ProcessContext 0x00C4D7D0 used on the error path.
// Data: "alGetError" 0x012B87E8, "alcSuspendContext" 0x012B8790, "alGenBuffers" 0x012B87B8, "alcProcessContext" 0x012B87A4.
// Layout (resolver, retail offsets): Module +4, alcSuspendContext +0x1c, alcProcessContext +0x20, alGenBuffers +0x2c,
//   alGetError +0xe0, Context +0x100, UseALGetProcAddress +0x104, Suspended +0x1b0, BufferCount +0x1b4. No PDB class.
// Shape: the opening clear/suspend/generate/process run is written with inline resolver helpers (retail inlines them
//   here; the error path calls the out-of-line wrappers); the GetProcAddress import is cached in ebp.
extern "C" __declspec(dllimport) void* __stdcall GetProcAddress(void* module, const char* name);   // IAT 0x0143FE44

typedef long (__cdecl* PFNALGETERROR)();
typedef void (__cdecl* PFNALCCONTEXT)(void* context);
typedef void (__cdecl* PFNALGENBUFFERS)(long count, unsigned long* buffers);

struct CALResolver {                             // the global @ 0x0143BDB0
    void*           _00;                         // +0x00
    void*           Module;                      // +0x04
    unsigned char   _08[0x14];
    PFNALCCONTEXT   alcSuspendContext;           // +0x1c
    PFNALCCONTEXT   alcProcessContext;           // +0x20
    unsigned char   _24[0x8];
    PFNALGENBUFFERS alGenBuffers;                // +0x2c
    unsigned char   _30[0xb0];
    PFNALGETERROR   alGetError;                  // +0xe0
    unsigned char   _e4[0x1c];
    void*           Context;                     // +0x100
    bool            UseALGetProcAddress;         // +0x104
    unsigned char   _105[0xab];
    bool            Suspended;                   // +0x1b0
    unsigned char   _1b1[3];
    long            BufferCount;                 // +0x1b4
    void* Resolve(const char* name);             // 0x00C4D740
    void* GetALProc(const char* name)
    {
        if (UseALGetProcAddress)
            return Resolve(name);
        return GetProcAddress(Module, name);
    }
    long CheckError();                           // 0x00C4DBE0
    void SuspendContext();                       // 0x00C4D790
    void ProcessContext();                       // 0x00C4D7D0
    void DeleteBuffers(long count, unsigned long* buffers);   // 0x00C4D8C0
    void BufferData(unsigned long buffer, long format, const void* data, long size, long frequency);   // 0x00C4D910
};
extern CALResolver g_alResolver;                 // 0x0143BDB0

struct SAudioBufferDesc {
    long          SampleWidth;                   // +0x00 bytes per sample
    unsigned long Channels;                      // +0x04
    long          Frequency;                     // +0x08
    long          Size;                          // +0x0c
    unsigned char _10[8];
    const void*   Data;                          // +0x18
};

struct CAudioBuffer {
    unsigned long           Buffer;              // +0x00 AL buffer name
    unsigned long           _04;
    unsigned long           Flags;               // +0x08
    const SAudioBufferDesc* Desc;                // +0x0c
    long Create(const SAudioBufferDesc* desc);
};

long CAudioBuffer::Create(const SAudioBufferDesc* desc)
{
    if (desc->Channels > 2)
        return -14;
    if (!g_alResolver.alGetError)
        g_alResolver.alGetError = (PFNALGETERROR)g_alResolver.GetALProc("alGetError");
    g_alResolver.alGetError();
    bool wasSuspended = g_alResolver.Suspended;
    if (!g_alResolver.alcSuspendContext)
        g_alResolver.alcSuspendContext = (PFNALCCONTEXT)GetProcAddress(g_alResolver.Module, "alcSuspendContext");
    g_alResolver.alcSuspendContext(g_alResolver.Context);
    g_alResolver.Suspended = true;
    if (!g_alResolver.alGenBuffers)
        g_alResolver.alGenBuffers = (PFNALGENBUFFERS)g_alResolver.GetALProc("alGenBuffers");
    g_alResolver.alGenBuffers(1, &Buffer);
    if (!wasSuspended) {
        if (!g_alResolver.alcProcessContext)
            g_alResolver.alcProcessContext = (PFNALCCONTEXT)GetProcAddress(g_alResolver.Module, "alcProcessContext");
        g_alResolver.alcProcessContext(g_alResolver.Context);
        g_alResolver.Suspended = false;
    }
    if (g_alResolver.CheckError() < 0)
        return -5;
    long format;
    switch (desc->SampleWidth) {
    case 1:
        format = desc->Channels == 1 ? 0x1101 : 0x1103;
        break;
    case 2:
        format = desc->Channels == 1 ? 0x1100 : 0x1102;
        break;
    default:
        format = 0;
        break;
    }
    g_alResolver.BufferData(Buffer, format, desc->Data, desc->Size, desc->Frequency);
    if (g_alResolver.CheckError() < 0) {
        bool suspended = g_alResolver.Suspended;
        g_alResolver.SuspendContext();
        g_alResolver.DeleteBuffers(1, &Buffer);
        if (!suspended)
            g_alResolver.ProcessContext();
        return -14;
    }
    Desc = desc;
    Flags |= 1;
    ++g_alResolver.BufferCount;
    return 0;
}
