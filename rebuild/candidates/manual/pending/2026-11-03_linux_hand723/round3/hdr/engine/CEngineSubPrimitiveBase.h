#ifndef FABLE_ENGINE_CENGINESUBPRIMITIVEBASE_H
#define FABLE_ENGINE_CENGINESUBPRIMITIVEBASE_H
// CEngineSubPrimitiveBase (chunk 723c): Ego_r.pdb class (V-keyed: ?RenderSingleSprite@CEnginePrimitiveManagerParticleSpriteGroup@@QAEX
// AAVCEngineSubPrimitiveBase@@W4ERenderPassType@@@Z), 0x18 bytes with a vptr at +0 (Ego_r _padding_), no container (retail = Ego_r).
// RenderSingleSprite 0x00BC04F0 reads Parent at +0x0C.
#include "rebuild_abi.h"
class CEngineInternalPrimitiveBase;
class CEnginePrimitiveManager;
class CEngineSubPrimitiveBase {
public:
    void*                         __vftable;                             // +0x00 (virtual interface not modelled)
    unsigned long                 RenderFlags;                           // +0x04
    unsigned char                 Alpha;                                 // +0x08
    CEngineInternalPrimitiveBase* Parent;                                // +0x0c
    CEngineSubPrimitiveBase*      NextSubPrimitive;                      // +0x10
    CEnginePrimitiveManager*      Manager;                               // +0x14
};
FABLE_STATIC_ASSERT(sizeof(CEngineSubPrimitiveBase) == 0x18);
#endif // FABLE_ENGINE_CENGINESUBPRIMITIVEBASE_H
