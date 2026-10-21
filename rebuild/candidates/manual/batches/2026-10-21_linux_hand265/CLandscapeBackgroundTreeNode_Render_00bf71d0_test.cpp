#include <cstdio>
// Behaviour test for CLandscapeBackgroundTreeNode::Render (0x00BF71D0): frustum cull by the normal-signed box corner,
// patch render, branch recursion and the visible / visible-updated bookkeeping.
struct CPlane { float X, Y, Z, D; };
static CPlane g_planes[4];
struct CNode;
struct CBranch;
struct CNode {
    float MinX, MinY, MinZ, MaxX, MaxY, MaxZ;
    bool IsPatch; bool HasSub; CBranch* Branch; int PatchRenders; int Marks;
    bool Visible, Updated;
    void Mark() { Marks++; Visible = false; Updated = true; }
    void Render(unsigned long flags);
};
struct CBranch { CNode Left, Right; };
void CNode::Render(unsigned long)
{
    bool visible = true;
    for (long i = 0; i < 4; i++) {
        const CPlane& p = g_planes[i];
        float nx = p.X, ny = p.Y, nz = p.Z;
        float x = nx > 0.0f ? MinX : MaxX;
        float y = ny > 0.0f ? MinY : MaxY;
        float z = nz > 0.0f ? MinZ : MaxZ;
        if (nx * x + ny * y + nz * z > p.D) { visible = false; break; }
    }
    if (visible) {
        if (HasSub && IsPatch) {
            PatchRenders++;
        } else if (HasSub && !IsPatch) {
            Branch->Left.Render(0);
            Branch->Right.Render(0);
            if (!Branch->Left.Visible && !Branch->Right.Visible) {
                if (Branch->Left.Updated && Branch->Right.Updated) { Visible = false; Updated = true; }
                return;
            }
        }
        Visible = true; Updated = true;
    } else if (Visible || !Updated)
        Mark();
}
static int Fail(int n) { std::printf("00bf71d0_TEST FAIL %d\n", n); return 1; }
int main()
{
    for (int i = 0; i < 4; i++) { g_planes[i].X = 0; g_planes[i].Y = 0; g_planes[i].Z = 0; g_planes[i].D = 0; }
    g_planes[0].X = 1.0f; g_planes[0].D = 5.0f;          // culls boxes whose min x exceeds 5
    CNode leaf = { 0, 0, 0, 1, 1, 1, true, true, 0, 0, 0, false, false };
    leaf.Render(0);
    if (leaf.PatchRenders != 1 || !leaf.Visible || !leaf.Updated) return Fail(1);
    leaf.MinX = 6; leaf.MaxX = 7;
    leaf.Render(0);
    if (leaf.Marks != 1 || leaf.Visible || leaf.PatchRenders != 1) return Fail(2);
    leaf.Render(0);
    if (leaf.Marks != 1) return Fail(3);
    CBranch br = { { 6, 0, 0, 7, 1, 1, true, true, 0, 0, 0, false, true }, { 6, 0, 0, 7, 1, 1, true, true, 0, 0, 0, false, true } };
    CNode root = { 0, 0, 0, 7, 1, 1, false, true, &br, 0, 0, true, false };
    root.Render(0);
    if (root.Visible || !root.Updated) return Fail(4);
    br.Left.MinX = 0;
    root.Visible = false; root.Updated = false;
    root.Render(0);
    if (!root.Visible || !root.Updated || br.Left.PatchRenders != 1) return Fail(5);
    br.Left.MinX = 6; br.Left.Updated = false; br.Left.Visible = false; br.Right.Updated = false;
    root.Visible = true; root.Updated = false;
    root.Render(0);
    if (!br.Left.Updated || br.Left.Marks != 1 || root.Visible || !root.Updated) return Fail(6);
    std::printf("00bf71d0_TEST PASS\n");
    return 0;
}
