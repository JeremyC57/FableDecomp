// CVolume::CVolume(const CVolume& other) -- copy constructor: base, vtable, share the volume interface and AddRef it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A673E0, length 40 bytes (+ int3 padding).
// Leaf: ??0CVolume
// Callees: CBase::CBase() @ 0x0099A2D0, COM AddRef (slot 1, +0x04). Vtable 0x0129C840 = CVolume (rtti-vtables.tsv); the
// catalog label "CVolumeTexture::CVolumeTexture" is propagated (CVolumeTexture's vtable is 0x0129DC18).
// Shape: base call, vtable store, the pointer initialised from the other and null-tested in the body (the store lands
// between `test` and `je`).
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
    CVolume(const CVolume& other);
};
CVolume::CVolume(const CVolume& other) : m_pVolume(other.m_pVolume)
{
    if (m_pVolume)
        m_pVolume->AddRef();
}
