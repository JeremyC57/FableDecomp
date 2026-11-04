// Behaviour test for 0x00BCFB00 CEnginePrimitiveManagerAnimatedMeshesBase::RenderPrimitiveBlocksIndexBuffer(C3DPrimitive2&, const CMatrix4x4*): a model; checks each animated block gathers its bones into the palette (in its bone order) and draws its triangles (strips n-2, lists n/3).
#include <cstdio>
#include <cmath>
#include <vector>
#include <string>
#include <set>
#include <map>

struct Block { std::vector<int> bones; bool strip; int indices; };
static void Render(const std::vector<Block>& blocks, const float* matrices, std::vector<float>& palette, int& triangles)
{
    triangles = 0;
    for (size_t b = 0; b < blocks.size(); ++b) {
        palette.clear(); for (size_t i = 0; i < blocks[b].bones.size(); ++i) palette.push_back(matrices[blocks[b].bones[i]]);
        triangles += blocks[b].strip ? blocks[b].indices - 2 : blocks[b].indices / 3;
    }
}
int main()
{
    int ok = 1, tris; float m[4] = { 10, 11, 12, 13 }; std::vector<float> pal; std::vector<Block> bl(2);
    bl[0].bones.push_back(2); bl[0].bones.push_back(0); bl[0].strip = true; bl[0].indices = 6;
    bl[1].bones.push_back(3); bl[1].strip = false; bl[1].indices = 9;
    Render(bl, m, pal, tris); ok &= tris == 7 && pal.size() == 1 && pal[0] == 13;
    std::printf(ok ? "00BCFB00_TEST PASS\n" : "00BCFB00_TEST FAIL\n"); return ok ? 0 : 1;
}
