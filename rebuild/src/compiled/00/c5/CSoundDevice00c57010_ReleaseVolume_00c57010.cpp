// CSoundDevice_00c57010::ReleaseVolume() -- run virtual slot 5, then release the COM object at +0x08, CoUninitialize and clear it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C57010, length 36 bytes (+ int3 padding). Referenced from two no-RTTI
// vtables (0x012B8DA0, 0x012B8E00), so the class name is descriptive.
// Leaf: ReleaseVolume
// Callees: this->vfunc5 (+0x14), COM Release (+0x08), CoUninitialize through the import slot @ 0x01440648.
// Shape: `if (p) { p->Release(); CoUninitialize(); p = 0; }`; the import is declared dllimport + __stdcall (no stack cleanup).
extern "C" __declspec(dllimport) void __stdcall CoUninitialize();
struct IUnknownLike {
    virtual long __stdcall QueryInterface(const void* iid, void** out);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
};
struct CSoundDevice_00c57010 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void Shutdown();        // slot 5 (+0x14)
    long m_x;
    IUnknownLike* m_pDevice;        // +0x08
    void ReleaseVolume();
};
void CSoundDevice_00c57010::ReleaseVolume()
{
    Shutdown();
    if (m_pDevice) {
        m_pDevice->Release();
        CoUninitialize();
        m_pDevice = 0;
    }
}