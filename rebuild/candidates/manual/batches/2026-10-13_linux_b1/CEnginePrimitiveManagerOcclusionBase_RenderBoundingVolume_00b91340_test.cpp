#include <cstdio>

static void* g_seen = 0;
struct CEngineInternalPrimitiveBase { int dummy; };
struct CEngineSubPrimitiveBase { void* vt; unsigned long flags; unsigned char alpha; unsigned char pad[3]; CEngineInternalPrimitiveBase* Parent; };
struct CEnginePrimitiveManagerOcclusionBase {
    void RenderBoundingBox(CEngineInternalPrimitiveBase* p) { g_seen = p; }
    void RenderBoundingVolume(CEngineSubPrimitiveBase& s) { RenderBoundingBox(s.Parent); }
};
int main()
{
    CEngineInternalPrimitiveBase prim; CEngineSubPrimitiveBase sub; sub.Parent = &prim;
    CEnginePrimitiveManagerOcclusionBase mgr; mgr.RenderBoundingVolume(sub);
    if (g_seen != &prim) { std::printf("00b91340_TEST FAIL\n"); return 1; }
    std::printf("00b91340_TEST PASS\n");
    return 0;
}
