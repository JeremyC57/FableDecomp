// CLandscapeBackgroundPatch::RenderWithOcclusion(long& queryHandle, bool& occluded) -- render a landscape patch under hardware occlusion queries: with occlusion off just render; with no pending query render inside a new query; otherwise, if last frame's query found no pixels, draw the patch's bounding box as the next query (clearing the parent chain's visibility the first time), else render it again inside a new query
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BE79D0, 526 bytes (+ int3 padding), `ret 8`.
// Callees: CLandscapeBackgroundPatch::Render(bool) 0x00BE6F70; on the occlusion manager global 0x01436E5C: BeginQuery 0x00B8D090 and
//   EndQuery(long) 0x00B8D0A0 (empty in retail; byte-identical bodies carry propagated labels), GetQueryResult(long, bool)
//   0x00B8D0B0 (manifest label "CEngineInternalPrimitiveBase::AddChildPrimitive" is propagated), RenderOcclusionBox(const
//   CMatrix3x4&) 0x00B8D420; CLandscapeBackgroundTreeNode::SetVisibilityFlagAndUpdateParentChain(bool) 0x00BF7370; the engine
//   global 0x01436EA8 (+0x63E occlusion-enabled flag).
// Layout: CLandscapeBackgroundPatch as engine/CLandscapeBackgroundPatch.h (MinZ +0xf8, MaxZ +0xfc, MapX/MapY/Width/Height
//   +0x10c..+0x112, Parent +0x04); that header is a flattened view without methods, so the patch is modelled locally with the
//   same offsets. The parent tree node's +0xA4 pointer reaches the landscape map whose +0x14 header holds the world-map origin
//   (+0x60 X, +0x62 Y) -- descriptive, the PDB node view differs from retail here. The occlusion manager keeps a fixed
//   0x2000-entry query array (+0x4014) and its count (+0x401C); the query allocation is inline.
class C3DVector { public: float X, Y, Z; };
class CMatrix3x4 { public:
    C3DVector Right, Up, Forward, Position;
};
struct SOcclusionQuery {
    long Query;
    long Result;
    SOcclusionQuery() : Query(0), Result(0) { }
};
class CEngineOcclusionManager { public:
    unsigned char    _pad0000[0x4014];
    SOcclusionQuery* Queries;                       // +0x4014
    unsigned char    _pad4018[4];
    unsigned long    QueryCount;                    // +0x401C
    void BeginQuery();                              // 0x00B8D090
    void EndQuery(long handle);                     // 0x00B8D0A0
    unsigned long GetQueryResult(long handle, bool wait);   // 0x00B8D0B0
    long RenderOcclusionBox(const CMatrix3x4& box); // 0x00B8D420
    __forceinline long AddQuery()
    {
        unsigned long index = QueryCount;
        if (index < 0x2000) {
            Queries[index] = SOcclusionQuery();
            ++QueryCount;
            return index;
        }
        return -1;
    }
};
extern CEngineOcclusionManager* g_pOcclusionManager;   // 0x01436E5C
struct CEngineGlobal {
    unsigned char _pad[0x63e];
    bool          OcclusionEnabled;                 // +0x63E (descriptive)
};
extern CEngineGlobal* g_pEngine;                    // 0x01436EA8
struct CLandscapeMapHeader { unsigned char _pad[0x60]; unsigned short OriginX, OriginY; };   // +0x60 / +0x62
struct CLandscapeMapView { unsigned char _pad[0x14]; CLandscapeMapHeader* Header; };          // +0x14
class CLandscapeBackgroundTreeNode { public:
    unsigned char      _pad[0xa4];
    CLandscapeMapView* Map;                         // +0xA4 (descriptive)
    void SetVisibilityFlagAndUpdateParentChain(bool visible);   // 0x00BF7370
};
class CLandscapeBackgroundPatch { public:
    void*                         __vftable;        // +0x00
    CLandscapeBackgroundTreeNode* Parent;           // +0x04
    unsigned char                 _pad08[0xf8 - 0x08];
    float                         MinZ;             // +0xf8
    float                         MaxZ;             // +0xfc
    unsigned char                 _pad100[0x10c - 0x100];
    unsigned short                MapX;             // +0x10c
    unsigned short                MapY;             // +0x10e
    unsigned short                Width;            // +0x110
    unsigned short                Height;           // +0x112
    bool Render(bool full);                         // 0x00BE6F70
    void RenderWithOcclusion(long& queryHandle, bool& occluded);
};

void CLandscapeBackgroundPatch::RenderWithOcclusion(long& queryHandle, bool& occluded)
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