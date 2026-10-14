#include <cstdio>
struct CComp {
    virtual bool IsRenderLayerAbsolute() { return absolute; } virtual bool IsRenderLayerDetached() { return detached; }
    virtual long GetRenderLayer() { if (!IsRenderLayerAbsolute() && !IsRenderLayerDetached() && PParent) return PParent->GetRenderLayer() + Layer; return Layer; }
    bool absolute, detached; CComp* PParent; char Layer;
};
int main()
{
    CComp root; root.absolute = false; root.detached = false; root.PParent = 0; root.Layer = 3;
    CComp child; child.absolute = false; child.detached = false; child.PParent = &root; child.Layer = -1;
    if (root.GetRenderLayer() != 3 || child.GetRenderLayer() != 2) { std::printf("0052f3e0_TEST FAIL\n"); return 1; }
    child.detached = true; if (child.GetRenderLayer() != -1) { std::printf("0052f3e0_TEST FAIL\n"); return 1; }
    child.detached = false; child.absolute = true; if (child.GetRenderLayer() != -1) { std::printf("0052f3e0_TEST FAIL\n"); return 1; }
    std::printf("0052f3e0_TEST PASS\n");
    return 0;
}