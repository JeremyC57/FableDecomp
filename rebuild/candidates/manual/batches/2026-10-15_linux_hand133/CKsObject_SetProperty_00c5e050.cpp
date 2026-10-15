// CKsObject::SetProperty(const GUID& propertySet, unsigned long id, void* value, unsigned long size) -- send a kernel-streaming KSPROPERTY_TYPE_SET request (IOCTL_KS_PROPERTY) for one property to the device handle at +4 and report success
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5E050, length 109 bytes (+ int3 padding).
// Callees: KERNEL32!DeviceIoControl through the import slot @ 0x0143FEC0 (`call [__imp]`, __stdcall, eight arguments).
// Label correction: the manifest's "CEngineDebug::Draw3DSphere" is a propagated name. The body builds a 0x18-byte KSPROPERTY on the stack
// (the 16-byte property-set GUID copied from the first argument, the property id, Flags = 2 = KSPROPERTY_TYPE_SET) and issues
// DeviceIoControl(handle, 0x002F0003 = IOCTL_KS_PROPERTY, &property, 0x18, value, size, &bytesReturned, 0). Its callers (around the audio
// driver start-up at 0x00C5DD70 and 0x00C5EAD0) pass a member object at +0x0c and the property-set GUID @ 0x012B90D8 with a 4-byte value,
// so the class is a kernel-streaming device wrapper with its HANDLE at +4; class and member names are descriptive.
// Shape: `sub esp,1Ch` = bytesReturned (+0x00) followed by the KSPROPERTY (+0x04..+0x1b); the GUID's four dwords are copied through edx,
// bytesReturned = 0 and Flags = 2 are immediate stores scheduled among the pushes; `neg ; sbb ; neg` is the bool `result != 0`.
// Leaf: SetProperty
typedef unsigned long DWORD;
typedef int BOOL;

struct GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
};

struct KSPROPERTY {
    GUID          Set;                       // +0x00
    unsigned long Id;                        // +0x10
    unsigned long Flags;                     // +0x14
};

struct OVERLAPPED;

extern "C" __declspec(dllimport) BOOL __stdcall DeviceIoControl(void* device, DWORD ioControlCode, void* inBuffer, DWORD inBufferSize,
    void* outBuffer, DWORD outBufferSize, DWORD* bytesReturned, OVERLAPPED* overlapped);   // import slot 0x0143FEC0

struct CKsObject {
    void* m_vtableOrOwner;                   // +0x00
    void* m_handle;                          // +0x04 (the kernel-streaming object's HANDLE)
    bool SetProperty(const GUID& propertySet, unsigned long id, void* value, unsigned long size);
};

bool CKsObject::SetProperty(const GUID& propertySet, unsigned long id, void* value, unsigned long size)
{
    DWORD bytesReturned = 0;
    KSPROPERTY property;
    property.Set = propertySet;
    property.Id = id;
    property.Flags = 2;                      // KSPROPERTY_TYPE_SET
    return DeviceIoControl(m_handle, 0x002F0003, &property, sizeof(property), value, size, &bytesReturned, 0) != 0;
}
