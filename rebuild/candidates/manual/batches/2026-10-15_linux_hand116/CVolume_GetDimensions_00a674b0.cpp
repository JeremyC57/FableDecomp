// CVolume::GetDimensions() const -- width, height and depth of the Direct3D volume (GetDesc) returned by value as an integer 3D vector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A674B0, length 46 bytes (+ int3 padding). Callers: CFrameData-side code @ 0x009FDA3D..0x009FDAF2.
// Leaf: GetDimensions
// Callee: IDirect3DVolume9::GetDesc (COM slot 8, +0x20, __stdcall with the interface pushed) into a 0x1c-byte D3DVOLUME_DESC frame.
// CVolume (vtable 0x0129C840) = {vptr, IDirect3DVolume9* +0x04}.
// Shape: the hidden result pointer is loaded after the call and the three dwords move through ecx/edx/ecx: the result is
// built by a value constructor `C3DVectorI(w, h, d)` straight into the return slot (three named-field stores into a POD
// local copy through esi/edi instead).
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
struct C3DVectorI {
    long X, Y, Z;
    C3DVectorI(long x, long y, long z) : X(x), Y(y), Z(z) {}
};
struct CVolume : CBase {
    IDirect3DVolume9* m_pVolume;
    C3DVectorI GetDimensions() const;
};
C3DVectorI CVolume::GetDimensions() const
{
    D3DVOLUME_DESC desc;
    m_pVolume->GetDesc(&desc);
    return C3DVectorI(desc.Width, desc.Height, desc.Depth);
}
