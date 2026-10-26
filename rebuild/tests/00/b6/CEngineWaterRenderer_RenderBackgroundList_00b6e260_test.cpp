// Behaviour test for 00b6e260 CEngineWaterRenderer::RenderBackgroundList: final.cpp body verbatim; the occlusion manager
// and RenderBackgroundPatch are stubs that record calls, boxes and query ids.
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __forceinline inline
#endif
extern "C" int __cdecl printf(const char* format, ...);
// CEngineWaterRenderer::RenderBackgroundList(CArray<CEngineWaterBackgroundSubPatch*>& list, float time) -- render a list of background water patches; with occlusion enabled, a patch whose last occlusion query says it is hidden only re-issues an occlusion box test, every other patch is rendered inside a fresh occlusion query
// Leaf: RenderBackgroundList
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B6E260, length 384 bytes (with int3 padding; body 0x17b), `ret 8`.
// PDB: ?RenderBackgroundList@CEngineWaterRenderer@@AAEXAAV?$CArray@PAVCEngineWaterBackgroundSubPatch@@@@M@Z.
// Callees: on the occlusion manager @ 0x01436E5C: the query result test @ 0x00B8D0B0 (id, wait; manifest label propagated),
//   RenderOcclusionBox(const CMatrix3x4&) @ 0x00B8D420, the begin/end query hooks @ 0x00B8D090 / 0x00B8D0A0;
//   CEngineWaterRenderer::RenderBackgroundPatch @ 0x00B6E010.
// Layout: retail CEngineWaterRenderer occlusion flag +0x296; CEngineWaterBackgroundSubPatch (PDB): OcclusionID +0x48, MapX
//   +0x4c, MapY +0x50, Width +0x54, Height +0x56, MinZ +0x58, MaxZ +0x5c; the occlusion manager's query records (8 bytes) at
//   +0x4014 with the count at +0x401c (0x2000 records).
// Shape: the occlusion box is a CMatrix3x4 built field by field (scale rows from the integer extents, translation row from
//   the map position and MinZ) from locals read in retail order (map Y, Height as an unsigned short local -- the early
//   16-bit `mov cx` load --, MinZ, MaxZ-MinZ, map X); the occlusion id is one local shared by both arms and stored once
//   after them (tail-merged per arm, held in edi); the query allocation is an inline manager helper (id = count; record =
//   SOcclusionQuery(0,0); ++count) returning -1 when full.
class CMatrix3x4 { public: float M[12]; };
struct SOcclusionQuery { long A; long B; SOcclusionQuery() {} SOcclusionQuery(long a, long b) : A(a), B(b) {} };

class CEngineOcclusionManager {
public:
    unsigned char    _pad[0x4014];
    SOcclusionQuery* Queries;                      // +0x4014
    long             _pad4018;
    unsigned long    QueryCount;                   // +0x401c
    long IsQueryVisible(unsigned long id, bool wait);   // 0x00B8D0B0
    long RenderOcclusionBox(const CMatrix3x4& box);     // 0x00B8D420
    void BeginQuery();                             // 0x00B8D090
    void EndQuery(long id);                        // 0x00B8D0A0
    __forceinline long AllocateQuery()
    {
        if (QueryCount < 0x2000) {
            unsigned long id = QueryCount;
            Queries[id] = SOcclusionQuery(0, 0);
            ++QueryCount;
            return id;
        }
        return -1;
    }
};
extern CEngineOcclusionManager* g_pEngineOcclusionManager;   // 0x01436E5C

class C2DCoordI { public: long X, Y; };
class CEngineWaterBackgroundSubPatch {
public:
    unsigned char  _pad00[0x48];
    unsigned long  OcclusionID;                    // +0x48
    C2DCoordI      MapPos;                         // +0x4c
    unsigned short Width;                          // +0x54
    unsigned short Height;                         // +0x56
    float          MinZ;                           // +0x58
    float          MaxZ;                           // +0x5c
};

template <class T> class CArray { public:
    T* First;
    T* Last;
    T* End;
};

class CEngineWaterRenderer {
public:
    unsigned char _pad[0x296];
    bool          OcclusionEnabled;                // +0x296
    void RenderBackgroundPatch(CEngineWaterBackgroundSubPatch* patch);   // 0x00B6E010
    void RenderBackgroundList(CArray<CEngineWaterBackgroundSubPatch*>& list, float time);
};

void CEngineWaterRenderer::RenderBackgroundList(CArray<CEngineWaterBackgroundSubPatch*>& list, float time)
{
    for (CEngineWaterBackgroundSubPatch** it = list.First; it != list.Last; ++it) {
        if (OcclusionEnabled) {
            long id;
            if ((*it)->OcclusionID > 0 && !g_pEngineOcclusionManager->IsQueryVisible((*it)->OcclusionID, true)) {
                CEngineWaterBackgroundSubPatch* patch = *it;
                long y = patch->MapPos.Y;
                unsigned short h = patch->Height;
                float minZ = patch->MinZ;
                float depth = patch->MaxZ - patch->MinZ;
                long x = patch->MapPos.X;
                CMatrix3x4 box;
                box.M[0] = (float)(long)patch->Width;
                box.M[1] = 0.0f;
                box.M[2] = 0.0f;
                box.M[3] = 0.0f;
                box.M[4] = (float)(long)h;
                box.M[5] = 0.0f;
                box.M[6] = 0.0f;
                box.M[7] = 0.0f;
                box.M[8] = depth;
                box.M[9] = (float)x;
                box.M[10] = (float)y;
                box.M[11] = minZ;
                id = g_pEngineOcclusionManager->RenderOcclusionBox(box);
            }
            else {
                g_pEngineOcclusionManager->BeginQuery();
                RenderBackgroundPatch(*it);
                id = g_pEngineOcclusionManager->AllocateQuery();
                g_pEngineOcclusionManager->EndQuery(id);
            }
            (*it)->OcclusionID = id;
        }
        else
            RenderBackgroundPatch(*it);
    }
}

static int g_fail;
static void Check(bool ok, const char* what) { if (!ok) { printf("FAIL: %s\n", what); g_fail = 1; } }
static SOcclusionQuery g_queries[0x2000];
static CEngineOcclusionManager g_mgr;
CEngineOcclusionManager* g_pEngineOcclusionManager = &g_mgr;
static int g_rendered, g_begins, g_ends, g_boxes, g_visTests;
static long g_lastEnd;
static CMatrix3x4 g_box;
static long g_visible;          // IsQueryVisible result
long CEngineOcclusionManager::IsQueryVisible(unsigned long id, bool wait) { ++g_visTests; return wait ? g_visible : 0; }
long CEngineOcclusionManager::RenderOcclusionBox(const CMatrix3x4& box) { g_box = box; ++g_boxes; return 77; }
void CEngineOcclusionManager::BeginQuery() { ++g_begins; }
void CEngineOcclusionManager::EndQuery(long id) { ++g_ends; g_lastEnd = id; }
void CEngineWaterRenderer::RenderBackgroundPatch(CEngineWaterBackgroundSubPatch* patch) { ++g_rendered; }

static void Reset() { g_rendered = g_begins = g_ends = g_boxes = g_visTests = 0; g_lastEnd = 12345; }

int main()
{
    g_mgr.Queries = g_queries;
    g_mgr.QueryCount = 5;
    g_queries[5].A = 9; g_queries[5].B = 9;
    static CEngineWaterBackgroundSubPatch p0, p1;
    p0.OcclusionID = 0; p0.MapPos.X = 10; p0.MapPos.Y = -3; p0.Width = 16; p0.Height = 40000; p0.MinZ = 1.5f; p0.MaxZ = 4.0f;
    p1.OcclusionID = 3; p1.MapPos.X = 2; p1.MapPos.Y = 7; p1.Width = 8; p1.Height = 4; p1.MinZ = -1.0f; p1.MaxZ = 2.0f;
    CEngineWaterBackgroundSubPatch* arr[2] = { &p0, &p1 };
    CArray<CEngineWaterBackgroundSubPatch*> list; list.First = arr; list.Last = arr + 2; list.End = arr + 2;
    static CEngineWaterRenderer r;

    // occlusion off: both rendered, nothing else
    r.OcclusionEnabled = false; Reset();
    r.RenderBackgroundList(list, 0.0f);
    Check(g_rendered == 2 && g_begins == 0 && g_boxes == 0 && p0.OcclusionID == 0 && p1.OcclusionID == 3, "occlusion off");

    // occlusion on, p1's query hidden: p0 queried+rendered (id 5), p1 gets a box test
    r.OcclusionEnabled = true; g_visible = 0; Reset();
    r.RenderBackgroundList(list, 0.0f);
    Check(g_rendered == 1 && g_begins == 1 && g_ends == 1 && g_lastEnd == 5 && p0.OcclusionID == 5, "p0 fresh query");
    Check(g_mgr.QueryCount == 6 && g_queries[5].A == 0 && g_queries[5].B == 0, "query record zeroed, count bumped");
    Check(g_visTests == 1 && g_boxes == 1 && p1.OcclusionID == 77, "p1 box test");
    Check(g_box.M[0] == 8.0f && g_box.M[4] == 4.0f && g_box.M[8] == 3.0f && g_box.M[9] == 2.0f && g_box.M[10] == 7.0f &&
          g_box.M[11] == -1.0f && g_box.M[1] == 0.0f && g_box.M[2] == 0.0f && g_box.M[3] == 0.0f && g_box.M[5] == 0.0f &&
          g_box.M[6] == 0.0f && g_box.M[7] == 0.0f, "box values");

    // p0 hidden too (large unsigned height), p1 visible -> rendered; manager full -> id -1
    p0.OcclusionID = 4; p1.OcclusionID = 3; g_mgr.QueryCount = 0x2000; Reset();
    g_visible = 0;
    CEngineWaterBackgroundSubPatch* one[1] = { &p0 };
    CArray<CEngineWaterBackgroundSubPatch*> l0; l0.First = one; l0.Last = one + 1; l0.End = one + 1;
    r.RenderBackgroundList(l0, 0.0f);
    Check(g_boxes == 1 && g_box.M[4] == 40000.0f && g_box.M[9] == 10.0f && g_box.M[10] == -3.0f && g_box.M[8] == 2.5f && g_box.M[11] == 1.5f, "p0 box, unsigned height");
    g_visible = 1; Reset();
    CEngineWaterBackgroundSubPatch* two[1] = { &p1 };
    CArray<CEngineWaterBackgroundSubPatch*> l1; l1.First = two; l1.Last = two + 1; l1.End = two + 1;
    r.RenderBackgroundList(l1, 0.0f);
    Check(g_rendered == 1 && g_ends == 1 && g_lastEnd == -1 && p1.OcclusionID == (unsigned long)-1 && g_mgr.QueryCount == 0x2000, "full manager -> -1");

    // empty list
    Reset(); l1.Last = l1.First;
    r.RenderBackgroundList(l1, 0.0f);
    Check(g_rendered == 0 && g_visTests == 0, "empty list");
    printf(g_fail ? "00b6e260_TEST FAIL\n" : "00b6e260_TEST PASS\n");
    return g_fail;
}