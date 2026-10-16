// CVolume::ReadOnlyLock(CLockedVolume& out, unsigned long flags) -- lock the whole volume read-only (flags | D3DLOCK_READONLY) and fill `out` with its dimensions, pitches and bits (left untouched when LockBox fails)
// Grade: DIFFER(151v141) -- the D3DBOX stores use immediates where retail shares an `xor eax,eax` zero (Front, Top, Left order), and the result: retail loads `out` into eax before the HRESULT test and copies the words through edx/ecx/esi in the order +4,+0,+0xc,+8,+0x10,+0x14; ours stores straight from edi/ebx/ebp; tried by-value return (no NRVO in VC7.1: frame copy), named dimension locals, out reference
// Leaf: ?ReadOnlyLock@CVolume@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A67530, length 141 bytes, `ret 8`.
// Callees: IDirect3DVolume9::GetDesc (COM slot 8, +0x20) and LockBox (slot 9, +0x24) on PD3DVolume (re-read after GetDesc).
// Graphics layer (D3D9, to be replaced): CVolume vtable 0x0129C840 = {vptr, IDirect3DVolume9* PD3DVolume +0x04} (Ego_r.pdb);
// the result record {Width, Height, Depth, RowPitch, SlicePitch, Bits} is 24 bytes (local model, name descriptive).
// Shape: the three dimensions are named locals (kept in edi/ebx/ebp across LockBox); the D3DBOX covers the whole volume (Left/Top/Front 0); `out` is assigned from a six-value temporary on
// success (its words shuffle through spare registers); eax is left holding `out`.
struct IUnknownLike {
    virtual long __stdcall QueryInterface(const void* iid, void** out);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
};
struct D3DVOLUME_DESC { long Format; long Type; unsigned long Usage; long Pool; unsigned long Width; unsigned long Height; unsigned long Depth; };
struct D3DBOX { unsigned long Left, Top, Right, Bottom, Front, Back; };
struct D3DLOCKED_BOX { long RowPitch; long SlicePitch; void* pBits; };
struct IDirect3DVolume9 : IUnknownLike {
    virtual long __stdcall GetDevice(void** device);
    virtual long __stdcall SetPrivateData(const void* guid, const void* data, unsigned long size, unsigned long flags);
    virtual long __stdcall GetPrivateData(const void* guid, void* data, unsigned long* size);
    virtual long __stdcall FreePrivateData(const void* guid);
    virtual long __stdcall GetContainer(const void* iid, void** container);
    virtual long __stdcall GetDesc(D3DVOLUME_DESC* desc);             // slot 8 (+0x20)
    virtual long __stdcall LockBox(D3DLOCKED_BOX* locked, const D3DBOX* box, unsigned long flags);   // slot 9 (+0x24)
    virtual long __stdcall UnlockBox();                               // slot 10 (+0x28)
};
struct CLockedVolume {                             // the lock result handed back by value (24 bytes)
    unsigned long Width;                           // +0x00
    unsigned long Height;                          // +0x04
    unsigned long Depth;                           // +0x08
    long          RowPitch;                        // +0x0c
    long          SlicePitch;                      // +0x10
    void*         Bits;                            // +0x14
    CLockedVolume() {}
    CLockedVolume(unsigned long w, unsigned long h, unsigned long d, long row, long slice, void* bits)
        : Width(w), Height(h), Depth(d), RowPitch(row), SlicePitch(slice), Bits(bits) {}
};
struct CBase {
    virtual ~CBase();
};
struct CVolume : CBase {                           // vtable 0x0129C840, 8 bytes
    IDirect3DVolume9* PD3DVolume;                  // +0x04
    void Lock(CLockedVolume& out, unsigned long flags);
    void ReadOnlyLock(CLockedVolume& out, unsigned long flags);
};
void CVolume::ReadOnlyLock(CLockedVolume& out, unsigned long flags)
{
    D3DVOLUME_DESC desc;
    PD3DVolume->GetDesc(&desc);
    unsigned long width = desc.Width;
    unsigned long height = desc.Height;
    unsigned long depth = desc.Depth;
    D3DBOX box;
    box.Left = 0;
    box.Top = 0;
    box.Right = width;
    box.Bottom = height;
    box.Front = 0;
    box.Back = depth;
    D3DLOCKED_BOX locked;
    if (PD3DVolume->LockBox(&locked, &box, flags | 0x10) >= 0)
        out = CLockedVolume(width, height, depth, locked.RowPitch, locked.SlicePitch, locked.pBits);
}
