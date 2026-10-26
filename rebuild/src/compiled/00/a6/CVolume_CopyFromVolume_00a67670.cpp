// CVolume::CopyFromVolume(const C3DBoxI* destBox, const CVolume& source, const C3DBoxI* sourceBox, unsigned long filter) -- copy (and filter) a region of another volume into this one with D3DXLoadVolumeFromVolume; a null box means the whole volume
// Leaf: ?CopyFromVolume@CVolume@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A67670, length 151 bytes, `ret 10h`; returns the D3DX HRESULT.
// Callees: D3DXLoadVolumeFromVolume @ 0x00BFEFEC (statically linked D3DX, `__stdcall`: dest volume, dest palette 0, dest box,
// source volume, source palette 0, source box, filter, colour key 0).
// Graphics layer (D3D9/D3DX, to be replaced): CVolume vtable 0x0129C840 = {vptr, IDirect3DVolume9* PD3DVolume +0x04}; C3DBoxI
// (Ego_r.pdb, 24 bytes) = {TL.X +0, TL.Y +4, BR.X +8, BR.Y +0xc, TLZ +0x10, BRZ +0x14} converted to a D3DBOX {Left, Top, Right,
// Bottom, Front, Back} frame copy (Top, Bottom, Left, Right, Front, Back store order).
struct IDirect3DVolume9;
struct D3DBOX { unsigned long Left, Top, Right, Bottom, Front, Back; };
struct PALETTEENTRY;
extern "C" long __stdcall D3DXLoadVolumeFromVolume(IDirect3DVolume9* destVolume, const PALETTEENTRY* destPalette, const D3DBOX* destBox,
    IDirect3DVolume9* srcVolume, const PALETTEENTRY* srcPalette, const D3DBOX* srcBox, unsigned long filter, unsigned long colorKey);   // 0x00BFEFEC
class C2DCoordI { public: long X, Y; };
class C3DBoxI { public:
    C2DCoordI TL;                                  // +0x00
    C2DCoordI BR;                                  // +0x08
    long      TLZ;                                 // +0x10
    long      BRZ;                                 // +0x14
};
inline void ToD3DBox(D3DBOX& out, const C3DBoxI& box)
{
    out.Top = box.TL.Y;
    out.Bottom = box.BR.Y;
    out.Left = box.TL.X;
    out.Right = box.BR.X;
    out.Front = box.TLZ;
    out.Back = box.BRZ;
}
struct CBase {
    virtual ~CBase();
};
class CVolume : public CBase { public:                           // vtable 0x0129C840
    IDirect3DVolume9* PD3DVolume;                  // +0x04
    long CopyFromVolume(const C3DBoxI* destBox, const CVolume& source, const C3DBoxI* sourceBox, unsigned long filter);
};
long CVolume::CopyFromVolume(const C3DBoxI* destBox, const CVolume& source, const C3DBoxI* sourceBox, unsigned long filter)
{
    D3DBOX dest;
    D3DBOX src;
    const D3DBOX* pDest = 0;
    const D3DBOX* pSrc = 0;
    if (destBox) {
        ToD3DBox(dest, *destBox);
        pDest = &dest;
    }
    if (sourceBox) {
        ToD3DBox(src, *sourceBox);
        pSrc = &src;
    }
    return D3DXLoadVolumeFromVolume(PD3DVolume, 0, pDest, source.PD3DVolume, 0, pSrc, filter, 0);
}