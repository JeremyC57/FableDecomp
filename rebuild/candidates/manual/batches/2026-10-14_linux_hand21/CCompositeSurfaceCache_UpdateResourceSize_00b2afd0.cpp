// CCompositeSurfaceCache::UpdateResourceSize() -- SetResourceSize(4 + the graphic frame data's size when a frame is attached, else 4)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B2AFD0, length 43 bytes (+ int3 padding).
// Callees: CGraphicFrameData::Sizeof @ 0x009FCBF0 (on the frame data block), CResource::SetResourceSize
// @ 0x009FBF20 (called on both paths: the compiler duplicates the tail, the constant path keeps the
// value in eax). The frame handle is at +0x4c in retail (PDB +0x54: the two CEngineTextureMorph
// members before it are 20 bytes each, not 24), hence the `_Retail` view next to engine/CResource.h.
// The size is the handle's own 4 bytes plus the referenced block.
#include "engine/CResource.h"

struct CGraphicFrameData {
    unsigned long Sizeof();                                     // 0x009FCBF0
};

struct CGraphicFrame_Retail {                                   // engine/CGraphicFrame.h: one pointer
    CGraphicFrameData* FrameData;
};

struct CCompositeSurfaceCache_Retail {
    virtual void v00();
    unsigned char _resource[0x18];                              // +0x04..+0x1b (engine/CResource.h)
    unsigned char _morphs[0x30];                                // +0x1c Owner, +0x20/+0x34 morphs, +0x48 MipmapLevel
    CGraphicFrame_Retail GraphicFrame;                          // +0x4c
    void SetResourceSize(unsigned long size);                   // 0x009FBF20 (CResource)
    void UpdateResourceSize();
};

FABLE_STATIC_ASSERT(sizeof(CResource) == 0x1c);

void CCompositeSurfaceCache_Retail::UpdateResourceSize()
{
    SetResourceSize(GraphicFrame.FrameData != 0 ? GraphicFrame.FrameData->Sizeof() + sizeof(GraphicFrame) : sizeof(GraphicFrame));
}
