// CVolumeTexture::ReleaseTexture() -- drop the Direct3D volume texture: when this is the last outside reference (AddRef/Release probe returns 2) take its size off the graphics and volume-texture memory counters, then release it and clear the pointer
// Leaf: ?ReleaseTexture@CVolumeTexture@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A67F60, length 132 bytes (padded to 144).
// Callees: COM AddRef (+0x04) / Release (+0x08, twice).
// Graphics layer (D3D9, to be replaced): CVolumeTexture vtable 0x0129DC18 = {vptr, IDirect3DVolumeTexture9* PD3DVolumeTexture +0x04,
// ByteLength +0x08, PData +0x0c} (Ego_r.pdb); memory counters g_GraphicsMemoryBytes 0x013CAAB0, g_VolumeTextureMemoryBytes 0x013D28E0
// (next to the landed CVolume counter 0x013D28DC); the inline GetMemorySize = (PData ? GetAllocationSize(PData) @ 0x009F2390 on the
// global allocator 0x013CAA90 : ByteLength) + 4, 0 without a texture.
struct D3DVOLUME_DESC_Local { unsigned long Format, Type, Usage, Pool, Width, Height, Depth; };
struct IDirect3DVolume9;
struct IDirect3DVolumeTexture9 {                   // IUnknown + IDirect3DResource9 + IDirect3DBaseTexture9 slots
    virtual long __stdcall QueryInterface(const void* iid, void** object);   // slot 0
    virtual unsigned long __stdcall AddRef();                                // slot 1 (+0x04)
    virtual unsigned long __stdcall Release();                               // slot 2 (+0x08)
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
};
class CMemoryAllocator { public:
    unsigned long GetAllocationSize(void* block);  // 0x009F2390
};
extern CMemoryAllocator* g_pGlobalAllocator_013caa90;   // 0x013CAA90
extern unsigned long g_GraphicsMemoryBytes;        // 0x013CAAB0
extern unsigned long g_VolumeTextureMemoryBytes;   // 0x013D28E0
struct CBase {
    virtual void v00();
    ~CBase();                                      // 0x0099A2E0
};
class CVolume : public CBase { public:                           // vtable 0x0129C840
    IDirect3DVolume9* PD3DVolume;                  // +0x04
    ~CVolume() { ReleaseVolume(true); }
    void ReleaseVolume(bool flag);                 // 0x00A67460
    unsigned long GetByteLength() const;           // 0x00A674E0
    virtual void v00();
};
class CVolumeTexture { public:                            // vtable 0x0129DC18, 16 bytes (Ego_r.pdb)
    virtual ~CVolumeTexture();
    IDirect3DVolumeTexture9* PD3DVolumeTexture;    // +0x04
    long                     ByteLength;           // +0x08
    void*                    PData;                // +0x0c
    unsigned long GetMemorySize() const            // inline: texture bytes (or the preallocated block's size) + 4, 0 when empty
    {
        unsigned long size = 0;
        if (PD3DVolumeTexture)
            size = (PData ? g_pGlobalAllocator_013caa90->GetAllocationSize(PData) : ByteLength) + 4;
        return size;
    }
    void Uninitialise();                           // 0x00A68060
    CVolume GetVolumeLevel(unsigned int level);    // 0x00A67EA0
    void ReleaseTexture();
    void CalcByteLength();
    bool InitialiseFromPreallocatedVolumeTexture(IDirect3DVolumeTexture9* texture);
};
void CVolumeTexture::ReleaseTexture()
{
    if (PD3DVolumeTexture) {
        PD3DVolumeTexture->AddRef();
        if (PD3DVolumeTexture->Release() == 2) {
            g_GraphicsMemoryBytes -= GetMemorySize();
            g_VolumeTextureMemoryBytes -= GetMemorySize();
        }
        PD3DVolumeTexture->Release();
        PD3DVolumeTexture = 0;
    }
}