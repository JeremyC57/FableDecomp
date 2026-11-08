// CDirectSoundDevice::DetectSpeakerConfig() -- ask DirectSound for the speaker configuration and store the device's speaker-mode bit for it (0x80000000 for an unsupported layout); a failed query releases DirectSound and returns false
// Leaf: ?DetectSpeakerConfig@CDirectSoundDevice@@UAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C53340; true length 180 bytes (0x9B of code, nop, 7-entry jump table to 0x00C533F4).
// The catalogue extent (256) also holds four more functions after the int3 padding: 0x00C53400 (`mov eax,ecx ; ret`),
// 0x00C53410 / 0x00C53420 / 0x00C53430 (store the argument at +4 / +8 / +0xC and OR 0x20 / 0x40 / 0x80 into the word at +0,
// `ret 4`).
// Identity: slot 3 of the libpk device vtable 0x012B5BB0 (data ref 0x012B5BBC); the landed CDirectSoundDevice::Initialise
// 0x00C53260 (slot 1) ends with `return DetectSpeakerConfig();`. Descriptive class/method names as landed there (libpk, no
// PDB/RTTI). The body is IDirectSound::GetSpeakerConfig (COM slot 8) then a switch on DSSPEAKER_CONFIG (the low byte):
// HEADPHONE 1 -> 1, MONO 2 -> 2, QUAD 3 -> 8, STEREO 4 -> 4, 5POINT1 6 -> 0x40, 7POINT1 7 -> 0x80, else (SURROUND 5,
// DIRECTOUT) 0x80000000 -- table checked by hand against retail.
// Callees: IDirectSound::GetSpeakerConfig / Release (COM slots 8 / 2), ole32!CoUninitialize (IAT 0x01440648).
// Shape: the failure path is the landed Initialise's inline ReleaseDirectSound helper; the case blocks are laid out in
// source order (MONO, HEADPHONE, STEREO, QUAD, 5.1, 7.1), each `mov [esi+4],imm ; mov al,1 ; ret`.
extern "C" __declspec(dllimport) void __stdcall CoUninitialize();                                          // 0x01440648

struct GUID;

struct IDirectSound {                              // COM: __stdcall, `this` pushed
    virtual long __stdcall QueryInterface(const GUID& iid, void** object);               // slot 0
    virtual unsigned long __stdcall AddRef();                                            // slot 1
    virtual unsigned long __stdcall Release();                                           // slot 2
    virtual long __stdcall CreateSoundBuffer(const void* desc, void** buffer, void* outer);   // slot 3
    virtual long __stdcall GetCaps(void* caps);                                          // slot 4
    virtual long __stdcall DuplicateSoundBuffer(void* original, void** duplicate);       // slot 5
    virtual long __stdcall SetCooperativeLevel(void* window, unsigned long level);       // slot 6
    virtual long __stdcall Compact();                                                    // slot 7
    virtual long __stdcall GetSpeakerConfig(unsigned long* config);                      // slot 8 (+0x20)
    virtual long __stdcall SetSpeakerConfig(unsigned long config);                       // slot 9
    virtual long __stdcall Initialize(const GUID* device);                               // slot 10
};

class CDirectSoundDevice {
public:
    virtual ~CDirectSoundDevice();                                                       // slot 0 (0x00C3E4F0)
    virtual bool Initialise(const GUID* device, void* window, bool multithreaded);       // slot 1 (0x00C53260)
    virtual void Uninitialise();                                                         // slot 2 (0x00C53310)
    virtual bool DetectSpeakerConfig();                                                  // slot 3

    void ReleaseDirectSound()
    {
        m_pDirectSound->Release();
        CoUninitialize();
        m_pDirectSound = 0;
    }

    unsigned long m_speakerMode;                   // +0x04 (0x80000000 = not initialised / unsupported)
    IDirectSound* m_pDirectSound;                  // +0x08
};

bool CDirectSoundDevice::DetectSpeakerConfig()
{
    unsigned long config;
    if (m_pDirectSound->GetSpeakerConfig(&config) < 0) {
        ReleaseDirectSound();
        return false;
    }
    switch ((unsigned char)config) {
    case 2:  m_speakerMode = 2;    break;    // DSSPEAKER_MONO
    case 1:  m_speakerMode = 1;    break;    // DSSPEAKER_HEADPHONE
    case 4:  m_speakerMode = 4;    break;    // DSSPEAKER_STEREO
    case 3:  m_speakerMode = 8;    break;    // DSSPEAKER_QUAD
    case 6:  m_speakerMode = 0x40; break;    // DSSPEAKER_5POINT1
    case 7:  m_speakerMode = 0x80; break;    // DSSPEAKER_7POINT1
    default: m_speakerMode = 0x80000000; break;
    }
    return true;
}