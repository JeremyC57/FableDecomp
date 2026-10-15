// Behaviour test for 0x00A8E5C0 C3DPrimitive2::CalculateTextureChangeFlags
#include <cstdio>
struct CStaticBlock { bool DegenerateTriangles; unsigned char ChangeFlags; long MaterialIndex; };
struct C3DMeshMaterial { unsigned long DiffuseMapBankIndex, BumpMapBankIndex; bool DegenerateTriangles; };
struct C3DMesh2 { C3DMeshMaterial* Materials; };
struct C3DPrimitive2 {
    unsigned long StaticBlockCount; CStaticBlock* StaticBlocks; unsigned long InitFlags;
    void CalculateTextureChangeFlags(const C3DMesh2* mesh)
    {
        for (unsigned long i = 1; i < StaticBlockCount; ++i) {
            const C3DMeshMaterial* materials = mesh->Materials;
            long previousIndex = StaticBlocks[i - 1].MaterialIndex;
            CStaticBlock& block = StaticBlocks[i];
            const C3DMeshMaterial& material = materials[block.MaterialIndex];
            const C3DMeshMaterial& previous = materials[previousIndex];
            if (material.DegenerateTriangles) block.DegenerateTriangles = true;
            else {
                if (material.DiffuseMapBankIndex != previous.DiffuseMapBankIndex) block.ChangeFlags |= 1;
                if (InitFlags & 2) { if (material.BumpMapBankIndex != previous.BumpMapBankIndex) block.ChangeFlags |= 2; }
            }
        }
    }
};
int main()
{
    C3DMeshMaterial mats[4] = { {1, 10, false}, {1, 11, false}, {2, 11, false}, {5, 5, true} };
    CStaticBlock blocks[5] = { {false, 0, 0}, {false, 0, 1}, {false, 0, 2}, {false, 0, 3}, {false, 0, 2} };
    C3DMesh2 mesh = { mats };
    C3DPrimitive2 p = { 5, blocks, 2 };
    p.CalculateTextureChangeFlags(&mesh);
    bool ok = blocks[0].ChangeFlags == 0 && blocks[1].ChangeFlags == 2 && blocks[2].ChangeFlags == 1
        && blocks[3].DegenerateTriangles && blocks[3].ChangeFlags == 0 && blocks[4].ChangeFlags == 3 && !blocks[4].DegenerateTriangles;
    CStaticBlock b2[2] = { {false, 0, 0}, {false, 0, 1} };
    C3DPrimitive2 q = { 2, b2, 0 };
    q.CalculateTextureChangeFlags(&mesh);
    ok = ok && b2[1].ChangeFlags == 0;
    if (!ok) { std::printf("00a8e5c0_TEST FAIL\n"); return 1; }
    std::printf("00a8e5c0_TEST PASS\n");
    return 0;
}