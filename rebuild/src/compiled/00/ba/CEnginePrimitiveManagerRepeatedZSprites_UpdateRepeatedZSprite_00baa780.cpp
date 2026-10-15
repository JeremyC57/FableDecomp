// CEnginePrimitiveManagerRepeatedZSprites::UpdateRepeatedZSprite(const CEnginePrimitiveHandle&, const NEnginePrimitiveManagerRepeatedZSprites::CAddPrimitiveDesc&) -- copies the add-description (objects, mesh, fade distances, alpha reference, bounds, max scale) into the handle's internal repeated-Z-sprite primitive and sets its engine flags to 0x300
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BAA780, length 147 bytes (+ int3 padding). No callees.
// Signature from Ego_r.pdb: ?UpdateRepeatedZSprite@CEnginePrimitiveManagerRepeatedZSprites@@QAEXABVCEnginePrimitiveHandle@@ABUCAddPrimitiveDesc@NEnginePrimitiveManagerRepeatedZSprites@@@Z
// (`ret 8`, `this` unused).
// Layout: CEnginePrimitiveHandle = vptr + CIVCountedPointer Data (+4: the internal primitive). CAddPrimitiveDesc (PDB,
// 0x50): BoundingSphere +0, BoundingBox +0x10, MaxScale +0x28, Mesh +0x2c, FadeDist +0x30, FadeInStart +0x38,
// FadeInEnd +0x3c, ObjectEnts +0x40, ObjectCount +0x44, AlphaRef +0x48. CEngineInternalPrimitiveRepeatedZSprite
// (PDB, 0x94): base EnginePrimitiveFlags +0xc, FadeDistance +0x14, AlphaRef (byte) +0x28; Mesh +0x50, FadeInStart
// +0x54, FadeInEnd +0x58, ObjectsEntries +0x5c, ObjectCount +0x60, BoundingSphere +0x64, BoundingBox +0x74,
// MaxScale +0x90.
// Shape: plain statement-order assignments; the 8-byte fade distance copies as two dword moves, the alpha
// reference narrows to a byte, the sphere and box are POD records copied through re-based pointers (`lea` of
// both ends, esi/edi/edx), as implicit struct assignments do in this module.
#include "engine/CEnginePrimitiveManagerRepeatedZSprites.h"

struct C3DVector { float X, Y, Z; };
struct C3DBoundingSphere { C3DVector Center; float Radius; };
struct C3DBoundingBox { C3DVector Min; C3DVector Max; };
struct CEngineFadeDistance { float Near; float Far; };
struct C3DMeshInfo;
struct CObjectEntry;

namespace NEnginePrimitiveManagerRepeatedZSprites {
struct CAddPrimitiveDesc {                               // PDB, 0x50 bytes
    C3DBoundingSphere   BoundingSphere;                  // +0x00
    C3DBoundingBox      BoundingBox;                     // +0x10
    float               MaxScale;                        // +0x28
    C3DMeshInfo*        Mesh;                            // +0x2c
    CEngineFadeDistance FadeDist;                        // +0x30
    float               FadeInStart;                     // +0x38
    float               FadeInEnd;                       // +0x3c
    CObjectEntry*       ObjectEnts;                      // +0x40
    long                ObjectCount;                     // +0x44
    long                AlphaRef;                        // +0x48
    bool                Persistent;                      // +0x4c
};
}

struct CEngineInternalPrimitiveRepeatedZSprite {        // CEngineInternalPrimitiveBase (0x48) + the sprite data (PDB)
    void*               vptr;                            // +0x00
    unsigned char       _pad04[0x8];
    unsigned long       EnginePrimitiveFlags;            // +0x0c
    float               AlphaMipBias;                    // +0x10
    CEngineFadeDistance FadeDistance;                    // +0x14
    unsigned char       _pad1c[0xc];
    unsigned char       AlphaRef;                        // +0x28
    unsigned char       _pad29[0x27];
    C3DMeshInfo*        Mesh;                            // +0x50
    float               FadeInStart;                     // +0x54
    float               FadeInEnd;                       // +0x58
    CObjectEntry*       ObjectsEntries;                  // +0x5c
    long                ObjectCount;                     // +0x60
    C3DBoundingSphere   BoundingSphere;                  // +0x64
    C3DBoundingBox      BoundingBox;                     // +0x74
    float               MinDistance;                     // +0x8c
    float               MaxScale;                        // +0x90
};

struct CEnginePrimitiveHandle {
    void*                                    vptr;       // +0x00
    CEngineInternalPrimitiveRepeatedZSprite* Data;       // +0x04 (CIVCountedPointer object)
};

struct CEnginePrimitiveManagerRepeatedZSprites_Methods : CEnginePrimitiveManagerRepeatedZSprites {
    void UpdateRepeatedZSprite(const CEnginePrimitiveHandle& handle,
                               const NEnginePrimitiveManagerRepeatedZSprites::CAddPrimitiveDesc& desc);
};

void CEnginePrimitiveManagerRepeatedZSprites_Methods::UpdateRepeatedZSprite(
    const CEnginePrimitiveHandle& handle, const NEnginePrimitiveManagerRepeatedZSprites::CAddPrimitiveDesc& desc)
{
    CEngineInternalPrimitiveRepeatedZSprite* sprite = handle.Data;
    sprite->ObjectsEntries = desc.ObjectEnts;
    sprite->ObjectCount = desc.ObjectCount;
    sprite->Mesh = desc.Mesh;
    sprite->FadeDistance = desc.FadeDist;
    sprite->AlphaRef = (unsigned char)desc.AlphaRef;
    sprite->FadeInStart = desc.FadeInStart;
    sprite->FadeInEnd = desc.FadeInEnd;
    sprite->EnginePrimitiveFlags = 0x300;
    sprite->BoundingSphere = desc.BoundingSphere;
    sprite->BoundingBox = desc.BoundingBox;
    sprite->MaxScale = desc.MaxScale;
}