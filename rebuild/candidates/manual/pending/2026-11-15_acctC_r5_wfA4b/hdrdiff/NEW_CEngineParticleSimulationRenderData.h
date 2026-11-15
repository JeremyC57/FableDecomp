#ifndef FABLE_ENGINE_CENGINEPARTICLESIMULATIONRENDERDATA_H
#define FABLE_ENGINE_CENGINEPARTICLESIMULATIONRENDERDATA_H
// NEW overlay header (wfA4, row 0x00BB2540 CEngineClothRenderer::RenderPolygons). Ego_r.pdb CEngineParticleSimulationRenderData (0x70
// bytes, struct_layouts_egor.tsv): the CParticleSimulationRenderData base (vptr), CVertexBufferWin32 VertexBuffers[2] +0x04, long CurrentVB
// +0x6C. FableWin.pdb ??0CEngineParticleSimulationRenderData@@QAE@XZ (retail 0x00BB63A0, out of line), ??1..@@UAE@XZ, ?IsBusy@..@@UBE_NXZ
// (retail 0x00BB6420); vtable 0x012A56BC. The includer defines FABLE_RETAIL_CPARTICLESIMULATIONRENDERDATA_VIRTUAL_DTOR and
// FABLE_RETAIL_CVERTEXBUFFERWIN32 (typed buffers). Declarations only.
#include "rebuild_abi.h"
#include "engine/CParticleSimulationRenderData.h"
#include "engine/CVertexBufferWin32.h"
class CEngineParticleSimulationRenderData : public CParticleSimulationRenderData {
public:
    CEngineParticleSimulationRenderData();                               // 0x00BB63A0
    virtual ~CEngineParticleSimulationRenderData();
    virtual bool IsBusy() const;                                         // 0x00BB6420
    CVertexBufferWin32 VertexBuffers[2];                                 // +0x04
    long CurrentVB;                                                      // +0x6C
};
FABLE_STATIC_ASSERT(sizeof(CEngineParticleSimulationRenderData) == 0x70);
#endif // FABLE_ENGINE_CENGINEPARTICLESIMULATIONRENDERDATA_H
