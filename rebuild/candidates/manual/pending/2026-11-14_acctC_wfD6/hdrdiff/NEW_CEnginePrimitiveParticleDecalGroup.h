#ifndef FABLE_ENGINE_CENGINEPRIMITIVEPARTICLEDECALGROUP_H
#define FABLE_ENGINE_CENGINEPRIMITIVEPARTICLEDECALGROUP_H
#include "engine/CEnginePrimitive.h"
enum EEnginePrimitiveBlendMode;enum EEnginePrimitiveBlendOp;
// New shared header (wfD6, row 0x00B1F200): Ego_r.pdb CEnginePrimitiveParticleDecalGroup (36 bytes): DecalBankIndex +0x0c, Flags +0x10
// (ushort), BlendMode +0x14, BlendOp +0x18, StencilCubeHeight +0x1c, LandscapeDecal +0x20. Constructor FableWin.pdb
// ??0CEnginePrimitiveParticleDecalGroup@@QAE@J_NW4EEnginePrimitiveBlendMode@@W4EEnginePrimitiveBlendOp@@MK@Z (inline in retail:
// 0x00B1F6A9.. stores type 0x1E, interpolation set 0, the channel bytes, then the members in place).
// Needs FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR (the public base constructor, as CEnginePrimitiveParticleSpriteGroup).
class CEnginePrimitiveParticleDecalGroup : public CEnginePrimitive {
public:
    CEnginePrimitiveParticleDecalGroup(long decal_bank_index, bool landscape_decal, EEnginePrimitiveBlendMode blend_mode,
                                       EEnginePrimitiveBlendOp blend_op, float stencil_cube_height, unsigned long flags)
        : CEnginePrimitive((EEnginePrimitiveType)0x1e,0,(EEngineInterpolationSet)0), DecalBankIndex(decal_bank_index),
          Flags((unsigned short)flags), BlendMode(blend_mode), BlendOp(blend_op), StencilCubeHeight(stencil_cube_height),
          LandscapeDecal(landscape_decal) { }
    unsigned long DecalBankIndex;                       // +0x0c
    unsigned short Flags;                               // +0x10
    EEnginePrimitiveBlendMode BlendMode;                // +0x14
    EEnginePrimitiveBlendOp BlendOp;                    // +0x18
    float StencilCubeHeight;                            // +0x1c
    bool LandscapeDecal;                                // +0x20
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveParticleDecalGroup)==36);
#endif
