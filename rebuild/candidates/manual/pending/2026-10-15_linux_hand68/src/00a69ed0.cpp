// CPreallocTexturePool::GetTexturesInUse(unsigned long) const -- count the pool's allocated textures that were used within the given number of frames
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A69ED0, length 61 bytes.
// Callees: CResource::GetUnusedFrames @ 0x009FBF70 (landed). Ego_r.pdb ?GetTexturesInUse@CPreallocTexturePool@@QBEKK@Z.
// Layout (Ego_r.pdb, retail CArray 12 bytes): Count +0x3c, Textures (CArray<CIVCountedPointer<
// CManagedTextureData> >, one word per entry) +0x48; CManagedTextureData::Allocated +0x48.
// Shape: `if (Textures[i]->Allocated && Textures[i]->GetUnusedFrames() <= maxUnusedFrames) ++inUse;` with the
// limit in ebp, Count re-read at the bottom, unsigned `ja`.
#include "engine/CManagedTextureData.h"                    // Allocated at +0x48 (PDB = retail here)

struct CManagedTextureData_Calls : CManagedTextureData {
    unsigned long GetUnusedFrames() const;                  // CResource::GetUnusedFrames 0x009FBF70
};

template <class T> struct CIVCountedPointer {               // intrusive counted pointer: one word
    T* Data;
    T* operator->() const { return Data; }
};

template <class T> struct CArray {                          // retail CArray: three pointers (12 bytes)
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    const T& operator[](unsigned long i) const { return m_pFirst[i]; }
};

struct CPreallocTexturePool {
    void*                                        __vftable; // +0x00
    unsigned char                                _pad04[0x38];
    unsigned long                                Count;     // +0x3c
    unsigned long                                MaxSize;   // +0x40
    long                                         PoolIndex; // +0x44
    CArray<CIVCountedPointer<CManagedTextureData_Calls> > Textures; // +0x48
    unsigned long GetTexturesInUse(unsigned long maxUnusedFrames) const;
};

unsigned long CPreallocTexturePool::GetTexturesInUse(unsigned long maxUnusedFrames) const
{
    unsigned long inUse = 0;
    for (unsigned long i = 0; i < Count; ++i) {
        if (Textures[i]->Allocated && Textures[i]->GetUnusedFrames() <= maxUnusedFrames)
            ++inUse;
    }
    return inUse;
}
