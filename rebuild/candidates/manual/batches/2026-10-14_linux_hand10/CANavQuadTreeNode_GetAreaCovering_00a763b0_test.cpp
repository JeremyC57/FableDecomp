#include <cstdio>
// Model of CANavQuadTreeNode::GetAreaCovering @ 0x00A763B0: position +/- half the level's cell size.
float g_quadTreeLevelCellSize[4] = { 64.0f, 32.0f, 16.0f, 8.0f };
struct C2DBoxF { float MinX, MinY, MaxX, MaxY; C2DBoxF(float a, float b, float c, float d) : MinX(a), MinY(b), MaxX(c), MaxY(d) {} };
struct Node {
    float Position_X, Position_Y; unsigned char QuadTreeLevel;
    C2DBoxF GetAreaCovering() const { float half = g_quadTreeLevelCellSize[QuadTreeLevel] * 0.5f; return C2DBoxF(Position_X - half, Position_Y - half, Position_X + half, Position_Y + half); }
};
int main()
{
    Node n = { 10.0f, 20.0f, 2 }; C2DBoxF b = n.GetAreaCovering();
    if (b.MinX != 2.0f || b.MinY != 12.0f || b.MaxX != 18.0f || b.MaxY != 28.0f) { std::printf("00a763b0_TEST FAIL\n"); return 1; }
    n.QuadTreeLevel = 0; b = n.GetAreaCovering();
    if (b.MinX != -22.0f || b.MaxY != 52.0f) { std::printf("00a763b0_TEST FAIL\n"); return 1; }
    std::printf("00a763b0_TEST PASS\n"); return 0;
}
