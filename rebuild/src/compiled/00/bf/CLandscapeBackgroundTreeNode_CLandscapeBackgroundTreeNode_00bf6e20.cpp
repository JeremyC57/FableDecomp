// CLandscapeBackgroundTreeNode::CLandscapeBackgroundTreeNode(CEngineLandscapeMap* map, CLandscapeBackgroundTreeNode* parent, long creationFlags) -- zero the node's LOD load table, file-block bookkeeping, map rectangle and flags, set the band range to 8/8/8, the occlusion handle to -1 and the camera-update sphere to the origin, and remember the map, parent and creation flags
// Leaf: ??0CLandscapeBackgroundTreeNode
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF6E20, length 349 bytes, `ret 0Ch`. Vtable 0x012A8240 (rtti-vtables.tsv).
// Layout: Ego_r.pdb CLandscapeBackgroundTreeNode (240 bytes) minus the 8 bytes of DynamicBuildTemplate/GPUUpdateTask, which
//   the retail build does not have (the eight 16-byte CLODLoadInfo records start at +0x0C), so every later member sits 8
//   bytes below engine/CLandscapeBackgroundTreeNode.h (Parent +0x9C, Map +0xA4, CameraUpdateThreshold +0xC0,
//   OcclusionTestHandle +0xD0, CreationFlags +0xD4, MapX..Height +0xD8..+0xDE, bands +0xE0..+0xE2, flags +0xE3..+0xE6);
//   modelled locally. The two words at +0x04/+0x08 belong to a vtable-less base whose constructor zeroes them before
//   the class's vtable store.
// Shape: member constructors (base words, the LOD records, the bounding sphere) in declaration order, then the body's
//   assignments in source order; the sphere's centre is re-assigned from a zero C3DVector temporary (its dead frame
//   stores are kept).
struct CEngineLandscapeMap;
struct CLandscapeBackgroundLoadTask;
struct CLandscapeBackgroundSubNodeBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct C3DBoundingBox {
    C3DVector Min;
    C3DVector Max;
    C3DBoundingBox() { }
};
struct C3DBoundingSphere {
    C3DVector Center;
    float     Radius;
    C3DBoundingSphere() : Center(0.0f, 0.0f, 0.0f), Radius(0.0f) { }
    void Set(const C3DVector& center, float radius) { Center = center; Radius = radius; }
};
struct CLODLoadInfo {                              // 16 bytes
    long FileBlockPos;
    long FileBlockSize;
    long OffsetIntoFileBlock;
    bool Loaded;
    CLODLoadInfo() : FileBlockPos(0), FileBlockSize(0), OffsetIntoFileBlock(0), Loaded(false) { }
};
struct __declspec(novtable) CLandscapeBackgroundNodeBase {
    void* m_04;                                    // +0x04
    void* m_08;                                    // +0x08
    CLandscapeBackgroundNodeBase() : m_04(0), m_08(0) { }
    virtual ~CLandscapeBackgroundNodeBase() { }
};
class CLandscapeBackgroundTreeNode : public CLandscapeBackgroundNodeBase {   // vtable 0x012A8240
public:
    CLODLoadInfo                     LODLoadInfo[8];          // +0x0c
    long                             FileBlockPos;            // +0x8c
    long                             FileBlockSize;           // +0x90
    long                             OffsetIntoFileBlock;     // +0x94
    CLandscapeBackgroundLoadTask*    AsyncLoadTask;           // +0x98
    CLandscapeBackgroundTreeNode*    Parent;                  // +0x9c
    CLandscapeBackgroundSubNodeBase* SubNode;                 // +0xa0
    CEngineLandscapeMap*             Map;                     // +0xa4
    C3DBoundingBox                   BoundingBox;             // +0xa8
    C3DBoundingSphere                CameraUpdateThreshold;   // +0xc0
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
    bool                             VisibleLastFrameHasBeenUpdated;   // +0xe5
    bool                             HasSubPatchLoadInfo;     // +0xe6
    CLandscapeBackgroundTreeNode(CEngineLandscapeMap* map, CLandscapeBackgroundTreeNode* parent, long creationFlags);
    virtual ~CLandscapeBackgroundTreeNode();
};

CLandscapeBackgroundTreeNode::CLandscapeBackgroundTreeNode(CEngineLandscapeMap* map, CLandscapeBackgroundTreeNode* parent, long creationFlags)
{
    CreationFlags = creationFlags;
    Parent = parent;
    Map = map;
    MapX = 0;
    MapY = 0;
    Width = 0;
    Height = 0;
    SubNode = 0;
    FirstBand = 8;
    FirstNonSplitBand = 8;
    LastBand = 8;
    TesselationValid = false;
    FileBlockPos = 0;
    FileBlockSize = 0;
    OffsetIntoFileBlock = 0;
    AsyncLoadTask = 0;
    CameraUpdateThreshold.Set(C3DVector(0.0f, 0.0f, 0.0f), 0.0f);
    VisibleLastFrame = false;
    VisibleLastFrameHasBeenUpdated = false;
    OcclusionTestHandle = -1;
    HasSubPatchLoadInfo = false;
}