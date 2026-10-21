// Grade: DIFFER(381v379) -- the occlusion box: retail builds it from integer locals (map X/Y copied to frame words, width/height converted through one scratch word, MinZ/MaxZ loaded first) in a 0x3c frame and walks the list with ebx = list, esi = iterator; ours converts straight from the patch (0x34 frame) with esi/edi
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
//   the map position and MinZ); the query allocation is an inline manager helper returning -1 when full.
struct CMatrix3x4 { float M[12]; };
struct SOcclusionQuery { long A; long B; };

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
            Queries[QueryCount].A = 0;
            Queries[QueryCount].B = 0;
            return QueryCount++;
        }
        return -1;
    }
};
extern CEngineOcclusionManager* g_pEngineOcclusionManager;   // 0x01436E5C

class CEngineWaterBackgroundSubPatch {
public:
    unsigned char  _pad00[0x48];
    unsigned long  OcclusionID;                    // +0x48
    long           MapX;                           // +0x4c
    long           MapY;                           // +0x50
    unsigned short Width;                          // +0x54
    unsigned short Height;                         // +0x56
    float          MinZ;                           // +0x58
    float          MaxZ;                           // +0x5c
};

template <class T> struct CArray {
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
            if ((*it)->OcclusionID > 0 && !g_pEngineOcclusionManager->IsQueryVisible((*it)->OcclusionID, true)) {
                CEngineWaterBackgroundSubPatch* patch = *it;
                CMatrix3x4 box;
                box.M[0] = (float)(long)patch->Width;
                box.M[1] = 0.0f;
                box.M[2] = 0.0f;
                box.M[3] = 0.0f;
                box.M[4] = (float)(long)patch->Height;
                box.M[5] = 0.0f;
                box.M[6] = 0.0f;
                box.M[7] = 0.0f;
                box.M[8] = patch->MaxZ - patch->MinZ;
                box.M[9] = (float)patch->MapX;
                box.M[10] = (float)patch->MapY;
                box.M[11] = patch->MinZ;
                (*it)->OcclusionID = g_pEngineOcclusionManager->RenderOcclusionBox(box);
            }
            else {
                g_pEngineOcclusionManager->BeginQuery();
                RenderBackgroundPatch(*it);
                long id = g_pEngineOcclusionManager->AllocateQuery();
                g_pEngineOcclusionManager->EndQuery(id);
                (*it)->OcclusionID = id;
            }
        }
        else
            RenderBackgroundPatch(*it);
    }
}
