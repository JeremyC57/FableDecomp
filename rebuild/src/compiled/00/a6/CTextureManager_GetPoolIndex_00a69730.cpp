// CTextureManager::GetPoolIndex(unsigned long size) const -- binary-search the sorted preallocation sizes: exact hit, else the first larger pool, else -1
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A69730, 99 bytes (+13 int3). __fastcall, `ret 4`. RENDER LAYER (texture pools).
// Shape: an inline lower_bound over PreallocSizes[0..PoolCount) (`half = len / 2` signed: `cdq ; sub eax,edx ; sar eax,1`;
//        larger -> `first = mid + 1 ; len += -1 - half`), returning mid on an exact match; PoolCount is copied into a named
//        local (kept in the frame) for the final unsigned `first < count` test (`jae` -> -1).
// Layout: retail CTextureManager (see the landed Uninitialise 0x00A696D0): PoolCount +0x08, PreallocSizes[16] +0x0C.
struct CTextureManager {
    void*         __vftable;                     // +0x00
    bool          Initialised;                   // +0x04
    unsigned long PoolCount;                     // +0x08
    unsigned long PreallocSizes[16];             // +0x0c
    long GetPoolIndex(unsigned long size) const;
};
long CTextureManager::GetPoolIndex(unsigned long size) const
{
    unsigned long count = PoolCount;
    long first = 0;
    long len = count;
    while (len > 0) {
        long half = len / 2;
        long mid = first + half;
        if (size == PreallocSizes[mid])
            return mid;
        if (size > PreallocSizes[mid]) {
            first = mid + 1;
            len = len - half - 1;
        } else {
            len = half;
        }
    }
    if ((unsigned long)first < count)
        return first;
    return -1;
}