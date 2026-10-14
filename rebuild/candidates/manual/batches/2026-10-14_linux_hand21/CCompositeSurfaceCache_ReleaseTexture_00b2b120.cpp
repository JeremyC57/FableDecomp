// CCompositeSurfaceCache::ReleaseTexture() -- when the frame's smaller dimension exceeds the owner's minimum surface size, drop one mipmap level through the owner's texture manager and refresh the resource size
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B2B120, length 94 bytes (+ int3 padding).
// Callees: CTextureManager::ReduceMipmapLevel(CGraphicFrame&, CManagedTexture&, ulong) @ 0x00A69F70
// (the texture manager embedded at +0x9d4 of the owner; result dropped), then the inlined
// UpdateResourceSize (CGraphicFrameData::Sizeof @ 0x009FCBF0, CResource::SetResourceSize @ 0x009FBF20,
// landed shape 0x00B2AFD0). Retail offsets: Owner +0x1c, GraphicFrame +0x4c, ManagedTexture +0x50
// (PDB +0x54/+0x58: the two 20-byte morphs before them), the frame data's Width/Height ushorts at
// +0x14/+0x16, the owner's minimum size at +0x10cc (engine/CEngineSurfaceCompositionManager.h places
// its texture manager at +0xb54, so the owner is modelled by its retail offsets). The smaller
// dimension is the Dinkumware `_Right < _Left ? _Right : _Left` minimum written inline; the
// comparison with the owner's limit is unsigned (`jbe`), and the frame handle's address is hoisted
// because it is passed by reference and re-read by the inlined size update.
#include "engine/CResource.h"

struct CGraphicFrameData {
    unsigned char  _pad[0x14];
    unsigned short Width;                                       // +0x14
    unsigned short Height;                                      // +0x16
    unsigned long Sizeof();                                     // 0x009FCBF0
};

struct CGraphicFrame_Retail {                                   // engine/CGraphicFrame.h: one pointer
    CGraphicFrameData* FrameData;
};

struct CManagedTexture_Retail {                                 // {vptr, Data}
    void* __vftable;
    void* Data;
};

struct CTextureManager_Calls {
    bool ReduceMipmapLevel(CGraphicFrame_Retail& frame, CManagedTexture_Retail& texture, unsigned long levels);   // 0x00A69F70
};

struct CEngineSurfaceCompositionManager_Retail {
    unsigned char         _pad0[0x9d4];
    CTextureManager_Calls TextureManager;                       // +0x9d4
    unsigned char         _pad1[0x10cc - 0x9d4 - 1];
    unsigned long         MinSurfaceSize;                       // +0x10cc (name inferred)
};

struct CCompositeSurfaceCache_Retail {
    virtual void v00();
    unsigned char _resource[0x18];                              // +0x04..+0x1b (engine/CResource.h)
    CEngineSurfaceCompositionManager_Retail* Owner;             // +0x1c
    unsigned char _morphs[0x2c];                                // +0x20/+0x34 morphs, +0x48 MipmapLevel
    CGraphicFrame_Retail   GraphicFrame;                        // +0x4c
    CManagedTexture_Retail ManagedTexture;                      // +0x50
    void SetResourceSize(unsigned long size);                   // 0x009FBF20 (CResource)
    void UpdateResourceSize()                                   // 0x00B2AFD0, inlined here
    {
        SetResourceSize(GraphicFrame.FrameData != 0 ? GraphicFrame.FrameData->Sizeof() + sizeof(GraphicFrame) : sizeof(GraphicFrame));
    }
    void ReleaseTexture();
};

FABLE_STATIC_ASSERT(sizeof(CResource) == 0x1c);

void CCompositeSurfaceCache_Retail::ReleaseTexture()
{
    CGraphicFrameData* data = GraphicFrame.FrameData;
    unsigned long width = data->Width;
    unsigned long height = data->Height;
    unsigned long smaller = width < height ? width : height;      // Dinkumware min(height, width): `_Right < _Left ? _Right : _Left`
    if (smaller > Owner->MinSurfaceSize) {
        Owner->TextureManager.ReduceMipmapLevel(GraphicFrame, ManagedTexture, 1);
        UpdateResourceSize();
    }
}
