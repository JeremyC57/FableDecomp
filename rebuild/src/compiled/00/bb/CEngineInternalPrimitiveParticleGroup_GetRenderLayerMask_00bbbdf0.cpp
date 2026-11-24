// Leaf: ?GetRenderLayerMask@CEngineInternalPrimitiveParticleGroup@@UBEKXZ
typedef unsigned long ulong;

struct CEngineInternalPrimitiveParticleGroup {
    char pad[0xa0];
    unsigned long type;
    virtual unsigned long GetRenderLayerMask() const;
};

unsigned long CEngineInternalPrimitiveParticleGroup::GetRenderLayerMask() const
{
    CEngineInternalPrimitiveParticleGroup* self = const_cast<CEngineInternalPrimitiveParticleGroup*>(this);
    switch (self->type) {
    case 0:
        return 0x8000;
    case 1:
        return 0x100000;
    default:
        return 0x80000;
    }
}
// Flatten route (flatten_pilot v4, NF324): CEngineInternalPrimitiveParticleGroup_GetRenderLayerMask -> CEngineInternalPrimitiveParticleGroup::GetRenderLayerMask, the RTTI-placed PDB member; class model: the row's local class model.
