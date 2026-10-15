// CVolume::operator=(const CVolume& other) -- release the held volume interface, take the other's and AddRef it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A67730, length 49 bytes (+ int3 padding).
// Leaf: ??4CVolume
// Callees: COM Release (slot 2, +0x08) and AddRef (slot 1, +0x04), __stdcall with the interface pushed.
// Shape: `if (p) { p->Release(); p = 0; }` keeps the clear inside the test; the new pointer is stored between its
// `test` and the `je` (the plain member assignment before the null-tested AddRef). Returns *this.
struct IUnknownLike {
    virtual long __stdcall QueryInterface(const void* iid, void** out);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
};
struct D3DVOLUME_DESC { long Format; long Type; unsigned long Usage; long Pool; unsigned long Width; unsigned long Height; unsigned long Depth; };
struct IDirect3DVolume9 : IUnknownLike {
    virtual long __stdcall GetDevice(void** device);
    virtual long __stdcall SetPrivateData(const void* guid, const void* data, unsigned long size, unsigned long flags);
    virtual long __stdcall GetPrivateData(const void* guid, void* data, unsigned long* size);
    virtual long __stdcall FreePrivateData(const void* guid);
    virtual long __stdcall GetContainer(const void* iid, void** container);
    virtual long __stdcall GetDesc(D3DVOLUME_DESC* desc);             // slot 8 (+0x20)
};
struct CBase {
    virtual ~CBase();
    CBase();                                // 0x0099A2D0
};
struct CVolume : CBase {
    IDirect3DVolume9* m_pVolume;
    CVolume& operator=(const CVolume& other);
};
CVolume& CVolume::operator=(const CVolume& other)
{
    if (m_pVolume) {
        m_pVolume->Release();
        m_pVolume = 0;
    }
    m_pVolume = other.m_pVolume;
    if (m_pVolume)
        m_pVolume->AddRef();
    return *this;
}
