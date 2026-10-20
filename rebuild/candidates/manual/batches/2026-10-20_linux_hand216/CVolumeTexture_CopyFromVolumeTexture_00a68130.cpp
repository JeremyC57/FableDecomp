// CVolumeTexture::CopyFromVolumeTexture(const CVolumeTexture& source, unsigned long filter) -- copies every mip level the two textures share (min of their level counts) from the source with CVolume::CopyFromVolume (whole volumes, the given filter)
// Leaf: CopyFromVolumeTexture
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A68130, 178 bytes (padded to 192), `ret 8`.
//   PDB name ?CopyFromVolumeTexture@CVolumeTexture@@QAEXABV1@K@Z.
// Callees: IDirect3DVolumeTexture9::GetLevelCount (slot 13, +0x34; the inline GetNoLevels), CVolumeTexture::GetVolumeLevel
//   0x00A67EA0 (CVolume by value, twice per level), CVolume::CopyFromVolume 0x00A67670, the CVolume temporaries' inline
//   destructor (vtable 0x0129C840 store, ReleaseVolume(true) 0x00A67460, CBase::~CBase 0x0099A2E0).
// Shape: `(min)(a, b)` over two const-reference level counts (spilled, `lea` of the winner); in
//   `GetVolumeLevel(i).CopyFromVolume(0, source.GetVolumeLevel(i), 0, filter)` VC evaluates the arguments (right to
//   left) before the object expression; the frame is 8-aligned for the 8-byte CVolume temporaries (as the landed
//   CalcByteLength 0x00A67FF0).
struct D3DVOLUME_DESC { unsigned long Format, Type, Usage, Pool, Width, Height, Depth; };
struct D3DBOX { unsigned long Left, Top, Right, Bottom, Front, Back; };
struct D3DLOCKED_BOX { long RowPitch; long SlicePitch; void* pBits; };
struct IDirect3DVolume9;
struct IDirect3DVolumeTexture9 {                   // IUnknown + IDirect3DResource9 + IDirect3DBaseTexture9 + volume slots
    virtual long __stdcall QueryInterface(const void* iid, void** object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual long __stdcall GetDevice(void** device);
    virtual long __stdcall SetPrivateData(const void* guid, const void* data, unsigned long size, unsigned long flags);
    virtual long __stdcall GetPrivateData(const void* guid, void* data, unsigned long* size);
    virtual long __stdcall FreePrivateData(const void* guid);
    virtual unsigned long __stdcall SetPriority(unsigned long priority);
    virtual unsigned long __stdcall GetPriority();
    virtual void __stdcall PreLoad();
    virtual long __stdcall GetType();
    virtual unsigned long __stdcall SetLOD(unsigned long lod);
    virtual unsigned long __stdcall GetLOD();
    virtual unsigned long __stdcall GetLevelCount();                         // slot 13 (+0x34)
    virtual long __stdcall SetAutoGenFilterType(long filter);
    virtual long __stdcall GetAutoGenFilterType();
    virtual void __stdcall GenerateMipSubLevels();
    virtual long __stdcall GetLevelDesc(unsigned long level, D3DVOLUME_DESC* desc);   // slot 17 (+0x44)
    virtual long __stdcall GetVolumeLevel(unsigned long level, IDirect3DVolume9** volume);   // slot 18 (+0x48)
    virtual long __stdcall LockBox(unsigned long level, D3DLOCKED_BOX* locked, const D3DBOX* box, unsigned long flags);   // slot 19 (+0x4c)
    virtual long __stdcall UnlockBox(unsigned long level);                   // slot 20 (+0x50)
};
struct C3DBoxI;
struct CBase {
    virtual void v00();
    ~CBase();                                      // 0x0099A2E0
};
struct CVolume : CBase {                           // vtable 0x0129C840
    IDirect3DVolume9* PD3DVolume;                  // +0x04
    ~CVolume() { ReleaseVolume(true); }
    void ReleaseVolume(bool flag);                 // 0x00A67460
    long CopyFromVolume(const C3DBoxI* destBox, const CVolume& source, const C3DBoxI* sourceBox, unsigned long filter);   // 0x00A67670
    virtual void v00();
};
template <class T> inline const T& (min)(const T& a, const T& b) { return a < b ? a : b; }
struct CVolumeTexture {                            // vtable 0x0129DC18, 16 bytes (Ego_r.pdb)
    virtual ~CVolumeTexture();
    IDirect3DVolumeTexture9* PD3DVolumeTexture;    // +0x04
    long                     ByteLength;           // +0x08
    void*                    PData;                // +0x0c
    long GetNoLevels() const { return PD3DVolumeTexture->GetLevelCount(); }
    CVolume GetVolumeLevel(long level) const;      // 0x00A67EA0
    void CopyFromVolumeTexture(const CVolumeTexture& source, unsigned long filter);
};
void CVolumeTexture::CopyFromVolumeTexture(const CVolumeTexture& source, unsigned long filter)
{
    long sourceLevels = source.GetNoLevels();
    long levels = GetNoLevels();
    long count = (min)(sourceLevels, levels);
    for (long level = 0; level < count; ++level)
        GetVolumeLevel(level).CopyFromVolume(0, source.GetVolumeLevel(level), 0, filter);
}
