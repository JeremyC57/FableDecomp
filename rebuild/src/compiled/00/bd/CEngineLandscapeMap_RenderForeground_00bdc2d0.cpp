// CEngineLandscapeMap::RenderForeground() -- when the map has patches: skip the whole map if its patch tree's bounding box lies entirely outside one of the engine camera's four frustum side planes, otherwise render every patch's foreground row by row
// Leaf: RenderForeground
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BDC2D0, 237 bytes (+ int3 padding).
// Callees: CEngineLandscapePatch::RenderForeground 0x00BF4570 (on each 0x48-byte patch). Globals: the engine camera
//   0x01436EA0 (FrustrumPlanes +0x1c0, four CPlane {Normal, D} of 16 bytes; Ego_r.pdb agrees). Float pool 0x0122DEDC = 0.0f.
// Layout (retail, as the ctor 0x00BDD2F0): PatchTree +4, Patches +8, PatchGridWidth +0xc, PatchGridHeight +0x10; the tree node's
//   bounding box at +0xa8 (Min) / +0xb4 (Max).
// Shape: per plane the three normal components are loaded first, the box corner is picked per axis by `n > 0 ? Min : Max`
//   (`test ah,41h ; jne`), the dot product summed z, y, then x and compared with D (`dot > D` exits); the plane is an indexed
//   reference (`FrustrumPlanes[i]`: VC biases the strength-reduced pointer to +8; a walking plane pointer does not); the grid loops are
//   unsigned (`jb`) with the width re-read after each call.
struct C3DVector { float X, Y, Z; };
struct CPlane {
    C3DVector Normal;                                  // +0x00
    float     D;                                       // +0x0c
};
struct CEngineCamera {
    unsigned char _pad[0x1c0];
    CPlane        FrustrumPlanes[4];                   // +0x1c0
};
extern CEngineCamera* g_pEngineCamera;                 // 0x01436EA0

struct CLandscapeBackgroundTreeNode {
    unsigned char _pad[0xa8];
    C3DVector     BoundsMin;                           // +0xa8
    C3DVector     BoundsMax;                           // +0xb4
};

struct CEngineLandscapePatch {                         // 0x48 bytes
    unsigned char _body[0x48];
    void RenderForeground();                           // 0x00BF4570
};

struct CEngineLandscapeMap {
    void*                         __vftable;           // +0x00
    CLandscapeBackgroundTreeNode* PatchTree;           // +0x04
    CEngineLandscapePatch*        Patches;             // +0x08
    long                          PatchGridWidth;      // +0x0c
    long                          PatchGridHeight;     // +0x10
    void RenderForeground();
};

void CEngineLandscapeMap::RenderForeground()
{
    if (!Patches)
        return;
    if (PatchTree) {
        for (long i = 0; i < 4; ++i) {
            const CPlane& plane = g_pEngineCamera->FrustrumPlanes[i];
            float nx = plane.Normal.X;
            float ny = plane.Normal.Y;
            float nz = plane.Normal.Z;
            float x = nx > 0.0f ? PatchTree->BoundsMin.X : PatchTree->BoundsMax.X;
            float y = ny > 0.0f ? PatchTree->BoundsMin.Y : PatchTree->BoundsMax.Y;
            float z = nz > 0.0f ? PatchTree->BoundsMin.Z : PatchTree->BoundsMax.Z;
            if (z * nz + y * ny + x * nx > plane.D)
                return;
        }
    }
    for (unsigned long row = 0; row < (unsigned long)PatchGridHeight; ++row)
        for (unsigned long column = 0; column < (unsigned long)PatchGridWidth; ++column)
            Patches[row * PatchGridWidth + column].RenderForeground();
}