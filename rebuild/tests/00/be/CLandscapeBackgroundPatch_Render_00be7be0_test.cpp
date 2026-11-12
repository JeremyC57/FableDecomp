// Behaviour test for CLandscapeBackgroundPatch::Render(bool, long&, bool&) 0x00BE7BE0: a copy of the model (patch, water sub-patch,
// the water renderer's six render lists and its two inline list helpers) with the two draw callees replaced by recorders.
#include <cstdio>

class CEngineWaterBackgroundSubPatch { public:
    unsigned short PolyCount;
    int            WaterType;
    bool IsSea() const { return WaterType == 3 || WaterType == 4 || WaterType == 5; }
    bool IsEmpty() const { return !(PolyCount > 0); }
};
struct CList {
    CEngineWaterBackgroundSubPatch* Items[8];
    int Count;
    CList() : Count(0) { }
    void push_back(CEngineWaterBackgroundSubPatch* const& x) { Items[Count++] = x; }
};
class CEngineWaterRenderer { public:
    CList BackgroundRenderList, AlphaedBackgroundRenderList, IceBackgroundRenderList, IceAlphaedBackgroundRenderList;
    CList SeaMidgroundRenderList, SeaAlphaedMidgroundRenderList;
    void AddMidgroundSeaPatchToRenderList(CEngineWaterBackgroundSubPatch* const patch, bool alphaed)
    {
        if (!patch->IsEmpty()) {
            if (alphaed)
                SeaAlphaedMidgroundRenderList.push_back(patch);
            else
                SeaMidgroundRenderList.push_back(patch);
        }
    }
    void AddBackgroundWaterPatchToRenderList(CEngineWaterBackgroundSubPatch* const patch, bool alphaed)
    {
        if (!patch->IsEmpty()) {
            if (patch->IsSea())
                AddMidgroundSeaPatchToRenderList(patch, alphaed);
            else {
                if (patch->WaterType == 8) {
                    if (alphaed)
                        IceAlphaedBackgroundRenderList.push_back(patch);
                    else
                        IceBackgroundRenderList.push_back(patch);
                } else {
                    if (alphaed)
                        AlphaedBackgroundRenderList.push_back(patch);
                    else
                        BackgroundRenderList.push_back(patch);
                }
            }
        }
    }
    int Total() const
    {
        return BackgroundRenderList.Count + AlphaedBackgroundRenderList.Count + IceBackgroundRenderList.Count
             + IceAlphaedBackgroundRenderList.Count + SeaMidgroundRenderList.Count + SeaAlphaedMidgroundRenderList.Count;
    }
};
static CEngineWaterRenderer* g_pEngineWaterRenderer;

class CLandscapeBackgroundPatch { public:
    CEngineWaterBackgroundSubPatch* WaterSubPatch;
    bool WaterOnlyPatch;
    // recorders standing in for the two draw callees
    bool  DrawResult;
    int   PlainDraws, OcclusionDraws;
    bool  LastPlainArg;
    long* LastQuery;
    bool* LastOccluded;
    bool Render(bool withOcclusion) { ++PlainDraws; LastPlainArg = withOcclusion; return DrawResult; }
    void RenderWithOcclusion(long& queryHandle, bool& occluded) { ++OcclusionDraws; LastQuery = &queryHandle; LastOccluded = &occluded; }
    bool HasWater() { return WaterSubPatch != 0; }
    void Render(bool withOcclusion, long& queryHandle, bool& occluded)
    {
        if (!WaterOnlyPatch) {
            bool alphaed = false;
            if (withOcclusion)
                RenderWithOcclusion(queryHandle, occluded);
            else
                alphaed = Render(false);
            if (HasWater())
                g_pEngineWaterRenderer->AddBackgroundWaterPatchToRenderList(WaterSubPatch, alphaed);
        } else {
            if (HasWater())
                g_pEngineWaterRenderer->AddMidgroundSeaPatchToRenderList(WaterSubPatch, false);
        }
    }
};

static int g_fail;
static void Check(bool ok, const char* what)
{
    if (!ok) { std::printf("  failed: %s\n", what); ++g_fail; }
}
// One call on a fresh renderer; returns the list the sub-patch landed on (0 = none) and checks nothing else was queued.
static CList* Run(CLandscapeBackgroundPatch& patch, CEngineWaterRenderer& r, bool withOcclusion)
{
    long query = 7; bool occluded = false;
    patch.PlainDraws = patch.OcclusionDraws = 0; patch.LastPlainArg = true; patch.LastQuery = 0; patch.LastOccluded = 0;
    g_pEngineWaterRenderer = &r;
    patch.Render(withOcclusion, query, occluded);
    if (patch.OcclusionDraws)
        Check(patch.LastQuery == &query && patch.LastOccluded == &occluded, "references forwarded to RenderWithOcclusion");
    Check(query == 7 && !occluded, "Render itself leaves the referenced values alone");
    CList* lists[6] = { &r.BackgroundRenderList, &r.AlphaedBackgroundRenderList, &r.IceBackgroundRenderList,
                        &r.IceAlphaedBackgroundRenderList, &r.SeaMidgroundRenderList, &r.SeaAlphaedMidgroundRenderList };
    CList* hit = 0;
    for (int i = 0; i < 6; ++i)
        if (lists[i]->Count) {
            Check(lists[i]->Count == 1 && lists[i]->Items[0] == patch.WaterSubPatch, "queued exactly the sub-patch");
            hit = lists[i];
        }
    Check(r.Total() <= 1, "at most one list entry per call");
    return hit;
}

int main()
{
    CEngineWaterBackgroundSubPatch water = { 12, 0 };
    CLandscapeBackgroundPatch patch;
    patch.WaterSubPatch = &water; patch.WaterOnlyPatch = false; patch.DrawResult = false;

    // water-only patch: no draw call, any non-empty sub-patch goes on the sea midground list (never the alphaed one)
    patch.WaterOnlyPatch = true;
    for (int type = 0; type < 10; ++type) {
        water.WaterType = type; patch.DrawResult = true;
        CEngineWaterRenderer r;
        Check(Run(patch, r, false) == &r.SeaMidgroundRenderList, "water-only -> SeaMidgroundRenderList");
        Check(patch.PlainDraws == 0 && patch.OcclusionDraws == 0, "water-only draws nothing");
    }
    { water.PolyCount = 0; CEngineWaterRenderer r; Check(Run(patch, r, true) == 0, "water-only, empty sub-patch -> nothing"); water.PolyCount = 12; }
    { patch.WaterSubPatch = 0; CEngineWaterRenderer r; Check(Run(patch, r, true) == 0, "water-only, no sub-patch -> nothing"); patch.WaterSubPatch = &water; }

    // normal patch, drawn directly: Render(false) is called once and its result picks the alphaed list
    patch.WaterOnlyPatch = false;
    for (int type = 0; type < 10; ++type) {
        for (int alphaed = 0; alphaed < 2; ++alphaed) {
            water.WaterType = type; patch.DrawResult = alphaed != 0;
            CEngineWaterRenderer r;
            CList* hit = Run(patch, r, false);
            CList* want;
            if (type == 3 || type == 4 || type == 5) want = alphaed ? &r.SeaAlphaedMidgroundRenderList : &r.SeaMidgroundRenderList;
            else if (type == 8)                      want = alphaed ? &r.IceAlphaedBackgroundRenderList : &r.IceBackgroundRenderList;
            else                                     want = alphaed ? &r.AlphaedBackgroundRenderList : &r.BackgroundRenderList;
            Check(hit == want, "direct draw: list by water type and draw result");
            Check(patch.PlainDraws == 1 && patch.OcclusionDraws == 0 && patch.LastPlainArg == false, "direct draw calls Render(false) once");
        }
    }
    // normal patch, occlusion path: RenderWithOcclusion gets the two references, the list is never an alphaed one
    for (int type = 0; type < 10; ++type) {
        water.WaterType = type; patch.DrawResult = true;
        CEngineWaterRenderer r;
        CList* hit = Run(patch, r, true);
        CList* want = (type == 3 || type == 4 || type == 5) ? &r.SeaMidgroundRenderList : type == 8 ? &r.IceBackgroundRenderList : &r.BackgroundRenderList;
        Check(hit == want, "occlusion draw: non-alphaed list by water type");
        Check(patch.PlainDraws == 0 && patch.OcclusionDraws == 1, "occlusion draw calls RenderWithOcclusion once");
    }
    // the draw still happens when there is nothing to queue
    { water.PolyCount = 0; water.WaterType = 4; CEngineWaterRenderer r; Check(Run(patch, r, false) == 0 && patch.PlainDraws == 1, "empty sub-patch: drawn, not queued"); water.PolyCount = 12; }
    { patch.WaterSubPatch = 0; CEngineWaterRenderer r; Check(Run(patch, r, true) == 0 && patch.OcclusionDraws == 1, "no sub-patch: drawn, not queued"); patch.WaterSubPatch = &water; }
    { water.PolyCount = 1; water.WaterType = 8; patch.DrawResult = true; CEngineWaterRenderer r; Check(Run(patch, r, false) == &r.IceAlphaedBackgroundRenderList, "one polygon is not empty"); }

    if (g_fail) { std::printf("00be7be0_TEST FAIL\n"); return 1; }
    std::printf("00be7be0_TEST PASS\n");
    return 0;
}