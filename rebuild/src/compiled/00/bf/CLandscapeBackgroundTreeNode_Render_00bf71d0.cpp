// CLandscapeBackgroundTreeNode::Render(unsigned long flags) -- cull the node's box against the camera's four frustum planes (the box corner picked by each normal's sign); a culled node marks its tree invisible unless it already is; a visible leaf patch renders, a visible split branch renders both halves and is visible when either half is (or invisible-and-updated when both halves were updated invisible); the node then records the result
// Leaf: Render@CLandscapeBackgroundTreeNode
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF71D0, length 403 bytes (`ret 4`).
// Callees: itself (both branch halves, flags 0), CLandscapeBackgroundPatch::Render(bool, long*, bool*) @ 0x00BE7BE0,
//   MarkTreeAsInvisibleLastFrame @ 0x00BF6A30 (landed); the sub node's IsPatch (slot 4) through the inline GetPatch /
//   GetSplitBranch (0x00BF6130 / 0x00BF6160).
// Layout: the retail CLandscapeBackgroundTreeNode model (0xE8 bytes, Ego_r.pdb offsets - 8) copied from the landed
//   MarkTreeAsInvisibleLastFrame 0x00BF6A30; BoundingBox Min +0xa8 / Max +0xb4; the engine camera 0x01436EA0 with its four
//   frustum planes at +0x1c0 (as the landed sources name it); 0x0122DEDC = 0.0f.
// Shape: the plane normal is read into three named float locals (kept on the x87 stack: `fld st(2)` for each sign test and
//   `fmul st(n)`); the cull result is a `visible` flag set before `break`, so the invisible branch is the else-arm at the end.
struct C3DVector { float X, Y, Z; };
struct C3DBoundingBox { C3DVector Min, Max; };
struct CPlane { C3DVector Normal; float D; };
struct CEngineCamera {
    unsigned char _pad[0x1c0];
    CPlane        FrustrumPlanes[4];                // +0x1c0
};
extern CEngineCamera* g_pEngineCamera;              // 0x01436EA0
struct CMovableResource {                          // landscape-memory resources: re-parented after a relocation
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual bool IsPatch();                        // slot 4 (+0x10), meaningful on a tree node's sub node
    virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void OnPostParentRelocation(CMovableResource* parent);   // slot 8 (+0x20)
};
struct CEngineLandscapeMemoryManager {
    void RebindBlockOwner(void* block, CMovableResource* owner);   // 0x00BD9AB0
};
struct SEngine_01436ea8 {
    unsigned char                  _pad[0x6b0];
    CEngineLandscapeMemoryManager* m_pLandscapeMemory;   // +0x6b0
};
extern SEngine_01436ea8* g_pEngine_01436ea8;      // 0x01436EA8
struct SLandscapeMapHeader {
    unsigned char  _pad00[0x60];
    unsigned short OriginX;                        // +0x60
    unsigned short OriginY;                        // +0x62
};
enum LANDSCAPE_EDGE_STRIP_SIDE { EDGE_RIGHT = 0, EDGE_LEFT = 1, EDGE_BOTTOM = 2, EDGE_TOP = 3 };
struct CEngineLandscapeMap {
    unsigned char        _pad00[0x14];
    SLandscapeMapHeader* Header;                   // +0x14
    void InvalidateEdge(long x, long y, long length, LANDSCAPE_EDGE_STRIP_SIDE side);   // 0x00BDCBB0
};
struct CLandscapeBackgroundLoadTask {              // 16 bytes, no vtable
    CMovableResource* m_pOwner;                    // +0x00
    void*         m_pManager;                      // +0x04
    long          m_filePos;                       // +0x08
    unsigned char m_lod;                           // +0x0c the LOD band of a sub-patch load
    bool          m_isPatchLoad;                   // +0x0d
    bool          m_requested;                     // +0x0e
    unsigned char _pad0f;
};
struct CLandscapeBackgroundSubNodeBase : CMovableResource {   // the node's sub node: a leaf patch or a split branch
};
struct CLandscapeBackgroundPatch;
struct CLandscapeBackgroundSubNodeSplitBranch;
struct CLandscapeBackgroundTreeNode : CMovableResource {
    unsigned char                    _pad04[0x88];
    long                             FileBlockPos;            // +0x8c
    long                             FileBlockSize;           // +0x90
    long                             OffsetIntoFileBlock;     // +0x94
    CLandscapeBackgroundLoadTask*    AsyncLoadTask;           // +0x98
    CLandscapeBackgroundTreeNode*    Parent;                  // +0x9c
    CLandscapeBackgroundSubNodeBase* SubNode;                 // +0xa0
    CEngineLandscapeMap*             Map;                     // +0xa4
    C3DBoundingBox                   BoundingBox;             // +0xa8
    unsigned char                    CameraUpdateThreshold[0x10]; // +0xc0
    long                             OcclusionTestHandle;     // +0xd0
    long                             CreationFlags;           // +0xd4
    unsigned short                   MapX;                    // +0xd8
    unsigned short                   MapY;                    // +0xda
    unsigned short                   Width;                   // +0xdc
    unsigned short                   Height;                  // +0xde
    unsigned char                    FirstBand;               // +0xe0
    unsigned char                    FirstNonSplitBand;       // +0xe1
    unsigned char                    LastBand;                // +0xe2
    bool                             TesselationValid;        // +0xe3
    bool                             VisibleLastFrame;        // +0xe4
    bool                             VisibleLastFrameHasBeenUpdated; // +0xe5
    bool                             HasSubPatchLoadInfo;     // +0xe6
    unsigned char                    _pad0e7;
    CLandscapeBackgroundPatch* GetPatch()                     // 0x00BF6130 (inline here)
    {
        if (SubNode && SubNode->IsPatch())
            return (CLandscapeBackgroundPatch*)SubNode;
        return 0;
    }
    CLandscapeBackgroundSubNodeSplitBranch* GetSplitBranch()  // 0x00BF6160 (inline here)
    {
        if (SubNode && !SubNode->IsPatch())
            return (CLandscapeBackgroundSubNodeSplitBranch*)SubNode;
        return 0;
    }
    void MarkTreeAsInvisibleLastFrame();         // 0x00BF6A30
    void Render(unsigned long flags);
};
struct CLandscapeBackgroundPatch : CLandscapeBackgroundSubNodeBase {
    void Render(bool force, long* count, bool* changed);   // 0x00BE7BE0 (manifest parameter types)
    unsigned char _pad04[0x114];
    unsigned char LODBand;                         // +0x118
};
struct CLandscapeBackgroundSubNodeSplitBranch : CLandscapeBackgroundSubNodeBase {
    CLandscapeBackgroundTreeNode LeftBranch;       // +0x04
    CLandscapeBackgroundTreeNode RightBranch;      // +0xec
};


void CLandscapeBackgroundTreeNode::Render(unsigned long flags)
{
    bool visible = true;
    for (long i = 0; i < 4; i++) {
        const CPlane& plane = g_pEngineCamera->FrustrumPlanes[i];
        float nx = plane.Normal.X;
        float ny = plane.Normal.Y;
        float nz = plane.Normal.Z;
        float x = nx > 0.0f ? BoundingBox.Min.X : BoundingBox.Max.X;
        float y = ny > 0.0f ? BoundingBox.Min.Y : BoundingBox.Max.Y;
        float z = nz > 0.0f ? BoundingBox.Min.Z : BoundingBox.Max.Z;
        if (nx * x + ny * y + nz * z > plane.D) {
            visible = false;
            break;
        }
    }
    if (visible) {
        CLandscapeBackgroundPatch* patch = GetPatch();
        CLandscapeBackgroundSubNodeSplitBranch* branch = GetSplitBranch();
        if (patch) {
            long count = 0;
            bool changed = false;
            patch->Render(false, &count, &changed);
        } else if (branch) {
            branch->LeftBranch.Render(0);
            branch->RightBranch.Render(0);
            if (!branch->LeftBranch.VisibleLastFrame && !branch->RightBranch.VisibleLastFrame) {
                if (branch->LeftBranch.VisibleLastFrameHasBeenUpdated && branch->RightBranch.VisibleLastFrameHasBeenUpdated) {
                    VisibleLastFrame = false;
                    VisibleLastFrameHasBeenUpdated = true;
                }
                return;
            }
        }
        VisibleLastFrame = true;
        VisibleLastFrameHasBeenUpdated = true;
    } else if (VisibleLastFrame || !VisibleLastFrameHasBeenUpdated)
        MarkTreeAsInvisibleLastFrame();
}