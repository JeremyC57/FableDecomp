// 00be79d0 behaviour test: CLandscapeBackgroundPatch::RenderWithOcclusion (match.cpp body, host-modelled)
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
struct C3DVector { float X, Y, Z; };
struct CMatrix3x4 { C3DVector Right, Up, Forward, Position; };
struct SOcclusionQuery { long Query; long Result; SOcclusionQuery() : Query(0), Result(0) {} };
static SOcclusionQuery g_q[0x2000];
struct CEngineOcclusionManager {
    SOcclusionQuery* Queries; unsigned long QueryCount;
    int begins, ends; long lastEnd; unsigned long result; CMatrix3x4 lastBox; long boxRet;
    void BeginQuery() { ++begins; }
    void EndQuery(long h) { ++ends; lastEnd = h; }
    unsigned long GetQueryResult(long, bool) { return result; }
    long RenderOcclusionBox(const CMatrix3x4& b) { lastBox = b; return boxRet; }
    long AddQuery() { unsigned long index = QueryCount; if (index < 0x2000) { Queries[index] = SOcclusionQuery(); ++QueryCount; return index; } return -1; }
};
static CEngineOcclusionManager g_mgr;
static CEngineOcclusionManager* g_pOcclusionManager = &g_mgr;
struct CEngineGlobal { bool OcclusionEnabled; };
static CEngineGlobal g_eng; static CEngineGlobal* g_pEngine = &g_eng;
struct CLandscapeMapHeader { unsigned short OriginX, OriginY; };
struct CLandscapeMapView { CLandscapeMapHeader* Header; };
struct CLandscapeBackgroundTreeNode { CLandscapeMapView* Map; int visCalls; bool lastVis;
    void SetVisibilityFlagAndUpdateParentChain(bool v) { ++visCalls; lastVis = v; } };
struct CLandscapeBackgroundPatch {
    CLandscapeBackgroundTreeNode* Parent; float MinZ, MaxZ; unsigned short MapX, MapY, Width, Height;
    int renders; bool lastFull;
    bool Render(bool full) { ++renders; lastFull = full; return true; }
    void RenderWithOcclusion(long& queryHandle, bool& occluded)
    {
        if (g_pEngine->OcclusionEnabled) {
            if (queryHandle >= 0) {
                if (g_pOcclusionManager->GetQueryResult(queryHandle, true) == 0) {
                    float y = (float)((Parent->Map->Header)->OriginY + MapY);
                    float x = (float)((Parent->Map->Header)->OriginX + MapX);
                    CMatrix3x4 box;
                    float width = (float)Width;
                    float height = (float)Height;
                    box.Right.X = width;    box.Right.Y = 0.0f;   box.Right.Z = 0.0f;
                    box.Up.X = 0.0f;        box.Up.Y = height;    box.Up.Z = 0.0f;
                    box.Forward.X = 0.0f;   box.Forward.Y = 0.0f; box.Forward.Z = MaxZ - MinZ;
                    box.Position.X = x;
                    box.Position.Z = MinZ;
                    box.Position.Y = y;
                    queryHandle = g_pOcclusionManager->RenderOcclusionBox(box);
                    if (occluded)
                        Parent->SetVisibilityFlagAndUpdateParentChain(false);
                    occluded = true;
                } else {
                    g_pOcclusionManager->BeginQuery();
                    Render(true);
                    queryHandle = g_pOcclusionManager->AddQuery();
                    occluded = false;
                    g_pOcclusionManager->EndQuery(queryHandle);
                }
            } else {
                g_pOcclusionManager->BeginQuery();
                Render(false);
                queryHandle = g_pOcclusionManager->AddQuery();
                occluded = true;
                g_pOcclusionManager->EndQuery(queryHandle);
            }
        } else {
            Render(true);
        }
    }
};
int main()
{
    CLandscapeMapHeader hdr = { 100, 200 }; CLandscapeMapView view = { &hdr };
    CLandscapeBackgroundTreeNode node = { &view, 0, true };
    CLandscapeBackgroundPatch p = { &node, 1.5f, 4.0f, 3, 7, 16, 32, 0, false };
    g_mgr.Queries = g_q; g_mgr.QueryCount = 0;
    long q = 5; bool occ = false;
    g_eng.OcclusionEnabled = false;
    p.RenderWithOcclusion(q, occ);
    CHECK(p.renders == 1 && p.lastFull && q == 5 && g_mgr.begins == 0);
    g_eng.OcclusionEnabled = true;
    q = -1; occ = false;
    p.RenderWithOcclusion(q, occ);
    CHECK(p.renders == 2 && !p.lastFull && q == 0 && occ && g_mgr.QueryCount == 1 && g_mgr.ends == 1 && g_mgr.lastEnd == 0);
    g_mgr.result = 1; occ = true;
    p.RenderWithOcclusion(q, occ);
    CHECK(p.renders == 3 && p.lastFull && q == 1 && !occ && g_mgr.begins == 2);
    g_mgr.result = 0; g_mgr.boxRet = 77; occ = true;
    p.RenderWithOcclusion(q, occ);
    CHECK(q == 77 && occ && node.visCalls == 1 && !node.lastVis && p.renders == 3);
    const CMatrix3x4& b = g_mgr.lastBox;
    CHECK(b.Right.X == 16.0f && b.Right.Y == 0.0f && b.Up.Y == 32.0f && b.Forward.Z == 2.5f);
    CHECK(b.Position.X == 103.0f && b.Position.Y == 207.0f && b.Position.Z == 1.5f);
    p.RenderWithOcclusion(q, occ);
    CHECK(node.visCalls == 2);
    occ = false; p.RenderWithOcclusion(q, occ);
    CHECK(node.visCalls == 2 && occ);
    g_mgr.QueryCount = 0x2000; q = -1;
    p.RenderWithOcclusion(q, occ);
    CHECK(q == -1 && g_mgr.lastEnd == -1);
    if (!g_fail) printf("00be79d0_TEST PASS\n");
    return g_fail;
}