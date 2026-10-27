// CDSoundStreamBuffer::OpenDevice(const GUID* device, void* window, bool multithreaded) -- bring up DirectSound for the stream buffer: CoInitializeEx (multithreaded or apartment), CoCreateInstance(CLSID_DirectSound) into +0x08, Initialize(device) and SetCooperativeLevel(window, DSSCL_PRIORITY); any failure releases what was made, CoUninitializes, clears +0x08 and returns false
// Leaf: ?OpenDevice@CDSoundStreamBuffer@@UAE_NPBUGUID@@PAX_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C56F70, 156 bytes, thiscall `ret 0Ch`.
// Identity (descriptive method name; GameCODA has no PDB or RTTI): slot 3 of the stream-buffer vtables 0x012B8D90 /
//   0x012B8DF0 (references 0x012B8D9C / 0x012B8DFC), the class of the landed IsChunkReady 0x00C57070 / LockChunk 0x00C570D0
//   and of ReleaseVolume 0x00C57010 (slot 4), which undoes exactly this (Release the +0x08 device, CoUninitialize, clear).
//   GUIDs: CLSID_DirectSound 0x012AC6D4 and IID_IDirectSound 0x012AC644 (bound spellings). Catalog gapscan name is a placeholder.
// Callees: ole32 CoInitializeEx / CoCreateInstance / CoUninitialize (IAT 0x0144063C / 0x01440640 / 0x01440648);
//   IDirectSound::Initialize (slot 10) / SetCooperativeLevel (slot 6) / Release (slot 2).
// Shape: `multithreaded ? 0 : 2` is `setne ; dec ; and 2`; CoInitializeEx accepts S_OK and S_FALSE; the CoCreateInstance and
//   SetCooperativeLevel failures share one CoUninitialize tail, the Initialize failure keeps its own copy.
struct GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
};
extern const GUID CLSID_DirectSound;               // 0x012AC6D4
extern const GUID IID_IDirectSound;                // 0x012AC644

extern "C" __declspec(dllimport) long __stdcall CoInitializeEx(void* reserved, unsigned long coInit);
extern "C" __declspec(dllimport) long __stdcall CoCreateInstance(const GUID& clsid, void* outer, unsigned long context,
                                                                 const GUID& iid, void** object);
extern "C" __declspec(dllimport) void __stdcall CoUninitialize();

struct IDirectSound {                              // COM: __stdcall, `this` pushed
    virtual long __stdcall QueryInterface(const GUID& iid, void** object);   // slot 0
    virtual unsigned long __stdcall AddRef();                                // slot 1
    virtual unsigned long __stdcall Release();                               // slot 2
    virtual long __stdcall CreateSoundBuffer(const void* desc, void** buffer, void* outer);   // slot 3
    virtual long __stdcall GetCaps(void* caps);                              // slot 4
    virtual long __stdcall DuplicateSoundBuffer(void* original, void** duplicate);   // slot 5
    virtual long __stdcall SetCooperativeLevel(void* window, unsigned long level);    // slot 6
    virtual long __stdcall Compact();                                        // slot 7
    virtual long __stdcall GetSpeakerConfig(unsigned long* config);          // slot 8
    virtual long __stdcall SetSpeakerConfig(unsigned long config);           // slot 9
    virtual long __stdcall Initialize(const GUID* device);                   // slot 10
};

class CDSoundStreamBuffer {                        // vtable 0x012B8D90
public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual bool OpenDevice(const GUID* device, void* window, bool multithreaded);   // slot 3
    void*         m_pBuffer;                       // +0x04
    IDirectSound* m_pDevice;                       // +0x08
};

bool CDSoundStreamBuffer::OpenDevice(const GUID* device, void* window, bool multithreaded)
{
    long hr = CoInitializeEx(0, multithreaded ? 0 : 2);
    if (hr != 0 && hr != 1)
        return false;
    if (CoCreateInstance(CLSID_DirectSound, 0, 1, IID_IDirectSound, (void**)&m_pDevice) != 0) {
        CoUninitialize();
        m_pDevice = 0;
        return false;
    }
    if (m_pDevice->Initialize(device) < 0) {
        m_pDevice->Release();
        CoUninitialize();
        m_pDevice = 0;
        return false;
    }
    if (m_pDevice->SetCooperativeLevel(window, 2) < 0) {
        m_pDevice->Release();
        CoUninitialize();
        m_pDevice = 0;
        return false;
    }
    return true;
}