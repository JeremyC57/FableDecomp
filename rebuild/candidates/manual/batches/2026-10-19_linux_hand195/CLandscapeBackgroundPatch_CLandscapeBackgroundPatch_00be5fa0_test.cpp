#include <cstdio>
#include <cstring>
#include <new>
static int g_strips = 0;
struct CMovableResource { virtual ~CMovableResource() {} };
struct CPatchTesselationEdgeStrip { unsigned char _body[0x2c]; CPatchTesselationEdgeStrip() { ++g_strips; } };
struct CLandscapeBackgroundTreeNode { int x; };
struct CLandscapeBackgroundPatch : CMovableResource {
    CLandscapeBackgroundTreeNode* Parent;
    CPatchTesselationEdgeStrip EdgeStrips[4];
    void* Texture; void* IndexBuffer; void* VertexBuffer; void* BridgeIndexBuffers[4]; void* BridgeVertexBuffers[4];
    unsigned short BridgePolyCount[4]; void* UVLockTreeU; void* UVLockTreeV; void* WaterSubPatch; float MinZ, MaxZ; long ZSorterHandle;
    unsigned short VertexCount, PolyCountIncDegenerates, TextureWidthNeeded, TextureHeightNeeded, MapX, MapY, Width, Height, LockedGridSpacingX, LockedGridSpacingY;
    unsigned char LODBand; bool SharedIndexBuffer, WaterOnlyPatch, IsDefaultSection;
    CLandscapeBackgroundPatch(CLandscapeBackgroundTreeNode* parent);
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
int main()
{
    static unsigned char buf[sizeof(CLandscapeBackgroundPatch)];
    std::memset(buf, 0x5a, sizeof(buf));
    CLandscapeBackgroundTreeNode n;
    CLandscapeBackgroundPatch* p = new (buf) CLandscapeBackgroundPatch(&n);
    bool ok = g_strips == 4 && p->Parent == &n && p->Texture == 0 && p->IndexBuffer == 0 && p->VertexBuffer == 0 && p->ZSorterHandle == -1
        && p->MinZ == 0.0f && p->MaxZ == 0.0f && p->WaterSubPatch == 0 && !p->SharedIndexBuffer && !p->IsDefaultSection && p->LODBand == 0
        && p->MapX == 0 && p->Height == 0 && p->LockedGridSpacingY == 0 && p->UVLockTreeV == 0
        && *(unsigned char*)&p->WaterOnlyPatch == 0x5a && p->VertexCount == 0x5a5a;
    for (int i = 0; i < 4; ++i) ok = ok && p->BridgeIndexBuffers[i] == 0 && p->BridgeVertexBuffers[i] == 0 && p->BridgePolyCount[i] == 0x5a5a;
    if (!ok) { std::printf("00be5fa0_TEST FAIL\n"); return 1; }
    std::printf("00be5fa0_TEST PASS\n");
    return 0;
}
