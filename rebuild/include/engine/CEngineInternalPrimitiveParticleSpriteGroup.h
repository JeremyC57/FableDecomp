#ifndef FABLE_ENGINE_CENGINEINTERNALPRIMITIVEPARTICLESPRITEGROUP_H
#define FABLE_ENGINE_CENGINEINTERNALPRIMITIVEPARTICLESPRITEGROUP_H
// New overlay header (row 0x00BBF4A0 CEnginePrimitiveManagerParticleSpriteGroup::RenderSubPrimitive): CEngineInternalPrimitiveParticleSpriteGroup
// : CEngineInternalPrimitiveParticleGroup (the typed particle-group opt-in, sizeof 0xAC); own members with their Ego_r.pdb names in Ego_r order
// at Ego_r minus 0x14 (the base's STLport containers): SpriteBankIndex +0xAC (0x00BBF4CC), Flags +0xB0 (0x00BBF752 / 0x00BBFEC5), BlendMode +0xB4
// (0x00BBFAAC), BlendOp +0xB8 (0x00BBFA1D); sizeof 0xBC.
#ifndef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP
#endif
#include "rebuild_abi.h"
#include "engine/CEngineInternalPrimitiveParticleGroup.h"
#include "engine/CEngineRenderEnums.h"
class CEngineInternalPrimitiveParticleSpriteGroup : public CEngineInternalPrimitiveParticleGroup {
public:
    unsigned long             SpriteBankIndex;                           // +0xac
    unsigned short            Flags;                                     // +0xb0
    EEnginePrimitiveBlendMode BlendMode;                                 // +0xb4
    EEnginePrimitiveBlendOp   BlendOp;                                   // +0xb8
};
FABLE_STATIC_ASSERT(sizeof(CEngineInternalPrimitiveParticleSpriteGroup) == 0xbc);
#endif
