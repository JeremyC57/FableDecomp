// CTextureManager::AllocateTexture(CIVCountedPointer<CManagedTextureData>& out, unsigned long width, unsigned long height, long mipLevels, const CPixelFormat& format, CTextureOwner* owner) -- pick the prealloc pool for the texture's byte size, take a managed texture from it, create the D3D texture of the requested shape, record the owner and hand the texture back through the counted pointer; a failed pool or creation bumps that pool's failure count
// Grade: DIFFER(ours v retail) -- 170v176: callee-saved colouring (retail this=esi/index=edi/tex=ebx, ours edi/ebx/esi) and the add-ref folded to `inc [esi+4]` where retail keeps `mov esi,ebx ; mov eax,[esi+4] ; inc eax ; mov [esi+4],eax`
// Leaf: AllocateTexture
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A69B70, 176 bytes. `ret 18h`.
// Callees: CalculateTextureSize(unsigned long, unsigned long, bool, const CPixelFormat&) 0x00A698F0 (`__fastcall`: the extents in
//          ecx/edx, the flag and format on the stack), CTextureManager::GetPoolIndex(unsigned long) 0x00A69730,
//          CPreallocTexturePool::Allocate() 0x00A69830, CManagedTextureData::CreateTexture(ulong, ulong, long, const CPixelFormat&)
//          0x00A697A0, the released texture's slot 1 (CIVCountedPointeeBase::Delete).
// Shape: the height is loaded before the prologue pushes and the format kept in ebp for both calls; `index == -1` is the first
//        early exit (bool in al); the pool is addressed as `index * 0x54 + 0x4c`; the counted pointer assignment is the inline
//        `if (Object) { if (--IVRefCount == 0) Delete(); Object = 0; } Object = tex;` followed by the add-ref written as
//        `IVRefCount = IVRefCount + 1` on a local copy of the texture (esi); the failure exits share the pool counter bump.
// Layout: CTextureManager (engine header has PDB offsets; retail pools are 0x54 bytes): PreallocPools +0x4c (stride 0x54),
//         FailedAllocations +0x58c; CManagedTextureData: IVRefCount +0x04, Delete slot 1, m_pOwner +0x30.
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
    bool CreateTexture(unsigned long width, unsigned long height, long mipLevels, const CPixelFormat& format);   // 0x00A697A0
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
    bool AllocateTexture(CIVCountedPointer& out, unsigned long width, unsigned long height, long mipLevels, const CPixelFormat& format, CTextureOwner* owner);
};
bool CTextureManager::AllocateTexture(CIVCountedPointer& out, unsigned long width, unsigned long height, long mipLevels, const CPixelFormat& format, CTextureOwner* owner)
{
    long index = GetPoolIndex(CalculateTextureSize(width, height, true, format));
    if (index == -1)
        return false;
    CManagedTextureData* tex = PreallocPools[index].Allocate();
    if (tex && tex->CreateTexture(width, height, mipLevels, format)) {
        tex->m_pOwner = owner;
        out.Set(tex);
        return true;
    }
    ++FailedAllocations[index];
    return false;
}
