// CSoundStream_00ca8660::ReleaseBuffers() -- stop and release the COM buffer at +0x98 (returning the failure HRESULT if Stop fails), release the object at +0x9c, return S_OK
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA8660, length 75 bytes (+ int3 padding). Referenced from the no-RTTI vtable 0x012BF7D4.
// Leaf: ReleaseBuffers
// Callees: COM slot 6 (+0x18) and Release (+0x08), __stdcall with the interface pushed.
// The catalog label "CVolume::operator=" is propagated (no argument, `long` result).
// Shape: `jl` straight to the epilogue with the HRESULT still in eax (the early `return hr`), member re-read after the
// call for the Release, `xor eax,eax` shared exit.
struct IUnknownLike {
    virtual long __stdcall QueryInterface(const void* iid, void** out);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual long __stdcall v03(); virtual long __stdcall v04(); virtual long __stdcall v05();
    virtual long __stdcall Stop();  // slot 6 (+0x18)
};
struct CSoundStream_00ca8660 {
    unsigned char _pad[0x98];
    IUnknownLike* m_pBuffer;        // +0x98
    IUnknownLike* m_pNotify;        // +0x9c
    long ReleaseBuffers();
};
long CSoundStream_00ca8660::ReleaseBuffers()
{
    if (m_pBuffer) {
        long hr = m_pBuffer->Stop();
        if (hr < 0)
            return hr;
        m_pBuffer->Release();
        m_pBuffer = 0;
    }
    if (m_pNotify) {
        m_pNotify->Release();
        m_pNotify = 0;
    }
    return 0;
}