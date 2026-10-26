// CPreallocTexturePool::Allocate() -- find a reusable pooled texture: up to Count times take the least-recently-used resource, mark it most-recently-used, skip it while its load is still pending (unless pending loads may be stolen), and return it once it can be released from its current owner
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A69830, length 125 bytes.
// Callees: CResource::SetAsMRU @ 0x009FC4A0, the load request's still-loading test @ 0x009F9D50 (the manifest's
// CThingFilter_IsUnconscious::operator() is propagated), the owner's virtual slot 10 (+0x28). Global byte @ 0x013D28E8.
// Layout: Ego_r.pdb CPreallocTexturePool: Count +0x3c (the resource-list base below it is not described by the PDB;
// its sentinel node sits at +4 with the LRU end pointer at +0x14). The resource fields (+0x20 load request, +0x30
// owner, +0x48 owned, +0x49 releasing, +0x4a load pending) are descriptive.
// Shape: the head is re-read every pass (the body calls out); the `releasing` flag brackets the owner release and the
// resource is returned when it is no longer owned.
struct CResourceNode {                                      // intrusive list node / sentinel
    unsigned char  _pad00[0x10];
    CResourceNode* m_pNext;                                 // +0x10
};

struct CLoadRequest {
    bool IsStillLoading();                                      // 0x009F9D50
};

struct CTextureOwner {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void ReleaseTexture();                          // slot 10 (+0x28)
};

class CManagedTextureData : public CResourceNode { public:
    unsigned char  _pad14[0x20 - 0x14];
    CLoadRequest*  m_pLoadRequest;                          // +0x20
    unsigned char  _pad24[0x30 - 0x24];
    CTextureOwner* m_pOwner;                                // +0x30
    unsigned char  _pad34[0x48 - 0x34];
    bool           m_bOwned;                                // +0x48
    bool           m_bReleasing;                            // +0x49
    bool           m_bLoadPending;                          // +0x4a
    void SetAsMRU();                                        // 0x009FC4A0 (CResource::SetAsMRU)
};

extern bool g_bStealPendingTextures;                        // 0x013D28E8

struct CPreallocTexturePool {
    void*          vfptr;                                   // +0x00
    CResourceNode  m_Head;                                  // +0x04 sentinel (LRU end at +0x14)
    unsigned char  _pad18[0x3c - 0x18];
    unsigned long  Count;                                   // +0x3c
    unsigned long  MaxSize;                                 // +0x40
    long           PoolIndex;                               // +0x44
    CManagedTextureData* Allocate();
};

CManagedTextureData* CPreallocTexturePool::Allocate()
{
    for (unsigned long i = 0; i < Count; ++i) {
        CManagedTextureData* tex = (CManagedTextureData*)m_Head.m_pNext;
        if (tex != &m_Head && tex) {
            tex->SetAsMRU();
            if (tex->m_bLoadPending) {
                tex->m_bLoadPending = tex->m_pLoadRequest->IsStillLoading();
                if (tex->m_bLoadPending && !g_bStealPendingTextures)
                    continue;
            }
            if (!tex->m_bReleasing) {
                tex->m_bReleasing = true;
                if (tex->m_bOwned)
                    tex->m_pOwner->ReleaseTexture();
                tex->m_bReleasing = false;
                if (!tex->m_bOwned)
                    return tex;
            }
        }
    }
    return 0;
}