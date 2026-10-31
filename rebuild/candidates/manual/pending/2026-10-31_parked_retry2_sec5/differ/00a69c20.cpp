// CTextureManager::AllocateVolumeTexture(CIVCountedPointer<CManagedTextureData>& out, unsigned long width, unsigned long height, unsigned long depth, long mipLevels, const CPixelFormat& format, CTextureOwner* owner) -- the volume twin of AllocateTexture: the slice size times the depth picks the pool, the managed texture creates the D3D volume texture, the owner is recorded and the texture handed back through the counted pointer; failures bump the pool's failure count
// Leaf: AllocateVolumeTexture
// Grade: DIFFER(180v186) -- true extent 186 (catalog 381 swallowed an uncatalogued function at 0x00A69CE0; BOUNDS_FIX +
//   oracle_extra); pool size named before GetPoolIndex (4 diff lines v 43 at the true extent); left: retail add-ref is
//   mov esi,ebx / load / inc / store interleaved with the pops, ours inc dword ptr [ebx+4] (same gap as AllocateTexture
//   0x00A69B70)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A69C20, 186 bytes. `ret 1Ch`.
// Callees: CalculateTextureSize(unsigned long, unsigned long, bool, const CPixelFormat&) 0x00A698F0 (`__fastcall`, flag false),
//          CTextureManager::GetPoolIndex(unsigned long) 0x00A69730, CPreallocTexturePool::Allocate() 0x00A69830,
//          CManagedTextureData::CreateVolumeTexture(ulong, ulong, ulong, long, const CPixelFormat&) 0x00A699D0, the released
//          texture's slot 1 (CIVCountedPointeeBase::Delete).
// Shape: as AllocateTexture (0x00A69B70): the slice size is multiplied by the depth argument straight from its slot
//        (`imul eax,[esp+1Ch]`), the format stays in ebp, the counted pointer assignment is the inline release/assign/add-ref
//        with the add-ref on a local copy of the texture.
// Layout: CTextureManager retail: PreallocPools +0x4c (stride 0x54), FailedAllocations +0x58c; CManagedTextureData:
//         IVRefCount +0x04, Delete slot 1, m_pOwner +0x30.
struct CPixelFormat { long m_format; };
unsigned long __fastcall CalculateTextureSize(unsigned long width, unsigned long height, bool volume, const CPixelFormat& format);   // 0x00A698F0
struct CTextureOwner;
struct CIVCountedPointeeBase {
    virtual void v00();
    virtual void Delete();                      // slot 1 (+0x04)
    long IVRefCount;                            // +0x04
    void Release() { if (--IVRefCount == 0) Delete(); }
};
struct CManagedTextureData : CIVCountedPointeeBase {
    unsigned char  _pad08[0x30 - 0x08];
    CTextureOwner* m_pOwner;                    // +0x30
    bool CreateVolumeTexture(unsigned long width, unsigned long height, unsigned long depth, long mipLevels, const CPixelFormat& format);   // 0x00A699D0
};
struct CIVCountedPointer {                      // CIVCountedPointer<CManagedTextureData>: vptr +0x00, Object +0x04
    virtual ~CIVCountedPointer() { }
    CManagedTextureData* Object;
    __forceinline void Set(CManagedTextureData* tex)
    {
        CManagedTextureData* old = Object;
        if (old) {
            old->Release();
            Object = 0;
        }
        Object = tex;
        CIVCountedPointeeBase* base = Object;
        base->IVRefCount = base->IVRefCount + 1;
    }
};
struct CPreallocTexturePool {
    unsigned char _body[0x54];
    CManagedTextureData* Allocate();            // 0x00A69830
};
struct CTextureManager {
    void*                __vftable;             // +0x00
    bool                 Initialised;           // +0x04
    unsigned long        PoolCount;             // +0x08
    unsigned long        PreallocSizes[16];     // +0x0c
    CPreallocTexturePool PreallocPools[16];     // +0x4c (retail stride 0x54)
    unsigned long        FailedAllocations[16]; // +0x58c
    long GetPoolIndex(unsigned long size);      // 0x00A69730
    bool AllocateVolumeTexture(CIVCountedPointer& out, unsigned long width, unsigned long height, unsigned long depth, long mipLevels, const CPixelFormat& format, CTextureOwner* owner);
};
bool CTextureManager::AllocateVolumeTexture(CIVCountedPointer& out, unsigned long width, unsigned long height, unsigned long depth, long mipLevels, const CPixelFormat& format, CTextureOwner* owner)
{
    unsigned long size = CalculateTextureSize(width, height, false, format) * depth;
    long index = GetPoolIndex(size);
    if (index == -1)
        return false;
    CManagedTextureData* tex = PreallocPools[index].Allocate();
    if (tex && tex->CreateVolumeTexture(width, height, depth, mipLevels, format)) {
        tex->m_pOwner = owner;
        out.Set(tex);
        return true;
    }
    ++FailedAllocations[index];
    return false;
}
