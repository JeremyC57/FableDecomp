// CUnprovenKsDevice_012B9148::OpenByDirectSoundGuid(const GUID* pDeviceGuid, long, long) -- find the kernel-streaming interface path of a DirectSound device (any device for a null GUID) through dsound.dll's private property set (DllGetClassObject -> CLSID_DirectSoundPrivate class factory -> IKsPropertySet::Get DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_A with the match callback 0x00C5E220), then open that path with CreateFileA (read/write, overlapped) into the device handle at +8; true when the handle is valid
// Grade: DIFFER(336v332) -- register swap and bool tail: retail keeps hDll in esi and hr in edi (ours edi / esi), and ends `setne al` where ours emits `xor ecx,ecx; setne cl; mov al,cl` (+4 bytes)
// Leaf: ?OpenByDirectSoundGuid@CUnprovenKsDevice_012B9148@@UAE_NPBU_GUID@@JJ@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C5E4E0, 332 bytes, __thiscall virtual, `ret 0Ch` (the second and third arguments are
//   unused), /GS frame 0x12C (cookie 0x0139C8A8, __security_check_cookie 0x00BFE9F9) for the search record's MAX_PATH path.
// Identity: slot 2 (cell 0x012B9150) of the vtable 0x012B9148 (slot 3 is the landed close 0x00C5E630 that CloseHandles +8
//   when not -1). The class is GameCODA's kernel-streaming device (the table sits after the "KsCreatePin" / "ksuser.dll"
//   strings and KSPROPSETID_Connection / KSINTERFACESETID_Standard; its ctor is inlined in the landed 0x00C5ED90). libpk /
//   GameCODA have no PDB or RTTI: class and method names are descriptive (CUnproven, keyed by the vtable).
//   Proven parts: "dsound.dll" 0x012B9018, "DllGetClassObject" 0x012B9004, CLSID_DirectSoundPrivate 0x012BEEC0,
//   IID_IClassFactory 0x012C06A4, IID_IKsPropertySet 0x012A9714, DSPROPSETID_DirectSoundDevice 0x012BEEB0 (bytes checked),
//   property 7 = DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_A ({Callback, Context}, 8 bytes); dsound.h's IKsPropertySet has
//   Get at slot 3 (+0x0C). The callback 0x00C5E220 copies the matching DeviceId and strncpy's Interface (0x103 chars) into
//   the {GUID, char[MAX_PATH]} context. The null-GUID default is GameCODA's zero GUID 0x012B8FF4 (bound g_anyDeviceGuid).
// Callees: LoadLibraryA [0x0143FE40], GetProcAddress [0x0143FE44], FreeLibrary [0x0143FE3C], CreateFileA [0x0143FD48].
typedef long HRESULT;
typedef unsigned long ULONG;
typedef unsigned long DWORD;
typedef int BOOL;
typedef struct _GUID { unsigned long Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; } GUID;   // guiddef.h
typedef GUID IID;
typedef GUID CLSID;
struct HINSTANCE__ { int unused; };
typedef HINSTANCE__* HMODULE;
typedef int (__stdcall* FARPROC)();

extern "C" __declspec(dllimport) HMODULE __stdcall LoadLibraryA(const char* lpLibFileName);                 // [0x0143FE40]
extern "C" __declspec(dllimport) FARPROC __stdcall GetProcAddress(HMODULE hModule, const char* lpProcName);  // [0x0143FE44]
extern "C" __declspec(dllimport) BOOL __stdcall FreeLibrary(HMODULE hLibModule);                             // [0x0143FE3C]
extern "C" __declspec(dllimport) void* __stdcall CreateFileA(const char* lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
                                                            void* lpSecurityAttributes, DWORD dwCreationDisposition,
                                                            DWORD dwFlagsAndAttributes, void* hTemplateFile);  // [0x0143FD48]

extern "C" const GUID CLSID_DirectSoundPrivate;        // 0x012BEEC0 (dsprivate)
extern "C" const GUID IID_IClassFactory;               // 0x012C06A4 (uuid.lib)
extern "C" const GUID IID_IKsPropertySet;              // 0x012A9714 (dsound.h)
extern "C" const GUID DSPROPSETID_DirectSoundDevice;   // 0x012BEEB0 (dsprivate)
extern const GUID g_anyDeviceGuid;                     // 0x012B8FF4 (GameCODA's all-zero GUID)

struct __declspec(novtable) IUnknown {
    virtual HRESULT __stdcall QueryInterface(const IID& riid, void** ppv) = 0;   // 0
    virtual ULONG __stdcall AddRef() = 0;                                        // 1
    virtual ULONG __stdcall Release() = 0;                                       // 2 (+0x08)
};
struct __declspec(novtable) IClassFactory : IUnknown {
    virtual HRESULT __stdcall CreateInstance(IUnknown* pUnkOuter, const IID& riid, void** ppvObject) = 0;   // 3 (+0x0C)
    virtual HRESULT __stdcall LockServer(BOOL fLock) = 0;                                                   // 4
};
struct __declspec(novtable) IKsPropertySet : IUnknown {   // dsound.h order: Get, Set, QuerySupport
    virtual HRESULT __stdcall Get(const GUID& rguidPropSet, ULONG ulId, void* pInstanceData, ULONG ulInstanceLength,
                                  void* pPropertyData, ULONG ulDataLength, ULONG* pulBytesReturned) = 0;   // 3 (+0x0C)
    virtual HRESULT __stdcall Set(const GUID& rguidPropSet, ULONG ulId, void* pInstanceData, ULONG ulInstanceLength,
                                  void* pPropertyData, ULONG ulDataLength) = 0;                            // 4
    virtual HRESULT __stdcall QuerySupport(const GUID& rguidPropSet, ULONG ulId, ULONG* pulTypeSupport) = 0;   // 5
};
typedef HRESULT (__stdcall* LPFNGETCLASSOBJECT)(const CLSID& rclsid, const IID& riid, void** ppv);

struct DSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_A_DATA;
typedef BOOL (__stdcall* LPFNDIRECTSOUNDDEVICEENUMERATECALLBACKA)(DSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_A_DATA* pData, void* pContext);
struct DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_A_DATA {   // dsprivate, 8 bytes
    LPFNDIRECTSOUNDDEVICEENUMERATECALLBACKA Callback;
    void* Context;
};
#define DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_A 7
#define MAX_PATH                 260
#define GENERIC_READ             0x80000000
#define GENERIC_WRITE            0x40000000
#define OPEN_EXISTING            3
#define FILE_ATTRIBUTE_NORMAL    0x00000080
#define FILE_FLAG_OVERLAPPED     0x40000000
#define INVALID_HANDLE_VALUE     ((void*)(long)-1)

struct SKsDeviceSearch {                               // the callback's context: device to find, its KS interface path
    GUID DeviceId;                                     // +0x00
    char Interface[MAX_PATH];                          // +0x10
};
BOOL __stdcall FindKsDeviceCallback(DSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_A_DATA* pData, void* pContext);   // 0x00C5E220

class CUnprovenKsHandle_012B90D4 {                     // vtable 0x012B90D4: the handle holder base (handle -1 at +4)
public:
    virtual ~CUnprovenKsHandle_012B90D4();
    void* m_hHandle;                                   // +0x04
};

class CUnprovenKsDevice_012B9148 : public CUnprovenKsHandle_012B90D4 {   // vtable 0x012B9148
public:
    void* m_hDevice;                                   // +0x08
    virtual ~CUnprovenKsDevice_012B9148();             // slot 0: 0x00C5EC80 (deleting)
    virtual void UnprovenSlot01();                     // slot 1: 0x00C5EDF0
    virtual bool OpenByDirectSoundGuid(const GUID* pDeviceGuid, long unused1, long unused2);   // slot 2: 0x00C5E4E0
    virtual void Close();                              // slot 3: 0x00C5E630
    virtual void UnprovenSlot04();                     // slot 4: 0x00C5E650
    virtual void UnprovenSlot05();                     // slot 5: 0x00C572C0
};

bool CUnprovenKsDevice_012B9148::OpenByDirectSoundGuid(const GUID* pDeviceGuid, long unused1, long unused2)
{
    HMODULE hDll = LoadLibraryA("dsound.dll");
    if (hDll)
    {
        LPFNGETCLASSOBJECT pfnGetClassObject = (LPFNGETCLASSOBJECT)GetProcAddress(hDll, "DllGetClassObject");
        if (pfnGetClassObject)
        {
            IClassFactory* pClassFactory;
            IKsPropertySet* pPropertySet;
            pfnGetClassObject(CLSID_DirectSoundPrivate, IID_IClassFactory, (void**)&pClassFactory);
            if (pClassFactory->CreateInstance(0, IID_IKsPropertySet, (void**)&pPropertySet) != 0)
            {
                pClassFactory->Release();
            }
            else
            {
                if (!pDeviceGuid)
                    pDeviceGuid = &g_anyDeviceGuid;

                SKsDeviceSearch search;
                search.DeviceId = *pDeviceGuid;

                DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_A_DATA data;
                data.Callback = FindKsDeviceCallback;
                data.Context = &search;

                ULONG cbReturned;
                HRESULT hr = pPropertySet->Get(DSPROPSETID_DirectSoundDevice, DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_A,
                                               0, 0, &data, sizeof(data), &cbReturned);
                pPropertySet->Release();
                pClassFactory->Release();
                FreeLibrary(hDll);
                if (hr != 0)
                    return false;

                void* hDevice = CreateFileA(search.Interface, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING,
                                            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0);
                m_hDevice = hDevice;
                return hDevice != INVALID_HANDLE_VALUE;
            }
        }
        FreeLibrary(hDll);
    }
    return false;
}
