// CLandscapeBackgroundPatch::CLandscapeBackgroundPatch(...) -- background landscape patch ctor
// Grade: DIFFER(ours v retail) -- store scheduling only: the 19 argument/zero stores after the edge-strip loop come out in another order (retail: Height, Width, MapY, MapX, TextureWidthNeeded, LODBand, UVLockTreeU, ...)
struct CLandscapeBackgroundTreeNode;
struct CTexture;
struct CIndexBuffer;
struct CVertexBufferWin32;
struct CSubPatchTreeLODLockElement;
struct CEngineWaterBackgroundSubPatch;
struct CPatchTesselationEdgeStrip {                // 0x2c bytes
    unsigned char _body[0x2c];
    CPatchTesselationEdgeStrip();                  // 0x00BFC370
};
class CLandscapeBackgroundPatch {
public:
    virtual ~CLandscapeBackgroundPatch();
    CLandscapeBackgroundTreeNode*   Parent;                  // +0x04
    CPatchTesselationEdgeStrip      EdgeStrips[4];           // +0x08
    CTexture*                       Texture;                 // +0xb8
    CIndexBuffer*                   IndexBuffer;             // +0xbc
    CVertexBufferWin32*             VertexBuffer;            // +0xc0
    CIndexBuffer*                   BridgeIndexBuffers[4];   // +0xc4
    CVertexBufferWin32*             BridgeVertexBuffers[4];  // +0xd4
    unsigned short                  BridgePolyCount[4];      // +0xe4
    CSubPatchTreeLODLockElement*    UVLockTreeU;             // +0xec
    CSubPatchTreeLODLockElement*    UVLockTreeV;             // +0xf0
    CEngineWaterBackgroundSubPatch* WaterSubPatch;           // +0xf4
    float                           MinZ;                    // +0xf8
    float                           MaxZ;                    // +0xfc
    long                            ZSorterHandle;           // +0x100
    unsigned short                  VertexCount;             // +0x104
    unsigned short                  PolyCountIncDegenerates; // +0x106
    unsigned short                  TextureWidthNeeded;      // +0x108
    unsigned short                  TextureHeightNeeded;     // +0x10a
    unsigned short                  MapX;                    // +0x10c
    unsigned short                  MapY;                    // +0x10e
    unsigned short                  Width;                   // +0x110
    unsigned short                  Height;                  // +0x112
    unsigned short                  LockedGridSpacingX;      // +0x114
    unsigned short                  LockedGridSpacingY;      // +0x116
    unsigned char                   LODBand;                 // +0x118
    bool                            SharedIndexBuffer;       // +0x119
    bool                            WaterOnlyPatch;          // +0x11a
    bool                            IsDefaultSection;        // +0x11b
    CLandscapeBackgroundPatch(CLandscapeBackgroundTreeNode* parent, float minZ, float maxZ,
                              unsigned short mapX, unsigned short mapY, unsigned short width, unsigned short height,
                              unsigned char lodBand, unsigned short textureWidthNeeded, unsigned short textureHeightNeeded,
                              CSubPatchTreeLODLockElement* uvLockTreeU, CSubPatchTreeLODLockElement* uvLockTreeV,
                              unsigned short lockedGridSpacingX, unsigned short lockedGridSpacingY, unsigned long waterOnly);
};

CLandscapeBackgroundPatch::CLandscapeBackgroundPatch(CLandscapeBackgroundTreeNode* parent, float minZ, float maxZ,
                              unsigned short mapX, unsigned short mapY, unsigned short width, unsigned short height,
                              unsigned char lodBand, unsigned short textureWidthNeeded, unsigned short textureHeightNeeded,
                              CSubPatchTreeLODLockElement* uvLockTreeU, CSubPatchTreeLODLockElement* uvLockTreeV,
                              unsigned short lockedGridSpacingX, unsigned short lockedGridSpacingY, unsigned long waterOnly)
{
    Parent = parent;
    MinZ = minZ;
    MaxZ = maxZ;
    MapX = mapX;
    MapY = mapY;
    Width = width;
    Height = height;
    LODBand = lodBand;
    TextureWidthNeeded = textureWidthNeeded;
    TextureHeightNeeded = textureHeightNeeded;
    UVLockTreeU = uvLockTreeU;
    UVLockTreeV = uvLockTreeV;
    LockedGridSpacingX = lockedGridSpacingX;
    LockedGridSpacingY = lockedGridSpacingY;
    WaterOnlyPatch = (waterOnly & 1) != 0;
    Texture = 0;
    IndexBuffer = 0;
    VertexBuffer = 0;
    SharedIndexBuffer = false;
    for (long i = 0; i < 4; ++i) {
        BridgeIndexBuffers[i] = 0;
        BridgeVertexBuffers[i] = 0;
    }
    WaterSubPatch = 0;
    IsDefaultSection = false;
    ZSorterHandle = -1;
}
