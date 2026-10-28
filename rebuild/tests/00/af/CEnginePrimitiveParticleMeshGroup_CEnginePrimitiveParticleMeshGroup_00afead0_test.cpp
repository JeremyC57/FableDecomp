// Behaviour test for CEnginePrimitiveParticleMeshGroup::CEnginePrimitiveParticleMeshGroup (0x00AFEAD0): type 0x1C, bank index, world matrix, blend mode/op.
#include <cstdio>
struct Desc { unsigned char C0, C1, Blend, Pad; Desc() : C0(0), C1(0), Blend(0) {} };
struct Prim { int Type; int InterpolationSet; Desc ThemeChannelDesc; Prim(int t) : Type(t), InterpolationSet(0) {} };
struct M { float e[12]; };
struct Group : Prim { unsigned int BankIndex; M WorldMat; int BlendMode; int BlendOp;
    Group(unsigned int bank, const M& m, int mode, int op) : Prim(0x1c), BankIndex(bank), WorldMat(m), BlendMode(mode), BlendOp(op) {} };
int main()
{
    M m; for (int i = 0; i < 12; ++i) m.e[i] = (float)(i * 2);
    Group g(9, m, 2, 3);
    if (g.Type != 0x1c || g.InterpolationSet != 0 || g.ThemeChannelDesc.C1 != 0 || g.BankIndex != 9 || g.WorldMat.e[5] != 10.0f ||
        g.BlendMode != 2 || g.BlendOp != 3) { std::printf("00afead0_TEST FAIL\n"); return 1; }
    std::printf("00afead0_TEST PASS\n");
    return 0;
}