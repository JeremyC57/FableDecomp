// CAudioContext::SetEAXGlobalProperty(const void* propertySetGuid, unsigned long property, void* value, unsigned long size) -- virtual EAX setter: call EAXSet with no source and translate the OpenAL error into the audio layer's result code
// Grade: DIFFER(127v127) -- same instructions, block placement only: retail places the default converter right after the compare tree (`cmp 0A001h ; jmp` into a shared `je` with the 0xA004 test) and the `return 0` / `return -3` blocks last; this build falls through into the -3 block after `cmp 0A001h` (tried: shared case body (range tree), separate bodies, default first, switch + trailing if).
// Leaf: SetEAXGlobalProperty
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C564F0, 127 bytes, __thiscall, ret 10h; slot 8 of the vtable at 0x012B8C18
// (no RTTI; slot 0 is the deleting destructor 0x00C565D0).
// The catalog label (CAudioContext::EAXSet) collides with the landed CAudioContext::EAXSet 0x00C558A0, so the
// row is named for what it does.
// Callees: CAudioContext::GetProcAddress 0x00C4D740 ("EAXSet" @ 0x012B8B80), the resolved EAXSet entry point
// (cdecl, five arguments, `add esp,14h`).
// Shape: CAudioContext::EAXSetGlobal (landed at 0x00C56250, the same translation unit) is inlined: lazy
// resolution of the entry point and the call with source 0. The result goes through a `switch`:
// AL_NO_ERROR -> 0, AL_INVALID_NAME / AL_INVALID_VALUE / AL_INVALID_OPERATION (0xA001 / 0xA003 / 0xA004) -> -3
// (VC's compare tree: `cmp 0A003h ; jg ; je` -- separate case bodies; one shared body for the three
// becomes a range test `jl/jg` pivoting on 0xA001); the default case is an inline converter that tests
// for no-error again and maps AL_OUT_OF_MEMORY (0xA005) to -5 and everything else to -9
// (`sete ; lea [ecx*4-9]`).
typedef void* (__cdecl* PFN_alGetProcAddress)(const char* name);
typedef long (__cdecl* PFN_EAXSet)(const void* propertySetGuid, unsigned long property, unsigned long source, void* value, unsigned long size);

enum {
    AL_NO_ERROR = 0,
    AL_INVALID_NAME = 0xA001,
    AL_INVALID_ENUM = 0xA002,
    AL_INVALID_VALUE = 0xA003,
    AL_INVALID_OPERATION = 0xA004,
    AL_OUT_OF_MEMORY = 0xA005
};

enum EAudioResult {
    AUDIO_OK = 0,
    AUDIO_INVALID_ARGUMENT = -3,
    AUDIO_OUT_OF_MEMORY = -5,
    AUDIO_FAILED = -9
};

static inline long ConvertALError(long error)
{
    if (error == AL_NO_ERROR)
        return AUDIO_OK;
    return error == AL_OUT_OF_MEMORY ? AUDIO_OUT_OF_MEMORY : AUDIO_FAILED;
}

struct CAudioContext {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual long SetEAXGlobalProperty(const void* propertySetGuid, unsigned long property, void* value, unsigned long size);   // slot 8
    void*              Module;                           // +0x04
    unsigned char      _pad1[0xe0];
    PFN_alGetProcAddress pfnGetProcAddress;              // +0xe8
    unsigned char      _pad2[4];
    PFN_EAXSet         pfnEAXSet;                        // +0xf0
    void* GetProcAddress(const char* name);              // 0x00C4D740
    long EAXSetGlobal(const void* propertySetGuid, unsigned long property, void* value, unsigned long size)   // 0x00C56250
    {
        if (!pfnEAXSet)
            pfnEAXSet = (PFN_EAXSet)GetProcAddress("EAXSet");
        return pfnEAXSet(propertySetGuid, property, 0, value, size);
    }
};

long CAudioContext::SetEAXGlobalProperty(const void* propertySetGuid, unsigned long property, void* value, unsigned long size)
{
    long error = EAXSetGlobal(propertySetGuid, property, value, size);
    switch (error) {
    case AL_NO_ERROR:
        return AUDIO_OK;
    case AL_INVALID_NAME:
        return AUDIO_INVALID_ARGUMENT;
    case AL_INVALID_VALUE:
        return AUDIO_INVALID_ARGUMENT;
    case AL_INVALID_OPERATION:
        return AUDIO_INVALID_ARGUMENT;
    default:
        return ConvertALError(error);
    }
}
