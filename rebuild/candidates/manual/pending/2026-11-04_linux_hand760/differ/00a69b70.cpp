// CTextureManager::AllocateTexture(CManagedTexture& texture, unsigned long width, unsigned long height, long mipLevels, const CPixelFormat& format, CManagedTextureOwner* owner) -- pick the prealloc pool for the texture's byte size, take a managed texture from it, create the D3D texture and hand it to `texture`; a pool that cannot serve counts a failed allocation
// Grade: DIFFER(149v176) -- dl=14 on shared headers + Reset: (Reset called out of line, colouring right); __forceinline Reset or the landed sibling 0x00A69C20 model: 176v176 with this/index/tex colouring swapped
// Leaf: ?AllocateTexture@CTextureManager@@QAE_NAAVCManagedTexture@@KKJABVCPixelFormat@@PAVCManagedTextureOwner@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A69B70, 176 bytes, `ret 18h` (Ego_r.pdb / FableWin.pdb
//   ?AllocateTexture@CTextureManager@@QAE_NAAVCManagedTexture@@KKJABVCPixelFormat@@PAVCManagedTextureOwner@@@Z).
// Callees: CTextureManager::CalculateTextureSize 0x00A698F0 (static __fastcall), GetPoolIndex 0x00A69730 (private),
//   CPreallocTexturePool::Allocate 0x00A69830, CManagedTextureData::CreateTexture 0x00A697A0, the released texture's
//   CIVCountedPointeeBase slot 1 Delete.
// Layout: Ego_r.pdb CManagedTexture (8: vptr, CIVCountedPointer<CManagedTextureData> Data +4); CManagedTextureData : CIVCountedPointeeBase
//   (IVRefCount +4), Owner +0x30; CTextureManager PreallocPools +0x4c (retail stride 0x54), FailedAllocations +0x58c.
// Shape: `texture.Data.Reset(data)` (FableWin.pdb ?Reset@?$CIVCountedPointer@..@@QAEXPAV1@@Z, inline): release, store, and the add-ref
//   without a null test (the pointer was tested before CreateTexture).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#include "rebuild_abi.h"
#include "engine/CIVCountedPointeeBase.h"
#include "engine/CIVCountedPointer.h"

class CPixelFormat { public: long Format; };
class CManagedTextureOwner;
class CManagedTextureData : public CIVCountedPointeeBase {   // engine/CManagedTextureData.h is the flat Ego_r layout (header debt)
public:
    bool CreateTexture(unsigned long width, unsigned long height, long mipLevels, const CPixelFormat& format);   // 0x00A697A0
    void SetOwner(CManagedTextureOwner* owner) { Owner = owner; }   // FableWin.pdb ?SetOwner@CManagedTextureData@@QAEXPAVCManagedTextureOwner@@@Z
private:
    unsigned char         _pad08[0x30 - 0x08];
    CManagedTextureOwner* Owner;                        // +0x30
};
class CManagedTexture {
public:
    virtual ~CManagedTexture();
    CIVCountedPointer<CManagedTextureData> Data;        // +0x04
};
class CPreallocTexturePool {
public:
    CManagedTextureData* Allocate();                    // 0x00A69830
private:
    unsigned char _body[0x54];
};
class CTextureManager {
public:
    bool AllocateTexture(CManagedTexture& texture, unsigned long width, unsigned long height, long mipLevels, const CPixelFormat& format,
                         CManagedTextureOwner* owner);
    static unsigned long __fastcall CalculateTextureSize(unsigned long width, unsigned long height, bool mipmaps, const CPixelFormat& format);   // 0x00A698F0
private:
    long GetPoolIndex(unsigned long size);              // 0x00A69730
    void*                __vftable;                     // +0x00
    bool                 Initialised;                   // +0x04
    unsigned long        PoolCount;                     // +0x08
    unsigned long        PreallocSizes[16];             // +0x0c
    CPreallocTexturePool PreallocPools[16];             // +0x4c
    unsigned long        FailedAllocations[16];         // +0x58c
};

bool CTextureManager::AllocateTexture(CManagedTexture& texture, unsigned long width, unsigned long height, long mipLevels,
                                      const CPixelFormat& format, CManagedTextureOwner* owner)
{
    long index = GetPoolIndex(CalculateTextureSize(width, height, true, format));
    if (index == -1)
        return false;
    CManagedTextureData* data = PreallocPools[index].Allocate();
    if (data && data->CreateTexture(width, height, mipLevels, format)) {
        data->SetOwner(owner);
        texture.Data.Reset(data);
        return true;
    }
    ++FailedAllocations[index];
    return false;
}
