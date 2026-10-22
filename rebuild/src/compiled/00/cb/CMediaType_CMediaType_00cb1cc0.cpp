// CComHolder_00cb1cc0::Release() -- free the COM task memory at +0x44 when the block at +0x40 is held (both cleared), then release the COM interface at +0x3c (cleared)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB1CC0, length 56 bytes.
// Callees: ole32 CoTaskMemFree (IAT 0x01440630, `call dword ptr [__imp]`), the interface's virtual slot 2 (Release, `__stdcall` with `this` pushed). Shape: the
// clears are immediates; the interface pointer is loaded once for the vtable read and the push. The chunk label "FinishWithDynamicTextures" is propagated; the
// class is named by address.
extern "C" __declspec(dllimport) void __stdcall CoTaskMemFree(void* block);   // IAT 0x01440630

struct IUnknown_00cb1cc0 {
    virtual void __stdcall QueryInterface();       // slot 0
    virtual void __stdcall AddRef();               // slot 1
    virtual void __stdcall Release();              // slot 2 (+0x08)
};

struct CComHolder_00cb1cc0 {
    unsigned char      _pad[0x3c];
    IUnknown_00cb1cc0* m_pInterface;               // +0x3c
    void*              m_pBlock;                   // +0x40
    void*              m_pTaskMemory;              // +0x44
    void Release();
};

void CComHolder_00cb1cc0::Release()
{
    if (m_pBlock) {
        CoTaskMemFree(m_pTaskMemory);
        m_pBlock = 0;
        m_pTaskMemory = 0;
    }
    if (m_pInterface) {
        m_pInterface->Release();
        m_pInterface = 0;
    }
}