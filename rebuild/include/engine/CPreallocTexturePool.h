#ifndef FABLE_ENGINE_CPREALLOCTEXTUREPOOL_H
#define FABLE_ENGINE_CPREALLOCTEXTUREPOOL_H
// The retail pool derives from CResourceList and occupies 0x54 bytes.
// Ego_r records the same member offsets and a 16-byte vector instead of retail's 12.
#include "engine/CResourceList.h"
#include "engine/CManagedTextureData.h"
#include "engine/CIVCountedPointer.h"
#include "engine/CArray.h"

class CPreallocTexturePool : public CResourceList {
public:
    CPreallocTexturePool();                                             // 0x00A6A330
    virtual ~CPreallocTexturePool();
    CManagedTextureData* Allocate();                                    // 0x00A69830
    bool ResizePool(unsigned long new_size);
    unsigned long GetTexturesAllocated() const
    {
        unsigned long allocated = 0;
        for (unsigned long c1 = 0; c1 < Count; ++c1) {
            if (PeekTextureData(c1).IsAllocated())
                ++allocated;
        }
        return allocated;
    }

private:
    CManagedTextureData& GetTextureData(unsigned long index) { return *Textures[index]; }
    const CManagedTextureData& PeekTextureData(unsigned long index) const { return *Textures[index]; }
    unsigned long Count;
    unsigned long MaxSize;
    long PoolIndex;
    CArray<CIVCountedPointer<CManagedTextureData> > Textures;
};
FABLE_STATIC_ASSERT(sizeof(CPreallocTexturePool) == 0x54);
#endif
