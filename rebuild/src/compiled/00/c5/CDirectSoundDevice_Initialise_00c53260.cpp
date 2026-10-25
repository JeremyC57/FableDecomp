// CDirectSoundDevice::Initialise(const GUID* device, void* window, bool multithreaded) -- start COM, create and initialise the DirectSound object for the device, take priority cooperation for the window, then read the speaker configuration; any failure releases what was created and returns false
// Leaf: ?Initialise@CDirectSoundDevice@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C53260, length 171 bytes (+ int3 padding); `ret 0Ch`; slot 1 of the vtable
// 0x012B5BB0 (slot 0 the deleting destructor 0x00C3E4F0, slot 2 the landed Uninitialise 0x00C53310, slot 3 the speaker
// configuration reader 0x00C53340).
// Callees: ole32 CoInitializeEx (IAT 0x0144063C; COINIT_MULTITHREADED 0 / COINIT_APARTMENTTHREADED 2 from the flag:
// `setne ; dec ; and eax,2`), CoCreateInstance (IAT 0x01440640; CLSID_DirectSound @ 0x012AC6D4, IID_IDirectSound @ 0x012AC644,
// CLSCTX_INPROC_SERVER), IDirectSound::Initialize (slot 10) / SetCooperativeLevel (slot 6, DSSCL_PRIORITY) / Release (slot 2),
// CoUninitialize (IAT 0x01440648), the virtual DetectSpeakerConfig (slot 3, its bool result returned).
// The libpk device class has no PDB/RTTI: the names are descriptive (the landed Uninitialise row calls it
// CDirectSoundDevice_00c53310). The catalog label "Audio_InitDirectSound3D" is descriptive too.
// Shape: CoInitializeEx accepts S_OK and S_FALSE; CoCreateInstance must return S_OK (`jne`), the two IDirectSound calls are
// FAILED tests (`jge`). The release path is an inline helper (Release, CoUninitialize, clear): the CoCreateInstance failure
// enters it after the Release.
extern "C" __declspec(dllimport) long __stdcall CoInitializeEx(void* reserved, unsigned long coInit);     // 0x0144063C
extern "C" __declspec(dllimport) long __stdcall CoCreateInstance(const struct GUID& clsid, struct IUnknown* outer,
                                                                 unsigned long context, const struct GUID& iid, void** object);   // 0x01440640
extern "C" __declspec(dllimport) void __stdcall CoUninitialize();                                          // 0x01440648

struct GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
};
extern const GUID CLSID_DirectSound;               // 0x012AC6D4 {47D4D946-62E8-11CF-93BC-444553540000}
extern const GUID IID_IDirectSound;                // 0x012AC644 {279AFA83-4981-11CE-A521-0020AF0BE560}

struct IDirectSound {                              // COM: __stdcall, `this` pushed
    virtual long __stdcall QueryInterface(const GUID& iid, void** object);               // slot 0
    virtual unsigned long __stdcall AddRef();                                            // slot 1
    virtual unsigned long __stdcall Release();                                           // slot 2
    virtual long __stdcall CreateSoundBuffer(const void* desc, void** buffer, void* outer);   // slot 3
    virtual long __stdcall GetCaps(void* caps);                                          // slot 4
    virtual long __stdcall DuplicateSoundBuffer(void* original, void** duplicate);       // slot 5
    virtual long __stdcall SetCooperativeLevel(void* window, unsigned long level);       // slot 6 (+0x18)
    virtual long __stdcall Compact();                                                    // slot 7
    virtual long __stdcall GetSpeakerConfig(unsigned long* config);                      // slot 8
    virtual long __stdcall SetSpeakerConfig(unsigned long config);                       // slot 9
    virtual long __stdcall Initialize(const GUID* device);                               // slot 10 (+0x28)
};

class CDirectSoundDevice {
public:
    virtual ~CDirectSoundDevice();                                                       // slot 0 (0x00C3E4F0)
    virtual bool Initialise(const GUID* device, void* window, bool multithreaded);       // slot 1
    virtual void Uninitialise();                                                         // slot 2 (0x00C53310)
    virtual bool DetectSpeakerConfig();                                                  // slot 3 (0x00C53340)

    void ReleaseDirectSound()
    {
        m_pDirectSound->Release();
        CoUninitialize();
        m_pDirectSound = 0;
    }

    unsigned long m_speakerMode;                   // +0x04 (0x80000000 = not initialised)
    IDirectSound* m_pDirectSound;                  // +0x08
};

bool CDirectSoundDevice::Initialise(const GUID* device, void* window, bool multithreaded)
{
    long hr = CoInitializeEx(0, multithreaded ? 0 : 2);
    if (hr != 0 && hr != 1)
        return false;
    if (CoCreateInstance(CLSID_DirectSound, 0, 1, IID_IDirectSound, (void**)&m_pDirectSound) != 0) {
        CoUninitialize();
        m_pDirectSound = 0;
        return false;
    }
    if (m_pDirectSound->Initialize(device) < 0) {
        ReleaseDirectSound();
        return false;
    }
    if (m_pDirectSound->SetCooperativeLevel(window, 2) < 0) {
        ReleaseDirectSound();
        return false;
    }
    return DetectSpeakerConfig();
}