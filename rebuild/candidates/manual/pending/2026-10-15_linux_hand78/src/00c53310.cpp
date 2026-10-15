// CDirectSoundDevice_00c53310::Uninitialise() -- shut the DirectSound object down: when one is held (+8), Release it, balance the CoInitializeEx of the initialiser with CoUninitialize, clear the pointer and reset the status word (+4) to 0x80000000
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C53310, length 38 bytes.
// Callees: IDirectSound8::Release (COM slot 2, `this` pushed), ole32!CoUninitialize (`call [__imp]` @ 0x01440648).
// Shape: `this` in esi, the null test on the member, Release, the import call, then the two immediate stores in
// source order (+8 then +4), `ret`. The manifest's "CSurface::Uninitialise" label is propagated: the body sits
// right after the DirectSound initialiser 0x00C53260 (CoInitializeEx, CoCreateInstance into +8,
// IDirectSound8::Initialize slot 10, SetCooperativeLevel(hwnd, DSSCL_PRIORITY) slot 6), whose failure paths
// end with the same Release + CoUninitialize + clear. The owning class is unidentified (named by address).
extern "C" __declspec(dllimport) void __stdcall CoUninitialize();   // 0x01440648

struct IDirectSound8 {                             // IUnknown slots
    virtual long __stdcall QueryInterface(const void* iid, void** object);   // slot 0
    virtual unsigned long __stdcall AddRef();                                // slot 1
    virtual unsigned long __stdcall Release();                               // slot 2 (+0x08)
};

struct CDirectSoundDevice_00c53310 {
    long           m_reserved00;                   // +0x00
    unsigned long  m_status;                       // +0x04 (0x80000000 = not initialised)
    IDirectSound8* m_pDirectSound;                 // +0x08
    void Uninitialise();
};

void CDirectSoundDevice_00c53310::Uninitialise()
{
    if (m_pDirectSound) {
        m_pDirectSound->Release();
        CoUninitialize();
        m_pDirectSound = 0;
        m_status = 0x80000000;
    }
}
