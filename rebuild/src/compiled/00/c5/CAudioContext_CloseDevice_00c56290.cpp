// CAudioContext::CloseDevice() -- when a device is open: resolve alcCloseDevice from the DLL export table on first use, close the device, forget it (and the +0x1ac word) and reset the context state
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C56290, length 82 bytes (the oracle row carries 14 bytes of int3 padding).
// Callees: GetProcAddress (IAT 0x0143FE44, "alcCloseDevice" @ 0x012B8CA0; alc entry points never go through alGetProcAddress), the
// resolved entry point (cdecl, `add esp,4`), CAudioContext::Reset(bool) @ 0x00C55B70 (`push 0`). The two zero stores are scheduled between
// the push and the call. The manifest label "CEngineWaterRenderer::DestroyWaterGenerator" is propagated.
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

void CAudioContext::CloseDevice()
{
    if (Device) {
        if (!pfnCloseDevice)
            pfnCloseDevice = (PFN_alcCloseDevice)::GetProcAddress(Module, "alcCloseDevice");
        pfnCloseDevice(Device);
        Device = 0;
        m_1ac = 0;
        Reset(false);
    }
}