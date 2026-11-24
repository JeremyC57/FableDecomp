// Leaf: ?IsBusy@CParticleSimulationRenderData@@UBE_NXZ

#define FABLE_RETAIL_CPARTICLESIMULATIONRENDERDATA_VIRTUAL_MODEL
#include "engine/CParticleSimulationRenderData.h"  // retyped onto the PDB layout; byte parity re-verified
bool CParticleSimulationRenderData::IsBusy() const
{
    return false;
}
// Flatten route (flatten_pilot v5, NF330): CParticleSimulationRenderData_IsBusy -> CParticleSimulationRenderData::IsBusy, the RTTI-placed PDB member; class model: engine/CParticleSimulationRenderData.h opt-in FABLE_RETAIL_CPARTICLESIMULATIONRENDERDATA_VIRTUAL_MODEL.
