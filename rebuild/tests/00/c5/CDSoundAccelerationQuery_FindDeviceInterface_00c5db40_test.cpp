// behaviour test for 0x00C5DB40 CDSoundAccelerationQuery::FindDeviceInterface (copy of the logic over stub COM objects)
#include <cstdio>
#include <cstring>
struct _GUID { unsigned long Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; };
static const _GUID CLSID_DirectSoundPrivate = { 0x11AB3EC0, 0x25EC, 0x11D1, { 0xA4, 0xD8, 0x00, 0xC0, 0x4F, 0xC2, 0x8A, 0xCA } };
static const _GUID IID_IClassFactory = { 1, 0, 0, { 0xC0, 0, 0, 0, 0, 0, 0, 0x46 } };
static const _GUID IID_IKsPropertySet = { 0x31EFAC30, 0x515C, 0x11D0, { 0xA9, 0xAA, 0x00, 0xAA, 0x00, 0x61, 0xBE, 0x93 } };
static const _GUID g_anyDeviceGuid = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };
static const _GUID DSPROPSETID_DirectSoundDevice = { 0x84624F82, 0x25EC, 0x11D1, { 0xA4, 0xD8, 0x00, 0xC0, 0x4F, 0xC2, 0x8A, 0xCA } };
static bool same(const _GUID& a, const _GUID& b) { return std::memcmp(&a, &b, sizeof(_GUID)) == 0; }

struct DSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_A_DATA {
    int Type; int DataFlow; _GUID DeviceId; const char* Description; const char* Module; const char* Interface; unsigned long WaveDeviceId;
};
typedef int (*LPFNDIRECTSOUNDDEVICEENUMERATECALLBACKA)(DSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_A_DATA* description, void* context);
struct DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_A_DATA { LPFNDIRECTSOUNDDEVICEENUMERATECALLBACKA Callback; void* Context; };
enum { DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_A = 7 };

static int g_freeCalls = 0, g_releases = 0; static bool g_failCreate = false; static bool g_noLibrary = false;
static _GUID g_devA = { 0xAAAA, 1, 2, { 1, 2, 3, 4, 5, 6, 7, 8 } };
static _GUID g_devB = { 0xBBBB, 1, 2, { 1, 2, 3, 4, 5, 6, 7, 8 } };
struct IKsPropertySet {
    long Get(const _GUID& set, unsigned long id, void*, unsigned long, void* data, unsigned long size, unsigned long* returned)
    {
        if (!same(set, DSPROPSETID_DirectSoundDevice) || id != DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_A || size != sizeof(DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_A_DATA)) return -1;
        DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_A_DATA* d = (DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_A_DATA*)data;
        DSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_A_DATA a = { 0, 0, g_devA, "A", "a.sys", "\\\\?\\devA", 0 };
        DSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_A_DATA b = { 0, 0, g_devB, "B", "b.sys", "\\\\?\\devB", 1 };
        if (d->Callback(&a, d->Context)) d->Callback(&b, d->Context);
        *returned = size;
        return 0;
    }
    unsigned long Release() { ++g_releases; return 0; }
};
static IKsPropertySet g_props;
struct IClassFactory {
    long CreateInstance(void*, const _GUID& riid, void** out)
    {
        if (g_failCreate || !same(riid, IID_IKsPropertySet)) return -1;
        *out = &g_props; return 0;
    }
    unsigned long Release() { ++g_releases; return 0; }
};
static IClassFactory g_factory;
typedef long (*LPFNGETCLASSOBJECT)(const _GUID& rclsid, const _GUID& riid, void** ppv);
static long StubGetClassObject(const _GUID& clsid, const _GUID& riid, void** out)
{
    if (!same(clsid, CLSID_DirectSoundPrivate) || !same(riid, IID_IClassFactory)) return -1;
    *out = &g_factory; return 0;
}
static int g_module = 1;
static void* LoadLibraryA(const char* name) { return (!g_noLibrary && !std::strcmp(name, "dsound.dll")) ? &g_module : 0; }
static void* GetProcAddress(void* module, const char* name) { return (module && !std::strcmp(name, "DllGetClassObject")) ? (void*)StubGetClassObject : 0; }
static int FreeLibrary(void*) { ++g_freeCalls; return 1; }

class CDSoundAccelerationQuery {
public:
    char m_interfacePath[0x104];
    struct SEnumContext { _GUID DeviceGuid; CDSoundAccelerationQuery* Owner; };
    static int FindDeviceInterfaceCallback(DSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_A_DATA* description, void* context);
    void FindDeviceInterface(const _GUID* deviceGuid);
};
// the retail callback 0x00C5DAA0 (not this row; reproduced so the enumeration has an effect)
int CDSoundAccelerationQuery::FindDeviceInterfaceCallback(DSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_A_DATA* description, void* context)
{
    SEnumContext* c = (SEnumContext*)context;
    if (description->DataFlow == 0 && (same(c->DeviceGuid, g_anyDeviceGuid) || same(c->DeviceGuid, description->DeviceId))) {
        c->DeviceGuid = description->DeviceId;
        std::strncpy(c->Owner->m_interfacePath, description->Interface, 0x103);
        c->Owner->m_interfacePath[0x103] = 0;
        return 0;
    }
    return 1;
}
void CDSoundAccelerationQuery::FindDeviceInterface(const _GUID* deviceGuid)
{
    void* module = LoadLibraryA("dsound.dll");
    if (!module)
        m_interfacePath[0] = 0;
    LPFNGETCLASSOBJECT getClassObject = (LPFNGETCLASSOBJECT)GetProcAddress(module, "DllGetClassObject");
    if (!getClassObject) {
        FreeLibrary(module);
        m_interfacePath[0] = 0;
        return;   // test-only: the retail body carries on into a null call here
    }
    IClassFactory* factory;
    getClassObject(CLSID_DirectSoundPrivate, IID_IClassFactory, (void**)&factory);
    IKsPropertySet* propertySet;
    if (factory->CreateInstance(0, IID_IKsPropertySet, (void**)&propertySet) != 0) {
        factory->Release();
        FreeLibrary(module);
        m_interfacePath[0] = 0;
        return;   // test-only: the retail body carries on with an unset pointer here
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
int main()
{
    CDSoundAccelerationQuery q;
    std::strcpy(q.m_interfacePath, "stale");
    q.FindDeviceInterface(0);                         // all zero: the first render device
    if (std::strcmp(q.m_interfacePath, "\\\\?\\devA") || g_freeCalls != 1 || g_releases != 2) { std::printf("00c5db40_TEST FAIL\n"); return 1; }
    q.FindDeviceInterface(&g_devB);                   // a specific device
    if (std::strcmp(q.m_interfacePath, "\\\\?\\devB") || g_freeCalls != 2 || g_releases != 4) { std::printf("00c5db40_TEST FAIL\n"); return 1; }
    g_failCreate = true;                              // property set unavailable: path emptied, factory released, DLL freed
    q.FindDeviceInterface(0);
    if (q.m_interfacePath[0] != 0 || g_freeCalls != 3 || g_releases != 5) { std::printf("00c5db40_TEST FAIL\n"); return 1; }
    g_failCreate = false; g_noLibrary = true; std::strcpy(q.m_interfacePath, "x");
    q.FindDeviceInterface(0);                         // no dsound.dll: path emptied
    if (q.m_interfacePath[0] != 0) { std::printf("00c5db40_TEST FAIL\n"); return 1; }
    std::printf("00c5db40_TEST PASS\n");
    return 0;
}