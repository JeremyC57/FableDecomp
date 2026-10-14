// CAudioContext::GetProcAddress(const char* name) -- resolves an OpenAL entry point: on first use the DLL's
// alGetProcAddress export is fetched (and, when the context is configured to go through it, re-resolved via itself),
// then the requested name is looked up through it.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C4D740, length 76 bytes.
// Callees: GetProcAddress (IAT 0x0143FE44, "alGetProcAddress" @ 0x012B877C), the resolved alGetProcAddress (cdecl).
// The flag byte at +0x104 is read before the first store (`mov cl ; test cl ; mov [esi+0E8h],eax ; je`); the second
// resolution calls the value just fetched (`call eax`) and overwrites the member.
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

void* CAudioContext::GetProcAddress(const char* name)
{
    if (!pfnGetProcAddress) {
        pfnGetProcAddress = (PFN_alGetProcAddress)::GetProcAddress(Module, "alGetProcAddress");
        if (UseALGetProcAddress)
            pfnGetProcAddress = (PFN_alGetProcAddress)pfnGetProcAddress("alGetProcAddress");
    }
    return pfnGetProcAddress(name);
}
