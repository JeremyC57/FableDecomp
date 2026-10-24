// CTextureManager::ResizePoolTextureCount(unsigned long textureSize, unsigned long count) -- find the preallocated pool for a texture size; when it exists, end the frame's pool bookkeeping and resize that pool to count textures, returning the pool's result (false when no pool matches)
// Leaf: ?ResizePoolTextureCount@CTextureManager@@QAE_NKK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6ABE0, 62 bytes, `ret 8`.
// Callees: CTextureManager::GetPoolIndex(unsigned long) @ 0x00A69730, CTextureManager::FrameEnd() @ 0x00A698B0,
//   CPreallocTexturePool::ResizePool(unsigned long) @ 0x00A6A670.
// Label: the manifest's `TextureManager_ResizePoolByFormat` is descriptive; FableWin.pdb names
//   `?ResizePoolTextureCount@CTextureManager@@QAE_NKK@Z`.
// Layout (Ego_r.pdb; the pool's 16-byte CArray is a 12-byte STLport vector in retail, so pools are 0x54 apart):
//   vptr +0, Initialised +4, PoolCount +8, PreallocSizes[16] +0xC, PreallocPools[16] +0x4C.
// Shape: `bool result = false` lives in bl for the miss path; the hit path returns ResizePool's al directly.
class CPreallocTexturePool {                                 // 0x54 bytes in retail
public:
    unsigned char Data[0x54];
    bool ResizePool(unsigned long count);                    // 0x00A6A670
};

class CTextureManager {
public:
    virtual ~CTextureManager();
    bool                 Initialised;                        // +0x04
    unsigned long        PoolCount;                          // +0x08
    unsigned long        PreallocSizes[16];                  // +0x0c
    CPreallocTexturePool PreallocPools[16];                  // +0x4c
    void FrameEnd();                                         // 0x00A698B0
    bool ResizePoolTextureCount(unsigned long textureSize, unsigned long count);
private:
    long GetPoolIndex(unsigned long textureSize);            // 0x00A69730
};

bool CTextureManager::ResizePoolTextureCount(unsigned long textureSize, unsigned long count)
{
    bool result = false;
    unsigned long index = GetPoolIndex(textureSize);
    if (index < PoolCount) {
        FrameEnd();
        result = PreallocPools[index].ResizePool(count);
    }
    return result;
}