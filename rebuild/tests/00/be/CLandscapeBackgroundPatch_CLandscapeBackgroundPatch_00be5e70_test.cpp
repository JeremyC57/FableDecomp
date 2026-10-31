// Behaviour test for CLandscapeBackgroundPatch::CLandscapeBackgroundPatch @ 0x00BE5E70: the member copies / clears of the ctor.
#include <cstdio>

static int g_stripCtors = 0;
class CPatchTesselationEdgeStrip { public: unsigned char _body[0x2c]; CPatchTesselationEdgeStrip() { ++g_stripCtors; } };
class CLandscapeBackgroundTreeNode { public: int dummy; };
class CSubPatchTreeLODLockElement { public: int dummy; };

class CLandscapeBackgroundPatch {
public:
    virtual ~CLandscapeBackgroundPatch() { }
    CLandscapeBackgroundTreeNode* Parent;
    CPatchTesselationEdgeStrip EdgeStrips[4];
    void* Texture; void* IndexBuffer; void* VertexBuffer;
    void* BridgeIndexBuffers[4]; void* BridgeVertexBuffers[4];
    unsigned short BridgePolyCount[4];
    const CSubPatchTreeLODLockElement* UVLockTreeU; const CSubPatchTreeLODLockElement* UVLockTreeV;
    void* WaterSubPatch;
    float MinZ, MaxZ;
    long ZSorterHandle;
    unsigned short VertexCount, PolyCountIncDegenerates, TextureWidthNeeded, TextureHeightNeeded, MapX, MapY, Width, Height,
                   LockedGridSpacingX, LockedGridSpacingY;
    unsigned char LODBand;
    bool SharedIndexBuffer, WaterOnlyPatch, IsDefaultSection;
    CLandscapeBackgroundPatch(CLandscapeBackgroundTreeNode* parent, float minZ, float maxZ, long mapX, long mapY, long width,
                              long height, long lodBand, long textureWidthNeeded, long textureHeightNeeded,
                              const CSubPatchTreeLODLockElement* uvLockTreeU, const CSubPatchTreeLODLockElement* uvLockTreeV,
                              long lockedGridSpacingX, long lockedGridSpacingY, long waterOnlyPatch)
    {
        Parent = parent;
        Texture = 0; VertexBuffer = 0; IndexBuffer = 0;
        Width = (unsigned short)width; Height = (unsigned short)height; MapX = (unsigned short)mapX; MapY = (unsigned short)mapY;
        LODBand = (unsigned char)lodBand;
        TextureWidthNeeded = (unsigned short)textureWidthNeeded; TextureHeightNeeded = (unsigned short)textureHeightNeeded;
        UVLockTreeU = uvLockTreeU; UVLockTreeV = uvLockTreeV;
        LockedGridSpacingX = (unsigned short)lockedGridSpacingX; LockedGridSpacingY = (unsigned short)lockedGridSpacingY;
        MinZ = minZ; MaxZ = maxZ;
        SharedIndexBuffer = false;
        WaterOnlyPatch = (waterOnlyPatch & 1) != 0;
        for (long i = 0; i < 4; ++i) { BridgeIndexBuffers[i] = 0; BridgeVertexBuffers[i] = 0; }
        WaterSubPatch = 0;
        IsDefaultSection = false;
        ZSorterHandle = -1;
    }
};

int main()
{
    CLandscapeBackgroundTreeNode node; CSubPatchTreeLODLockElement u, v;
    CLandscapeBackgroundPatch p(&node, -2.0f, 7.5f, 0x10010, 32, 64, 65, 0x103, 128, 256, &u, &v, 4, 8, 2);
    bool ok = g_stripCtors == 4 && p.Parent == &node && p.MinZ == -2.0f && p.MaxZ == 7.5f && p.MapX == 0x10 && p.MapY == 32 &&
              p.Width == 64 && p.Height == 65 && p.LODBand == 3 && p.TextureWidthNeeded == 128 && p.TextureHeightNeeded == 256 &&
              p.UVLockTreeU == &u && p.UVLockTreeV == &v && p.LockedGridSpacingX == 4 && p.LockedGridSpacingY == 8 &&
              !p.WaterOnlyPatch && p.Texture == 0 && p.IndexBuffer == 0 && p.VertexBuffer == 0 && !p.SharedIndexBuffer &&
              p.BridgeIndexBuffers[3] == 0 && p.BridgeVertexBuffers[0] == 0 && p.WaterSubPatch == 0 && !p.IsDefaultSection &&
              p.ZSorterHandle == -1;
    CLandscapeBackgroundPatch q(&node, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3);
    ok = ok && q.WaterOnlyPatch;
    if (!ok) { std::printf("00be5e70_TEST FAIL\n"); return 1; }
    std::printf("00be5e70_TEST PASS\n");
    return 0;
}