// CAudioContext::EAXSetSource(unsigned long source, unsigned long property, void* value, unsigned long size) -- EAXSet on the source (buffer) property set (the GUID constant @ 0x012BEEA0) for one source: resolve the entry point on first use and call it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C73380, length 67 bytes (the oracle row carries 13 bytes of int3 padding).
// Callees: CAudioContext::GetProcAddress @ 0x00C4D740 ("EAXSet" @ 0x012B8B80), the resolved entry point (cdecl, five arguments, `add esp,14h`).
// The source is the FIRST parameter (`[esp+8]`) but the third argument of the call (the property, `[esp+0Ch]`, goes second). `ret 10h`.
// The manifest label is propagated.
struct CAudioContext;
extern "C" __declspec(dllimport) void* __stdcall GetProcAddress(void* module, const char* name);   // IAT 0x0143FE44

typedef void* (__cdecl* PFN_alGetProcAddress)(const char* name);
typedef long (__cdecl* PFN_EAXSet)(const void* propertySetGuid, unsigned long property, unsigned long source, void* value, unsigned long size);
typedef long (__cdecl* PFN_EAXGet)(const void* propertySetGuid, unsigned long property, unsigned long source, void* value, unsigned long size);
typedef void (__cdecl* PFN_alcCloseDevice)(void* device);

// Retail OpenAL wrapper (same model as the landed CAudioContext::Listener3f @ 0x00C560F0 family): the DLL module handle at +4, lazily
// resolved entry points, the device at +0xfc, the context at +0x100 and the alGetProcAddress flag at +0x104.
struct CAudioContext {
    void*              __vftable;
    void*              Module;                           // +0x04
    unsigned char      _pad0[0xc];
    PFN_alcCloseDevice pfnCloseDevice;                   // +0x14
    unsigned char      _pad1[0xd0];
    PFN_alGetProcAddress pfnGetProcAddress;              // +0xe8
    unsigned char      _pad2[4];
    PFN_EAXSet         pfnEAXSet;                        // +0xf0
    PFN_EAXGet         pfnEAXGet;                        // +0xf4
    unsigned char      _pad3[4];
    void*              Device;                           // +0xfc
    void*              Context;                          // +0x100
    bool               UseALGetProcAddress;              // +0x104
    unsigned char      _pad4[0xa7];
    long               m_1ac;                            // +0x1ac
    void* GetProcAddress(const char* name);              // 0x00C4D740
    void Reset(bool keepDevice);                         // 0x00C55B70
    long EAXSet(const void* propertySetGuid, unsigned long property, unsigned long source, void* value, unsigned long size);          // 0x00C558A0
    long EAXGet(const void* propertySetGuid, unsigned long property, unsigned long source, void* value, unsigned long size);          // 0x00C558F0
    long EAXGetGlobal(const void* propertySetGuid, unsigned long property, void* value, unsigned long size);                          // 0x00C55940
    long EAXSetGlobal(const void* propertySetGuid, unsigned long property, void* value, unsigned long size);                          // 0x00C56250
    long EAXSetListener(unsigned long property, void* value, unsigned long size);                                                    // 0x00C56590
    long EAXSetSource(unsigned long source, unsigned long property, void* value, unsigned long size);                                // 0x00C73380
    void CloseDevice();                                  // 0x00C56290
};

extern const unsigned char g_eaxSourcePropertySetGuid_012beea0[16];   // 0x012BEEA0

long CAudioContext::EAXSetSource(unsigned long source, unsigned long property, void* value, unsigned long size)
{
    if (!pfnEAXSet)
        pfnEAXSet = (PFN_EAXSet)GetProcAddress("EAXSet");
    return pfnEAXSet(g_eaxSourcePropertySetGuid_012beea0, property, source, value, size);
}