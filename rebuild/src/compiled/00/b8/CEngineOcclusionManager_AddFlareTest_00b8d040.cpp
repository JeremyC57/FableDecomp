// CEngineOcclusionManager::AddFlareTest(unsigned long handle, const C3DVector& position, float radius, long segments) -- an empty PC stub
//   (`ret 10h`: four stack arguments).
// Leaf: ?AddFlareTest@CEngineOcclusionManager@@QAEXKABVC3DVector@@MJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B8D040, length 3 bytes (`ret 10h`).
// Identity (NF56, round 7, queue q10416): Ego_r.pdb 0x00A34750 / FableWin.pdb 0x02DC5CD0 ?AddFlareTest@CEngineOcclusionManager@@QAEXKABVC3DVector@@MJ@Z,
//   the four-argument void overload: `ret 10h` pops four dwords, while the other overload (Ego_r 0x00A34760, ...QAEKABVC3DVector@@MJ@Z)
//   takes three and returns K; the Ego_r -> retail delta 0x1588F0 matches GetResult 0x00B8D0B0 and Start/EndOcclusionTest 0x00B8D090 /
//   0x00B8D0A0; the landed caller CEngineSkyRenderer::Render 0x00B662F0 (four pushes) binds this spelling through engine/CEngineOcclusionManager.h
//   FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_ADDFLARETEST. Was the placeholder ?M@C00b8d040@@QAEXHHHH@Z. Parameter names are descriptive.
#include "fable_compiler.h"
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_SCOPED_FORCEINLINE
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_ADDFLARETEST
#include "engine/CRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/C3DVector.h"
#include "engine/CEngineOcclusionManager.h"

void CEngineOcclusionManager::AddFlareTest(unsigned long /*handle*/, const C3DVector& /*position*/, float /*radius*/, long /*segments*/)
{
}
