// CLandscapeBackgroundPatch::CLandscapeBackgroundPatch(CLandscapeBackgroundTreeNode* parent) -- construct an empty landscape patch: four edge strips, no buffers or texture, z-sorter handle -1
// Leaf: ??0CLandscapeBackgroundPatch
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE5FA0, length 229 bytes (`ret 4`), vtable 0x012A803C.
// Callees: CPatchTesselationEdgeStrip::CPatchTesselationEdgeStrip() @ 0x00BFC370 (labelled CManagedTextureData), once per strip.
// Layout: engine/CLandscapeBackgroundPatch.h offsets (they agree with retail), modelled locally with the same member names because the
// generated header's `void* __vftable` cannot carry the virtuals (same choice as the landed dtor 0x00BE6090).
// Shape: the vtable store precedes the member-array construction (a counted `dec ebx` loop: the strip has a ctor and no dtor in this TU);
// Parent and every zero are BODY assignments (stored after the strips, in source order, one zero register); WaterOnlyPatch is left alone;
// the bridge-buffer clearing loop is unrolled with the two arrays interleaved.
class CMovableResource { public:
    virtual ~CMovableResource();
};
class CTexture;
class CIndexBuffer;
class CVertexBufferWin32;
class CSubPatchTreeLODLockElement;
class CEngineWaterBackgroundSubPatch;
class CLandscapeBackgroundTreeNode;
class CLandscapeBackgroundPatch;
class CPatchTesselationEdgeStrip { public:                         // 0x2c bytes
    unsigned char _body[0x2c];
    CPatchTesselationEdgeStrip();                           // 0x00BFC370
};
class CLandscapeBackgroundPatch : public CMovableResource { public:
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
    CLandscapeBackgroundPatch(CLandscapeBackgroundTreeNode* parent);
    virtual ~CLandscapeBackgroundPatch();
};

CLandscapeBackgroundPatch::CLandscapeBackgroundPatch(CLandscapeBackgroundTreeNode* parent)
{
    Parent = parent;
    Texture = 0;
    VertexBuffer = 0;
    IndexBuffer = 0;
    TextureWidthNeeded = 0;
    TextureHeightNeeded = 0;
    Width = 0;
    Height = 0;
    MapX = 0;
    MapY = 0;
    LODBand = 0;
    UVLockTreeU = 0;
    UVLockTreeV = 0;
    LockedGridSpacingX = 0;
    LockedGridSpacingY = 0;
    MinZ = 0;
    MaxZ = 0;
    WaterSubPatch = 0;
    SharedIndexBuffer = false;
    ZSorterHandle = -1;
    for (long i = 0; i < 4; ++i) {
        BridgeIndexBuffers[i] = 0;
        BridgeVertexBuffers[i] = 0;
    }
    IsDefaultSection = false;
}