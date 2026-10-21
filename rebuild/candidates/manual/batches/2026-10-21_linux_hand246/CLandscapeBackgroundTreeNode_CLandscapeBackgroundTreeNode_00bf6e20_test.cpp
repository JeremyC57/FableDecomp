// Behaviour test for CLandscapeBackgroundTreeNode::CLandscapeBackgroundTreeNode @ 0x00BF6E20 (copy of the model).
#include <cstdio>
#include <cstring>
struct C3DVector { float X, Y, Z; C3DVector() {} C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} };
struct C3DBoundingSphere {
    C3DVector Center; float Radius;
    C3DBoundingSphere() : Center(0.0f, 0.0f, 0.0f), Radius(0.0f) {}
    void Set(const C3DVector& c, float r) { Center = c; Radius = r; }
};
struct CLODLoadInfo { long FileBlockPos, FileBlockSize, OffsetIntoFileBlock; bool Loaded; CLODLoadInfo() : FileBlockPos(0), FileBlockSize(0), OffsetIntoFileBlock(0), Loaded(false) {} };
struct CLandscapeBackgroundNodeBase { void* m_04; void* m_08; CLandscapeBackgroundNodeBase() : m_04(0), m_08(0) {} };
struct CLandscapeBackgroundTreeNode : CLandscapeBackgroundNodeBase {
    CLODLoadInfo LODLoadInfo[8]; long FileBlockPos, FileBlockSize, OffsetIntoFileBlock; void* AsyncLoadTask;
    CLandscapeBackgroundTreeNode* Parent; void* SubNode; void* Map; C3DBoundingSphere CameraUpdateThreshold;
    long OcclusionTestHandle, CreationFlags; unsigned short MapX, MapY, Width, Height;
    unsigned char FirstBand, FirstNonSplitBand, LastBand; bool TesselationValid, VisibleLastFrame, VisibleLastFrameHasBeenUpdated, HasSubPatchLoadInfo;
    CLandscapeBackgroundTreeNode(void* map, CLandscapeBackgroundTreeNode* parent, long creationFlags)
    {
        CreationFlags = creationFlags; Parent = parent; Map = map; MapX = 0; MapY = 0; Width = 0; Height = 0; SubNode = 0;
        FirstBand = 8; FirstNonSplitBand = 8; LastBand = 8; TesselationValid = false;
        FileBlockPos = 0; FileBlockSize = 0; OffsetIntoFileBlock = 0; AsyncLoadTask = 0;
        CameraUpdateThreshold.Set(C3DVector(0.0f, 0.0f, 0.0f), 0.0f);
        VisibleLastFrame = false; VisibleLastFrameHasBeenUpdated = false; OcclusionTestHandle = -1; HasSubPatchLoadInfo = false;
    }
};
int main()
{
    int bad = 0; int mapToken = 0;
    unsigned char raw[sizeof(CLandscapeBackgroundTreeNode)]; std::memset(raw, 0xcd, sizeof(raw));
    CLandscapeBackgroundTreeNode* parent = (CLandscapeBackgroundTreeNode*)raw;
    CLandscapeBackgroundTreeNode n(&mapToken, parent, 0x55);
    if (n.Map != &mapToken || n.Parent != parent || n.CreationFlags != 0x55 || n.OcclusionTestHandle != -1) bad++;
    if (n.FirstBand != 8 || n.FirstNonSplitBand != 8 || n.LastBand != 8 || n.TesselationValid || n.VisibleLastFrame || n.HasSubPatchLoadInfo) bad++;
    if (n.MapX || n.MapY || n.Width || n.Height || n.SubNode || n.AsyncLoadTask || n.FileBlockPos || n.FileBlockSize || n.OffsetIntoFileBlock || n.m_04 || n.m_08) bad++;
    for (int i = 0; i < 8; ++i) if (n.LODLoadInfo[i].FileBlockPos || n.LODLoadInfo[i].Loaded) bad++;
    if (n.CameraUpdateThreshold.Radius != 0.0f || n.CameraUpdateThreshold.Center.Z != 0.0f) bad++;
    if (bad) { std::printf("00bf6e20_TEST FAIL\n"); return 1; }
    std::printf("00bf6e20_TEST PASS\n");
    return 0;
}
