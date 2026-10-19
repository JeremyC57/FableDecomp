#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#endif
// Behaviour test: the model and logic below are a copy of the src/ file; the callees are test doubles.
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

// ---- test doubles and driver ----
static int g_rendered[16]; static int g_n = 0;
static CEngineLandscapePatch* g_base = 0;
void CEngineLandscapePatch::RenderForeground() { if (g_n < 16) g_rendered[g_n++] = (int)(this - g_base); }
static CEngineCamera g_camera;
CEngineCamera* g_pEngineCamera = &g_camera;
static void SetPlane(int i, float x, float y, float z, float d) { CPlane& p = g_camera.FrustrumPlanes[i]; p.Normal.X = x; p.Normal.Y = y; p.Normal.Z = z; p.D = d; }
int main()
{
    static CEngineLandscapePatch patches[6]; g_base = patches;
    static CLandscapeBackgroundTreeNode tree;
    tree.BoundsMin.X = 0; tree.BoundsMin.Y = 0; tree.BoundsMin.Z = 0; tree.BoundsMax.X = 10; tree.BoundsMax.Y = 10; tree.BoundsMax.Z = 10;
    CEngineLandscapeMap m; m.PatchTree = &tree; m.Patches = patches; m.PatchGridWidth = 3; m.PatchGridHeight = 2;
    // all planes keep the box: render 2x3 patches in row order
    for (int i = 0; i < 4; ++i) SetPlane(i, 1.0f, 0.0f, 0.0f, 100.0f);
    m.RenderForeground();
    bool ok = g_n == 6;
    for (int i = 0; ok && i < 6; ++i) ok = g_rendered[i] == i;
    // plane 2: x > 5 is outside, nearest corner x = 0 is inside -> still rendered; plane 3: -x > -20 (x < 20 outside) -> nearest corner x = 10 -> culled
    g_n = 0; SetPlane(2, 1.0f, 0.0f, 0.0f, 5.0f); SetPlane(3, -1.0f, 0.0f, 0.0f, -20.0f);
    m.RenderForeground();
    ok = ok && g_n == 0;
    // no tree: no culling
    g_n = 0; m.PatchTree = 0; m.RenderForeground();
    ok = ok && g_n == 6;
    // no patches: nothing
    g_n = 0; m.Patches = 0; m.RenderForeground();
    ok = ok && g_n == 0;
    if (!ok) { std::printf("00bdc2d0_TEST FAIL\n"); return 1; }
    std::printf("00bdc2d0_TEST PASS\n");
    return 0;
}