#ifndef FABLE_ENGINE_CENGINEINTERNALPRIMITIVEPARTICLEDECALGROUP_H
#define FABLE_ENGINE_CENGINEINTERNALPRIMITIVEPARTICLEDECALGROUP_H
// New header (chunk 2038): Ego_r.pdb CEngineInternalPrimitiveParticleDecalGroup : CEngineInternalPrimitiveParticleGroup (Ego_r own
// members from 0xC0 = sizeof the Ego_r base; retail base 0xAC): DecalBankIndex +0xAC, Flags +0xB0, BlendMode +0xB4, BlendOp +0xB8,
// StencilCubeHeight +0xBC, LandscapeDecal +0xC0 (the landed Update 0x00BB6D70 stores them at these offsets). Needs the typed base
// (FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP).
#include "rebuild_abi.h"
#include "engine/CEngineInternalPrimitiveParticleGroup.h"
enum EEnginePrimitiveBlendMode;
enum EEnginePrimitiveBlendOp;
class CEngineInternalPrimitiveParticleDecalGroup : public CEngineInternalPrimitiveParticleGroup {
public:
    unsigned long             DecalBankIndex;      // +0xac
    unsigned short            Flags;               // +0xb0
    EEnginePrimitiveBlendMode BlendMode;           // +0xb4
    EEnginePrimitiveBlendOp   BlendOp;             // +0xb8
    float                     StencilCubeHeight;   // +0xbc
    bool                      LandscapeDecal;      // +0xc0
};
FABLE_STATIC_ASSERT(sizeof(CEngineInternalPrimitiveParticleDecalGroup) == 0xc4);
#endif // FABLE_ENGINE_CENGINEINTERNALPRIMITIVEPARTICLEDECALGROUP_H
