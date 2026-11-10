// Behaviour test for 0x00B84470 CEngineSubPrimitiveRenderer::CEngineSubPrimitiveRenderer() (the list setup).
#include <stdio.h>
#include <string.h>
#include <math.h>
enum ERenderPassType { RENDER_PASS_NULL, RENDER_PASS_COMBINED, RENDER_PASS_Z_WRITE, RENDER_PASS_GLOW_BUFFER };
enum ESubPrimitiveType { SUBPRIM_OPAQUE, SUBPRIM_ALPHA, SUBPRIM_GLOW_BUFFER, SUBPRIM_BOUNDING_VOLUME, SUBPRIM_2D, SUBPRIM_COUNT };
struct CSubPrimitiveList {
    ERenderPassType RenderPass, PreRenderPass; bool BoundingVolumePass, RequiresStableSort, Sorted, Enable;
    CSubPrimitiveList() : RenderPass(RENDER_PASS_NULL), PreRenderPass(RENDER_PASS_NULL), BoundingVolumePass(false), RequiresStableSort(false), Sorted(true), Enable(true) {}
};
static unsigned long ScratchTable[16];
class CEngineSubPrimitiveRenderer { public:
    CSubPrimitiveList SubPrimitiveLists[SUBPRIM_COUNT]; float MaxSortDistance, SortDistanceMultiplier;
    void SetMaxSortDistance(float distance) { MaxSortDistance = distance; SortDistanceMultiplier = 1073741824.0f / distance; }
    CSubPrimitiveList& GetList(ESubPrimitiveType type) { return SubPrimitiveLists[type]; }
    CEngineSubPrimitiveRenderer()
    {
        SetMaxSortDistance(10000.0f);
        memset(ScratchTable, 0, sizeof(ScratchTable));
        GetList(SUBPRIM_OPAQUE).RenderPass = RENDER_PASS_COMBINED;
        GetList(SUBPRIM_ALPHA).RenderPass = RENDER_PASS_COMBINED;
        GetList(SUBPRIM_ALPHA).PreRenderPass = RENDER_PASS_Z_WRITE;
        GetList(SUBPRIM_GLOW_BUFFER).RenderPass = RENDER_PASS_GLOW_BUFFER;
        GetList(SUBPRIM_BOUNDING_VOLUME).BoundingVolumePass = true;
        GetList(SUBPRIM_2D).RenderPass = RENDER_PASS_COMBINED;
        GetList(SUBPRIM_2D).RequiresStableSort = true;
    }
};
int main()
{
    ScratchTable[3] = 7;
    CEngineSubPrimitiveRenderer r;
    int ok = ScratchTable[3] == 0 && fabs(r.MaxSortDistance - 10000.0f) < 1e-3 && fabs(r.SortDistanceMultiplier - 107374.1824f) < 0.01
          && r.SubPrimitiveLists[SUBPRIM_ALPHA].PreRenderPass == RENDER_PASS_Z_WRITE && r.SubPrimitiveLists[SUBPRIM_OPAQUE].PreRenderPass == RENDER_PASS_NULL
          && r.SubPrimitiveLists[SUBPRIM_GLOW_BUFFER].RenderPass == RENDER_PASS_GLOW_BUFFER && r.SubPrimitiveLists[SUBPRIM_BOUNDING_VOLUME].BoundingVolumePass
          && r.SubPrimitiveLists[SUBPRIM_BOUNDING_VOLUME].RenderPass == RENDER_PASS_NULL && r.SubPrimitiveLists[SUBPRIM_2D].RequiresStableSort
          && !r.SubPrimitiveLists[SUBPRIM_OPAQUE].RequiresStableSort && r.SubPrimitiveLists[SUBPRIM_2D].Sorted && r.SubPrimitiveLists[SUBPRIM_2D].Enable;
    printf(ok ? "00b84470_TEST PASS\n" : "00b84470_TEST FAIL\n");
    return ok ? 0 : 1;
}