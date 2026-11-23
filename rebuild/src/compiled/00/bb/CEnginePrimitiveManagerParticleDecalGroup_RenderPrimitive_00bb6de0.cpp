// CEnginePrimitiveManagerParticleDecalGroup_Methods::RenderPrimitive @ 0x00bb6de0
// xor eax,eax; ret 0xc  -> returns ERendered(0); 3 stack args (thiscall)
// Leaf: ?RenderPrimitive@CEnginePrimitiveManagerParticleDecalGroup@@UAE?AW4ERendered@@AAVCEngineInternalPrimitiveBase@@W4EEngineRenderLayer@@AAVCEngineSubPrimitiveChain@@@Z
// Signature port (NF320, 2026-11-23): defined on the real class of engine/CEnginePrimitiveManagerParticleDecalGroup.h under FABLE_RETAIL_CENGINEPRIMITIVEMANAGERPARTICLEDECALGROUP, FABLE_RETAIL_CENGINEPRIMITIVEMANAGERPARTICLEDECALGROUP_OVERRIDES with the PDB parameter / return types (was ?RenderPrimitive@CEnginePrimitiveManagerParticleDecalGroup_Methods@@QAEHPAVCEngineInternalPrimitiveBase@@HPAVCEngineSubPrimitiveChain@@@Z);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member here.
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERPARTICLEDECALGROUP
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERPARTICLEDECALGROUP_OVERRIDES
#include "engine/CEnginePrimitiveManagerParticleDecalGroup.h"
#include "rebuild_abi.h"
class CEngineInternalPrimitiveBase; class CEngineSubPrimitiveChain;
ERendered CEnginePrimitiveManagerParticleDecalGroup::RenderPrimitive(CEngineInternalPrimitiveBase& primitive, EEngineRenderLayer layer, CEngineSubPrimitiveChain& chain) {
    return (ERendered)0;
}
