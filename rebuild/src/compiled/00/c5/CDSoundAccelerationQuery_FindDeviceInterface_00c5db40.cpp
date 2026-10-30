// CDSoundAccelerationQuery::FindDeviceInterface(const _GUID* deviceGuid) -- load dsound.dll by hand, create the DirectSoundPrivate property set through its class factory and enumerate the render devices (DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_A) so the callback copies the matching device's driver interface path into this object; any failure empties the path
// Leaf: ?FindDeviceInterface@CDSoundAccelerationQuery@@
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C5DB40, 261 bytes, thiscall, `ret 4`; /GS frame (cookie 0x0139C8A8, check 0x00BFE9F9).
// Caller: the DirectSound acceleration probe 0x00C5DD70 (the same object, built on the stack of 0x00C3FB4C by the empty
//   constructor 0x00C5DB30), which skips the registry checks ("DirectSound\Mixer Defaults" "Acceleration",
//   "DirectSound\Device Presence" "VxD"/"WDM") when the path stays empty.
// Callees: LoadLibraryA (IAT 0x0143FE40), GetProcAddress (IAT 0x0143FE44), FreeLibrary (IAT 0x0143FE3C), the module's
//   DllGetClassObject, IClassFactory::CreateInstance / Release, IKsPropertySet::Get / Release (COM slots 3 / 2).
//   Data: CLSID_DirectSoundPrivate 0x012BEEC0, IID_IClassFactory 0x012C06A4, IID_IKsPropertySet 0x012A9714, the module's zero GUID
//   0x012B8FF4 (also read by the callback; uuid.lib's GUID_NULL is 0x012C0694), DSPROPSETID_DirectSoundDevice 0x012BEEB0; the enumeration callback 0x00C5DAA0 (a render device whose id is
//   the requested one, or any when that is all zero: copy its id back and strncpy its Interface string, 0x103 chars + NUL).
// Name: descriptive (libpk / GameCODA: no PDB or RTTI); the classic DirectSoundPrivate device-description recipe.
// Shape: the failure arms only empty the path (no early return: retail carries on with the null handles); the enumeration
//   context {device GUID, owner} and the {callback, context} property data are frame locals.
struct _GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
};

extern "C" __declspec(dllimport) void* __stdcall LoadLibraryA(const char* fileName);               // IAT 0x0143FE40
extern "C" __declspec(dllimport) void* __stdcall GetProcAddress(void* module, const char* name);   // IAT 0x0143FE44
extern "C" __declspec(dllimport) int __stdcall FreeLibrary(void* module);                          // IAT 0x0143FE3C

extern "C" const _GUID CLSID_DirectSoundPrivate;         // 0x012BEEC0 {11AB3EC0-25EC-11D1-A4D8-00C04FC28ACA}
extern "C" const _GUID IID_IClassFactory;                // 0x012C06A4 {00000001-0000-0000-C000-000000000046}
extern "C" const _GUID IID_IKsPropertySet;               // 0x012A9714 {31EFAC30-515C-11D0-A9AA-00AA0061BE93}
extern const _GUID g_anyDeviceGuid;                      // 0x012B8FF4 an all-zero GUID in this module's .rdata ("any render device"; not uuid.lib's GUID_NULL 0x012C0694)
extern "C" const _GUID DSPROPSETID_DirectSoundDevice;    // 0x012BEEB0 {84624F82-25EC-11D1-A4D8-00C04FC28ACA}

struct IUnknown {
    virtual long __stdcall QueryInterface(const _GUID& riid, void** ppvObject) = 0;
    virtual unsigned long __stdcall AddRef() = 0;
    virtual unsigned long __stdcall Release() = 0;
};
struct IClassFactory : public IUnknown {
    virtual long __stdcall CreateInstance(IUnknown* pUnkOuter, const _GUID& riid, void** ppvObject) = 0;
    virtual long __stdcall LockServer(int fLock) = 0;
};
struct IKsPropertySet : public IUnknown {
    virtual long __stdcall Get(const _GUID& rguidPropSet, unsigned long ulId, void* pInstanceData, unsigned long ulInstanceLength,
                               void* pPropertyData, unsigned long ulDataLength, unsigned long* pulBytesReturned) = 0;
    virtual long __stdcall Set(const _GUID& rguidPropSet, unsigned long ulId, void* pInstanceData, unsigned long ulInstanceLength,
                               void* pPropertyData, unsigned long ulDataLength) = 0;
    virtual long __stdcall QuerySupport(const _GUID& rguidPropSet, unsigned long ulId, unsigned long* pulTypeSupport) = 0;
};
typedef long (__stdcall* LPFNGETCLASSOBJECT)(const _GUID& rclsid, const _GUID& riid, void** ppv);

enum { DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_A = 7 };

struct DSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_A_DATA;
typedef int (__stdcall* LPFNDIRECTSOUNDDEVICEENUMERATECALLBACKA)(DSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_A_DATA* description, void* context);
struct DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_A_DATA {
    LPFNDIRECTSOUNDDEVICEENUMERATECALLBACKA Callback;
    void*                                   Context;
};

class CDSoundAccelerationQuery {
public:
    char m_interfacePath[0x104];                         // +0x000 the device's driver interface path, empty when unknown
    struct SEnumContext {
        _GUID                     DeviceGuid;            // the requested device (all zero: the first render device)
        CDSoundAccelerationQuery* Owner;
    };
    static int __stdcall FindDeviceInterfaceCallback(DSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_A_DATA* description, void* context);   // 0x00C5DAA0
    void FindDeviceInterface(const _GUID* deviceGuid);
};

void CDSoundAccelerationQuery::FindDeviceInterface(const _GUID* deviceGuid)
{
    void* module = LoadLibraryA("dsound.dll");
    if (!module)
        m_interfacePath[0] = 0;
    LPFNGETCLASSOBJECT getClassObject = (LPFNGETCLASSOBJECT)GetProcAddress(module, "DllGetClassObject");
    if (!getClassObject) {
        FreeLibrary(module);
        m_interfacePath[0] = 0;
    }
    IClassFactory* factory;
    getClassObject(CLSID_DirectSoundPrivate, IID_IClassFactory, (void**)&factory);
    IKsPropertySet* propertySet;
    if (factory->CreateInstance(0, IID_IKsPropertySet, (void**)&propertySet) != 0) {
        factory->Release();
        FreeLibrary(module);
        m_interfacePath[0] = 0;
    }
    if (!deviceGuid)
        deviceGuid = &g_anyDeviceGuid;
    SEnumContext context;
    context.DeviceGuid = *deviceGuid;
    context.Owner = this;
    DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_A_DATA data;
    data.Callback = FindDeviceInterfaceCallback;
    data.Context = &context;
    unsigned long bytesReturned;
    propertySet->Get(DSPROPSETID_DirectSoundDevice, DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_A, 0, 0, &data, sizeof(data), &bytesReturned);
    propertySet->Release();
    factory->Release();
    FreeLibrary(module);
}