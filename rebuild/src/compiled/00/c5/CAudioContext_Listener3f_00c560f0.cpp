// CAudioContext::Listener3f(long param, float v1, float v2, float v3) -- resolves alListener3f on first use (through
// alGetProcAddress when so configured, else from the DLL export table) and forwards the four arguments.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C560F0, length 84 bytes.
// Callees: CAudioContext::GetProcAddress 0x00C4D740, GetProcAddress (IAT 0x0143FE44), the resolved alListener3f (cdecl);
//          "alListener3f" @ 0x012B8C5C is pushed once before the branch (the common argument of both resolutions).
// The float arguments are forwarded as dwords (no FPU traffic): the pointer type takes floats and the parameters are
// floats, so VC7.1 copies the slots.
struct CAudioContext;
extern "C" __declspec(dllimport) void* __stdcall GetProcAddress(void* module, const char* name);   // IAT 0x0143FE44

typedef void* (__cdecl* PFN_alGetProcAddress)(const char* name);
typedef void (__cdecl* PFN_alcProcessContext)(void* context);
typedef void (__cdecl* PFN_alListener3f)(long param, float v1, float v2, float v3);
typedef void (__cdecl* PFN_alSourceUnqueueBuffers)(unsigned long source, long count, unsigned long* buffers);
typedef long (__cdecl* PFN_alGetEnumValue)(const char* name);

// Retail OpenAL wrapper: the DLL module handle at +4, lazily resolved entry points, the device context at +0x100 and
// the flag at +0x104 that routes resolution through alGetProcAddress instead of the DLL export table.
struct CAudioContext {
    void* __vftable;
    void* Module;                                        // +4
    unsigned char _pad0[0x18];
    PFN_alcProcessContext pfnProcessContext;             // +0x20
    unsigned char _pad1[0x6c];
    PFN_alSourceUnqueueBuffers pfnSourceUnqueueBuffers;  // +0x90
    unsigned char _pad2[4];
    PFN_alListener3f pfnListener3f;                      // +0x98
    unsigned char _pad3[0x4c];
    PFN_alGetProcAddress pfnGetProcAddress;              // +0xe8
    PFN_alGetEnumValue pfnGetEnumValue;                  // +0xec
    unsigned char _pad4[0x10];
    void* Context;                                       // +0x100
    bool UseALGetProcAddress;                            // +0x104
    unsigned char _pad5[3];
    long PendingUpdates;                                 // +0x108
    unsigned char _pad6[0xa4];
    bool ContextDirty;                                   // +0x1b0
    void* GetProcAddress(const char* name);              // 0x00C4D740
    void ProcessContext();                               // 0x00C56020
    void Listener3f(long param, float v1, float v2, float v3);                      // 0x00C560F0
    void SourceUnqueueBuffers(unsigned long source, long count, unsigned long* buffers);   // 0x00C576E0
    long GetEnumValue(const char* name);                 // 0x00C57730
};

void CAudioContext::Listener3f(long param, float v1, float v2, float v3)
{
    if (!pfnListener3f)
        pfnListener3f = (PFN_alListener3f)(UseALGetProcAddress ? GetProcAddress("alListener3f")
                                                               : ::GetProcAddress(Module, "alListener3f"));
    pfnListener3f(param, v1, v2, v3);
}