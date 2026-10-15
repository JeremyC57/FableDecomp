// CLandscapeBackgroundTreeNode::OnPostRelocation() -- after the node moved, re-register it as the owner of its sub node, its load task and the task's request object with the landscape memory manager, and tell the sub node and the request object their parent moved
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF6B70, length 128 bytes.
// Callees: CEngineLandscapeMemoryManager @ 0x00BD9AB0 (block, owner) through engine 0x01436EA8 -> +0x6b0,
//          OnPostParentRelocation (virtual slot 8, +0x20) on the sub node and on the task's request object.
// Shape: the manager is re-read from the engine global for every call (no local).
// Retail CLandscapeBackgroundTreeNode is 0xE8 bytes: every member from LODLoadInfo on sits 8 bytes below the
// Ego_r.pdb offsets (two containers ahead of it are 4 bytes smaller in the shipped build), so a retail-offset
// model is declared here instead of engine/CLandscapeBackgroundTreeNode.h.
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
    unsigned char                    BoundingBox[0x18];       // +0xa8
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
    void OnPostRelocation();
};
struct CLandscapeBackgroundPatch : CLandscapeBackgroundSubNodeBase {
    unsigned char _pad04[0x114];
    unsigned char LODBand;                         // +0x118
};
struct CLandscapeBackgroundSubNodeSplitBranch : CLandscapeBackgroundSubNodeBase {
    CLandscapeBackgroundTreeNode LeftBranch;       // +0x04
    CLandscapeBackgroundTreeNode RightBranch;      // +0xec
};

void CLandscapeBackgroundTreeNode::OnPostRelocation()
{
    if (SubNode)
    {
        g_pEngine_01436ea8->m_pLandscapeMemory->RebindBlockOwner(SubNode, this);
        SubNode->OnPostParentRelocation(this);
    }
    if (AsyncLoadTask)
    {
        g_pEngine_01436ea8->m_pLandscapeMemory->RebindBlockOwner(AsyncLoadTask, this);
        if (AsyncLoadTask->m_pOwner)
        {
            g_pEngine_01436ea8->m_pLandscapeMemory->RebindBlockOwner(AsyncLoadTask->m_pOwner, this);
            AsyncLoadTask->m_pOwner->OnPostParentRelocation(this);
        }
    }
}