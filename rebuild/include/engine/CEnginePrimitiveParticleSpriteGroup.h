#ifndef FABLE_ENGINE_CENGINEPRIMITIVEPARTICLESPRITEGROUP_H
#define FABLE_ENGINE_CENGINEPRIMITIVEPARTICLESPRITEGROUP_H
#include "engine/CEnginePrimitive.h"
enum EEnginePrimitiveBlendMode;enum EEnginePrimitiveBlendOp;
// Layout: both original PDB types; native AFCC30 accesses and constructor neighbours.
class CEnginePrimitiveParticleSpriteGroup : public CEnginePrimitive {
public:
    CEnginePrimitiveParticleSpriteGroup(unsigned long bank_index, unsigned short flags, EEnginePrimitiveBlendMode blend_mode,
                                        EEnginePrimitiveBlendOp blend_op)
        : CEnginePrimitive((EEnginePrimitiveType)0x1b,0,(EEngineInterpolationSet)0), BankIndex(bank_index), Flags(flags),
          BlendMode(blend_mode), BlendOp(blend_op) { }
    unsigned long BankIndex;                            // +0x0c
    unsigned short Flags;                               // +0x10
    EEnginePrimitiveBlendMode BlendMode;                // +0x14
    EEnginePrimitiveBlendOp BlendOp;                    // +0x18
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveParticleSpriteGroup)==28);
#endif
