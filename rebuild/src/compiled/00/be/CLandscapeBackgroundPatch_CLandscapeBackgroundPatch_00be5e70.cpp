// CLandscapeBackgroundPatch::CLandscapeBackgroundPatch(CLandscapeBackgroundTreeNode* parent, float minZ, float maxZ, long mapX, long mapY, long width, long height, long lodBand, long textureWidthNeeded, long textureHeightNeeded, const CSubPatchTreeLODLockElement* uvLockTreeU, const CSubPatchTreeLODLockElement* uvLockTreeV, long lockedGridSpacingX, long lockedGridSpacingY, long waterOnlyPatch) -- background landscape patch: store the vtable, construct the four edge strips, copy the placement / LOD / texture-size parameters, clear the buffers and the bridge arrays, no Z-sorter handle (-1)
// Leaf: ??0CLandscapeBackgroundPatch@@QAE@PAVCLandscapeBackgroundTreeNode@@MM
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BE5E70, 302 bytes, thiscall `ret 3Ch` (15 argument words).
// Identity: Ego_r.pdb ??0CLandscapeBackgroundPatch@@QAE@PAVCLandscapeBackgroundTreeNode@@MMJJJJJJJPBVCSubPatchTreeLODLockElement@@1JJJ@Z
//   (the other ctor takes the parent only); stores the CLandscapeBackgroundPatch vtable 0x012A803C (rtti-vtables.tsv).
// Callees: CPatchTesselationEdgeStrip::CPatchTesselationEdgeStrip() 0x00BFC370 (the four-element member array, `dec ebx` loop).
// Layout: engine/CLandscapeBackgroundPatch.h has the PDB members but flattens the vptr into a data word (and its EdgeStrips is an
//   opaque byte array), so the class is modelled locally with the same offsets and a virtual destructor; the PDB parameters are
//   longs truncated into the ushort / uchar members (`mov dx,word ptr [esp+x]`), the lock trees const pointers.
// Shape (what matched): retail's store order is VC7.1's pairwise schedule of this body order: Parent, Texture, VertexBuffer,
//   IndexBuffer, Width, Height, MapX, MapY, LODBand, TextureWidthNeeded, TextureHeightNeeded, UVLockTree U / V,
//   LockedGridSpacing X / Y, MinZ, MaxZ, SharedIndexBuffer, WaterOnlyPatch, then the bridge loop, WaterSubPatch,
//   IsDefaultSection, ZSorterHandle. Consecutive parameter copies come out swapped in pairs (Height Width MapY MapX ...), the
//   parent is loaded first and stored after MinZ, the zero stores sink below the copies. The parameter-order body and the
//   member-declaration-order body give other permutations (and an initializer list 308 bytes).
class CLandscapeBackgroundTreeNode;
class CTexture;
class CIndexBuffer;
class CVertexBufferWin32;
class CSubPatchTreeLODLockElement;
class CEngineWaterBackgroundSubPatch;
class CPatchTesselationEdgeStrip {                 // 0x2c bytes (Ego_r.pdb)
public:
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
    const CSubPatchTreeLODLockElement* UVLockTreeU;             // +0xec
    const CSubPatchTreeLODLockElement* UVLockTreeV;             // +0xf0
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
                              long mapX, long mapY, long width, long height,
                              long lodBand, long textureWidthNeeded, long textureHeightNeeded,
                              const CSubPatchTreeLODLockElement* uvLockTreeU, const CSubPatchTreeLODLockElement* uvLockTreeV,
                              long lockedGridSpacingX, long lockedGridSpacingY, long waterOnly);
};

CLandscapeBackgroundPatch::CLandscapeBackgroundPatch(CLandscapeBackgroundTreeNode* parent, float minZ, float maxZ,
                              long mapX, long mapY, long width, long height,
                              long lodBand, long textureWidthNeeded, long textureHeightNeeded,
                              const CSubPatchTreeLODLockElement* uvLockTreeU, const CSubPatchTreeLODLockElement* uvLockTreeV,
                              long lockedGridSpacingX, long lockedGridSpacingY, long waterOnly)
{
    Parent = parent;
    Texture = 0;
    VertexBuffer = 0;
    IndexBuffer = 0;
    Width = width;
    Height = height;
    MapX = mapX;
    MapY = mapY;
    LODBand = lodBand;
    TextureWidthNeeded = textureWidthNeeded;
    TextureHeightNeeded = textureHeightNeeded;
    UVLockTreeU = uvLockTreeU;
    UVLockTreeV = uvLockTreeV;
    LockedGridSpacingX = lockedGridSpacingX;
    LockedGridSpacingY = lockedGridSpacingY;
    MinZ = minZ;
    MaxZ = maxZ;
    SharedIndexBuffer = false;
    WaterOnlyPatch = (waterOnly & 1) != 0;
    for (long i = 0; i < 4; ++i) {
        BridgeIndexBuffers[i] = 0;
        BridgeVertexBuffers[i] = 0;
    }
    WaterSubPatch = 0;
    IsDefaultSection = false;
    ZSorterHandle = -1;
}