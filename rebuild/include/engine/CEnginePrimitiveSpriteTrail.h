#ifndef FABLE_ENGINE_CENGINEPRIMITIVESPRITETRAIL_H
#define FABLE_ENGINE_CENGINEPRIMITIVESPRITETRAIL_H
// New shared header (hand3c3, row 0x00B075D0 CPSCSingleSprite::AddToBoundingSphere): CEnginePrimitiveSpriteTrail, the public sprite
// trail primitive handed to CIEngine::SetChildPrimitive. Layout: Ego_r.pdb CEnginePrimitiveSpriteTrail (struct_layouts_egor.tsv,
// 136 bytes with its 12-byte list) and retail's accesses: the STLport list is one sentinel pointer, so every member after Trail
// sits 8 bytes lower and the object is 0x80 bytes (the landed constructor 0x00B09F90; CPSCSingleSprite's next member PrimIDList at
// +0x12C = +0xAC + 0x80; landed PreparePrimitives 0x00B087C0 stores GraphicIndex +0x18 .. WorldTransform +0x50).
// CSpriteTrailEntry: Ego_r.pdb CEnginePrimitiveSpriteTrail::CSpriteTrailEntry (Position +0, Width +0x0C; landed 0x00B081A0).
// Not polymorphic (CEnginePrimitive has no vptr). The default constructor is out of line in retail (0x00B09F90).
#include "engine/CEnginePrimitive.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CMatrix3x4.h"
#include "stlport/fable_containers.h"
enum EEnginePrimitiveBlendMode;enum EEnginePrimitiveBlendOp;   // MS forward enums, as engine/CEnginePrimitiveParticleSpriteTrailGroup.h
class CEnginePrimitiveSpriteTrail : public CEnginePrimitive {
public:
    class CSpriteTrailEntry {
    public:
        CSpriteTrailEntry(const C3DVector& pos, float width) : Position(pos), Width(width) { }
        C3DVector Position;                             // +0x00
        float     Width;                                // +0x0c
    };
    CEnginePrimitiveSpriteTrail();                      // 0x00B09F90
    std::list<CSpriteTrailEntry> Trail;                 // +0x0c
    bool                      UpdateExisting;           // +0x10
    unsigned long             MaxLength;                // +0x14
    unsigned long             GraphicIndex;             // +0x18
    unsigned long             GraphicFrame;             // +0x1c
    float                     SkipPoints;               // +0x20
    float                     Width;                    // +0x24
    CRGBColour                HeadColour;               // +0x28
    CRGBColour                MidColour;                // +0x2c
    CRGBColour                TailColour;               // +0x30
    float                     FadeStart;                // +0x34
    C3DVector                 OffsetVector;             // +0x38
    unsigned short            Flags;                    // +0x44
    EEnginePrimitiveBlendMode BlendMode;                // +0x48
    EEnginePrimitiveBlendOp   BlendOp;                  // +0x4c
    CMatrix3x4                WorldTransform;           // +0x50
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveSpriteTrail) == 0x80);
#endif
