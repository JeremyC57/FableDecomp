// CAudioContext::GetEnumValue(const char* name) -- resolves alGetEnumValue on first use (through alGetProcAddress
// when so configured, else from the DLL export table); when it cannot be resolved the value is 0, otherwise the
// enum lookup result.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C57730, length 75 bytes.
// Callees: CAudioContext::GetProcAddress 0x00C4D740, GetProcAddress (IAT 0x0143FE44), the resolved entry point (cdecl);
//          "alGetEnumValue" @ 0x012B8EC4 is pushed once before the branch.
// The failure path returns the null entry point itself (`test eax,eax ; mov [esi+0ECh],eax ; jne` then `ret` with the
// zero still in eax) -- no separate `xor eax,eax`.
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

long CAudioContext::GetEnumValue(const char* name)
{
    if (!pfnGetEnumValue) {
        pfnGetEnumValue = (PFN_alGetEnumValue)(UseALGetProcAddress ? GetProcAddress("alGetEnumValue")
                                                                   : ::GetProcAddress(Module, "alGetEnumValue"));
        if (!pfnGetEnumValue)
            return 0;
    }
    return pfnGetEnumValue(name);
}