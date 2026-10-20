// Grade: DIFFER(171v167) -- register allocation: retail loads desc.Width/Height straight into ebx/ebp and stores the box zeros first (Front, Top, Left) from eax; ours loads through eax/ecx and copies, so the box stores interleave
// CVolumeTexture::Lock(unsigned long level, unsigned long flags) -- locks a whole mip level of the volume texture (box 0..width/height/depth), tells the render statistics the texture is in use and returns the locked box (dimensions, pitches, bits); on failure the result is left unwritten
// Leaf: Lock
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A685E0, 165 bytes (padded to 176), `ret 0Ch` (hidden result + level + flags).
//   PDB name ?Lock@CVolumeTexture@@QAE?AVCVolumeLockedBox@@KK@Z.
// Callees: IDirect3DVolumeTexture9 GetLevelDesc (slot 17, +0x44) and LockBox (slot 19, +0x4c); GFGetSystemManager
//   0x009A4EC0 and the render statistics' texture call 0x00A0AA70 (named as in the landed CVolumeTexture::CreateTexture /
//   Uninitialise rows; called here with 1.0f).
// Shape: the D3DBOX is a frame local filled member by member; a failed lock returns a CVolumeLockedBox built by its
//   empty user constructor (nothing is stored into the hidden result), success constructs it in place from the
//   description and the locked box.
// Layout: CVolumeTexture (Ego_r.pdb, 16 bytes) vptr +0, PD3DVolumeTexture +4, ByteLength +8, PData +0xc;
//   CVolumeLockedBox {Dimensions +0, RowPitch +0xc, SlicePitch +0x10, PMem +0x14}.
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
struct CRenderStatistics {
    void RemoveTexture(void* texture, float weight);    // 0x00A0AA70
};
struct CRenderSystem {
    unsigned char      _pad00[8];
    CRenderStatistics* m_pStatistics;              // +0x08
};
struct CSystemManager {
    unsigned char  _pad00[0x60];
    CRenderSystem* m_pRender;                      // +0x60
};
CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0
struct C3DExtentsI { long X, Y, Z; };
struct CVolumeLockedBox {                          // 24 bytes (Ego_r.pdb)
    C3DExtentsI    Dimensions;                     // +0x00
    long           RowPitch;                       // +0x0c
    long           SlicePitch;                     // +0x10
    unsigned char* PMem;                           // +0x14
    CVolumeLockedBox() { }
    CVolumeLockedBox(long x, long y, long z, long rowPitch, long slicePitch, unsigned char* mem)
    {
        Dimensions.X = x;
        Dimensions.Y = y;
        Dimensions.Z = z;
        RowPitch = rowPitch;
        SlicePitch = slicePitch;
        PMem = mem;
    }
};
struct CVolumeTexture {                            // vtable 0x0129DC18
    virtual ~CVolumeTexture();
    IDirect3DVolumeTexture9* PD3DVolumeTexture;    // +0x04
    long                     ByteLength;           // +0x08
    void*                    PData;                // +0x0c
    CVolumeLockedBox Lock(unsigned long level, unsigned long flags);
};
CVolumeLockedBox CVolumeTexture::Lock(unsigned long level, unsigned long flags)
{
    D3DVOLUME_DESC desc;
    PD3DVolumeTexture->GetLevelDesc(level, &desc);
    long width = desc.Width;
    long height = desc.Height;
    D3DBOX box;
    box.Left = 0;
    box.Top = 0;
    box.Right = width;
    box.Bottom = height;
    box.Front = 0;
    box.Back = desc.Depth;
    CSystemManager* system = GFGetSystemManager();
    system->m_pRender->m_pStatistics->RemoveTexture(PD3DVolumeTexture, 1.0f);
    D3DLOCKED_BOX locked;
    if (PD3DVolumeTexture->LockBox(level, &locked, &box, flags) < 0)
        return CVolumeLockedBox();
    return CVolumeLockedBox(width, height, desc.Depth, locked.RowPitch, locked.SlicePitch,
                            (unsigned char*)locked.pBits);
}
