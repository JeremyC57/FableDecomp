// CAudioContext::SourceUnqueueBuffers(unsigned long source, long count, unsigned long* buffers) -- resolves
// alSourceUnqueueBuffers on first use (through alGetProcAddress when so configured, else from the DLL export table)
// and forwards the three arguments.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C576E0, length 79 bytes.
// Callees: CAudioContext::GetProcAddress 0x00C4D740, GetProcAddress (IAT 0x0143FE44), the resolved entry point (cdecl);
//          "alSourceUnqueueBuffers" @ 0x012B8EAC is pushed once before the branch.
// Inferred: named resolved address (NF217, 2026-11-21, q12380) -- the SourceUnqueueBuffers resolver keeps the resolved entry point in a
//   named `void* proc` before the member store and calls through the member, the one spelling of landed 0x00C73050 CAudioSource::UpdateStream
//   (batches/2026-11-21_landing_auth3; AUTH3 measured this text on 0x00C576E0: pending/2026-11-21_auth3/00c73050/cons/), so the TU has one
//   spelling of CAudioContext::SourceUnqueueBuffers. The bytes do not tell it from the HEAD ternary here (same grade); 0x00C73050's do.
//   The other resolvers stay on the ternary.
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

void CAudioContext::SourceUnqueueBuffers(unsigned long source, long count, unsigned long* buffers)
{
    if (!pfnSourceUnqueueBuffers) {
        void* proc = UseALGetProcAddress ? GetProcAddress("alSourceUnqueueBuffers") : ::GetProcAddress(Module, "alSourceUnqueueBuffers");
        pfnSourceUnqueueBuffers = (PFN_alSourceUnqueueBuffers)proc;
    }
    pfnSourceUnqueueBuffers(source, count, buffers);
}