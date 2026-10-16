// CTexture::Lock(unsigned long level, unsigned long flags) const -- lock a whole mip level and return its width, height, pitch and bits
// Grade: DIFFER(142v138) -- width/height loaded through eax/ecx and copied into ebx/ebp (`mov ebx,eax ; mov ebp,ecx`) where retail loads ebx/ebp straight from the desc; rest identical
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FA450, 136 bytes (+8 int3). __thiscall, hidden result pointer, `ret 0Ch`.
//         RENDER LAYER (D3D9): IDirect3DTexture9 at +0. 11 callers (0x0071BCE6 ... 0x00B672E4).
// Callees: IDirect3DTexture9::GetLevelDesc (COM slot 17, +0x44) and LockRect (slot 19, +0x4C); GFGetSystemManager()
//          0x009A4EC0 (__fastcall); a texture-use hook 0x00A0AA70 reached through system manager +0x60 -> +0x08, called with
//          (texture, 1.0f) -- its retail body is an empty `ret 8` (descriptive name NoteTextureUse).
// Shape: returns CSurfaceLockedRect (PDB mangled name ?Lock@CTexture@@QAE?AVCSurfaceLockedRect@@KK@Z) built in the result
//        on success; the failure path is `return CSurfaceLockedRect();` with an empty user ctor (no stores). D3DSURFACE_DESC local (Width +0x18 / Height +0x1C kept in ebx/ebp); a full-level RECT {0, 0, Width, Height};
//        on a successful LockRect (`test eax,eax ; jl`) the 16-byte result gets Width, Height, Pitch, pBits; on failure it
//        is returned unwritten.
// Layout: PDB CTexture: PD3DTexture +0x00.
struct IDirect3DSurface9;
struct D3DSURFACE_DESC {
    long Format; long Type; unsigned long Usage; long Pool; long MultiSampleType; unsigned long MultiSampleQuality;
    unsigned int Width; unsigned int Height;
};
struct D3DLOCKED_RECT { int Pitch; void* pBits; };
struct RECT { long left; long top; long right; long bottom; };
struct IDirect3DTexture9 {
    virtual long __stdcall QueryInterface(const void* iid, void** object);   // slot 0
    virtual unsigned long __stdcall AddRef();                                // slot 1
    virtual unsigned long __stdcall Release();                               // slot 2
    virtual long __stdcall GetDevice(void** device);                         // slot 3
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
    virtual void __stdcall GenerateMipSubLevels();                           // slot 16
    virtual long __stdcall GetLevelDesc(unsigned long level, D3DSURFACE_DESC* desc);    // slot 17 (+0x44)
    virtual long __stdcall GetSurfaceLevel(unsigned long level, IDirect3DSurface9** surface);   // slot 18 (+0x48)
    virtual long __stdcall LockRect(unsigned long level, D3DLOCKED_RECT* lockedRect, const RECT* rect, unsigned long flags);   // slot 19 (+0x4c)
};
struct CTextureUseHook {
    void NoteTextureUse(IDirect3DTexture9* texture, float weight);   // 0x00A0AA70 (empty in retail)
};
struct CSystemSubManager { unsigned char _pad[8]; CTextureUseHook* Hook; };      // +0x08
struct CSystemManager { unsigned char _pad[0x60]; CSystemSubManager* Sub; };     // +0x60
CSystemManager* __fastcall GFGetSystemManager();                                 // 0x009A4EC0
struct C2DExtentsI {
    long X, Y;
    C2DExtentsI() {}
    C2DExtentsI(long x, long y) : X(x), Y(y) {}
};
struct CSurfaceLockedRect {                      // PDB, 16 bytes
    C2DExtentsI Dimensions;                      // +0x00
    long Pitch;                                  // +0x08
    unsigned char* PMem;                         // +0x0c
    CSurfaceLockedRect() {}
    CSurfaceLockedRect(const C2DExtentsI& dimensions, long pitch, unsigned char* mem)
        : Dimensions(dimensions), Pitch(pitch), PMem(mem) {}
};
class CTexture {
public:
    IDirect3DTexture9* PD3DTexture;                // +0x00
    unsigned long      ByteLength : 28;            // +0x04
    unsigned long      AllocationSource : 4;       // +0x04
    CSurfaceLockedRect Lock(unsigned long level, unsigned long flags) const;
};
CSurfaceLockedRect CTexture::Lock(unsigned long level, unsigned long flags) const
{
    D3DSURFACE_DESC desc;
    PD3DTexture->GetLevelDesc(level, &desc);
    C2DExtentsI size(desc.Width, desc.Height);
    RECT rect;
    rect.top = 0;
    rect.left = 0;
    rect.bottom = size.Y;
    rect.right = size.X;
    CSystemManager* manager = GFGetSystemManager();
    manager->Sub->Hook->NoteTextureUse(PD3DTexture, 1.0f);
    D3DLOCKED_RECT locked;
    if (PD3DTexture->LockRect(level, &locked, &rect, flags) >= 0)
        return CSurfaceLockedRect(size, locked.Pitch, (unsigned char*)locked.pBits);
    return CSurfaceLockedRect();
}
