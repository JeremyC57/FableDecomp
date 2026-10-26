// CVolumeTexture::InitialiseFromPreallocatedVolumeTexture(IDirect3DVolumeTexture9* texture) -- adopt an existing volume texture: uninitialise, store it, compute its byte length and add its size to the volume-texture and graphics memory counters
// Leaf: ?InitialiseFromPreallocatedVolumeTexture@CVolumeTexture@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A682E0, length 129 bytes (padded to 144), `ret 4`.
// Callees: CVolumeTexture::Uninitialise @ 0x00A68060, CVolumeTexture::CalcByteLength @ 0x00A67FF0.
// Graphics layer (D3D9, to be replaced): CVolumeTexture vtable 0x0129DC18 = {vptr, IDirect3DVolumeTexture9* PD3DVolumeTexture +0x04,
// ByteLength +0x08, PData +0x0c} (Ego_r.pdb); memory counters g_GraphicsMemoryBytes 0x013CAAB0, g_VolumeTextureMemoryBytes 0x013D28E0
// (next to the landed CVolume counter 0x013D28DC); the inline GetMemorySize = (PData ? GetAllocationSize(PData) @ 0x009F2390 on the
// global allocator 0x013CAA90 : ByteLength) + 4, 0 without a texture.
// Shape: GetMemorySize keeps the size in a local for the null-texture and allocation arms and returns early for the
// ByteLength arm: in the first inline the early return joins (`mov eax,[esi+8] ; add eax,4` falls into the add), in the
// second -- the last statement before `return true` -- VC7.1 duplicates the tail for that arm (`mov esi,[esi+8] ; mov eax,
// [g] ; add esi,4 ; add eax,esi ; mov [g],eax ; mov al,1 ; ret 4`). A single `?:` joins both; all-return forms duplicate both.
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
        if (PD3DVolumeTexture) {
            if (PData)
                size = g_pGlobalAllocator_013caa90->GetAllocationSize(PData) + 4;
            else
                return ByteLength + 4;
        }
        return size;
    }
    void Uninitialise();                           // 0x00A68060
    CVolume GetVolumeLevel(unsigned int level);    // 0x00A67EA0
    void ReleaseTexture();
    void CalcByteLength();
    bool InitialiseFromPreallocatedVolumeTexture(IDirect3DVolumeTexture9* texture);
};
bool CVolumeTexture::InitialiseFromPreallocatedVolumeTexture(IDirect3DVolumeTexture9* texture)
{
    Uninitialise();
    PD3DVolumeTexture = texture;
    CalcByteLength();
    g_VolumeTextureMemoryBytes += GetMemorySize();
    g_GraphicsMemoryBytes += GetMemorySize();
    return true;
}