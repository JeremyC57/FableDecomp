// C3DClothPrimitive::RemapBoneIndices(map<unsigned long, unsigned long>& remap) -- remap the cloth primitive's bones: forward to its particle
//   program's RemapBoneIndices (CParticleProgram::RemapBoneIndices 0x00AE3A30 on ParticleProgram +0x04)
// Leaf: ?RemapBoneIndices@C3DClothPrimitive@@QAEXAAV?$map@KKU?$less@K@std@@V?$allocator@U?$pair@$$CBKK@std@@@2@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC3AF0, `mov ecx,[ecx+4] ; jmp 0x00AE3A30` (the remap stays on the stack).
// Identity: Ego_r.pdb 0x00C4B610 / FableWin.pdb 0x03496150 ?RemapBoneIndices@C3DClothPrimitive@@QAEXAAV?$map@KK...@Z; called by
//   C3DPrimitive2::RemapBoneIndices 0x00A90F00 for each cloth primitive. Layout: engine/C3DClothPrimitive.h (CCountedPointer<CParticleProgram>
//   ParticleProgram +0x04, whose Data is the first word).
// NF70 (round 7, land1117fu q10799): was the placeholder `T::Run` forwarder (?Run@T@@QAEXXZ) over a local `Sub` stand-in; now the PDB member on
//   the shared headers (overlay opt-ins FABLE_RETAIL_C3DCLOTHPRIMITIVE_REMAPBONEINDICES, FABLE_RETAIL_CPARTICLEPROGRAM_REMAPBONEINDICES_PUBLIC).
#define FABLE_RETAIL_CPARTICLEPROGRAM
#define FABLE_RETAIL_CPARTICLEPROGRAM_REMAPBONEINDICES_PUBLIC
#define FABLE_RETAIL_C3DCLOTHPRIMITIVE_REMAPBONEINDICES
#include "stlport/fable_map.h"
#include "engine/CParticleProgram.h"
#include "engine/C3DClothPrimitive.h"

void C3DClothPrimitive::RemapBoneIndices(std::map<unsigned long, unsigned long>& remap)
{
    ParticleProgram->RemapBoneIndices(remap);
}
