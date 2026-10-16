// Grade: DIFFER(170v170) -- register rotation only: retail keeps `this` in esi (later reused for the AL source) and the handle in edi; ours swaps esi/edi | tried: inline ternary lookups, if-lookups, inline AL wrappers (this form) | next: lane gap: register rotation
// CAudioSource::SetSourceProperty(long buffer) -- attach a sound buffer to the OpenAL source: translate the handle through the virtual lookup, clear the AL error state, then alSourcei(source, AL_BUFFER, id) and check the error
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
typedef long (__cdecl* PFNALGETERROR)();
typedef void (__cdecl* PFNALSOURCEI)(unsigned long source, long param, long value);
extern "C" __declspec(dllimport) void* __stdcall GetProcAddress(void* module, const char* name);   // import 0x0143FE44

struct COpenALLoader {
    long _f00;                                     // +0x000
    void* m_hModule;                               // +0x004
    unsigned char _pad8[0xfc];                     // +0x008
    bool m_useRouter;                              // +0x104
    void* Resolve(const char* name);               // 0x00C4D740
    void CheckError();                             // 0x00C4DBE0
    void* GetProc(const char* name) { return m_useRouter ? Resolve(name) : GetProcAddress(m_hModule, name); }
};
extern COpenALLoader g_openAL;                     // 0x0143BDB0
extern PFNALGETERROR g_pfnalGetError;              // 0x0143BE90
extern PFNALSOURCEI g_pfnalSourcei;                // 0x0143BE0C

// The loader's inline OpenAL wrappers: the parameters are evaluated (into registers) before the lazy lookup.
inline long alGetError()
{
    if (!g_pfnalGetError)
        g_pfnalGetError = (PFNALGETERROR)g_openAL.GetProc("alGetError");
    return g_pfnalGetError();
}
inline void alSourcei(unsigned long source, long param, long value)
{
    if (!g_pfnalSourcei)
        g_pfnalSourcei = (PFNALSOURCEI)g_openAL.GetProc("alSourcei");
    g_pfnalSourcei(source, param, value);
}

struct CAudioSource {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual long TranslateBuffer(long buffer, long* id);   // slot 19 (+0x4c)
    unsigned char _pad04[0x54];                    // +0x04
    unsigned long m_source;                        // +0x58 AL source name
    void SetSourceProperty(long buffer);
};

void CAudioSource::SetSourceProperty(long buffer)
{
    if (TranslateBuffer(buffer, &buffer) < 0)
        return;
    alGetError();
    alSourcei(m_source, 0x1009, buffer);
    g_openAL.CheckError();
}
