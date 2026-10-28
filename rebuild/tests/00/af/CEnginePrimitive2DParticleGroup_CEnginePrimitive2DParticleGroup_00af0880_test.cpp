// Behaviour test for CEnginePrimitive2DParticleGroup::CEnginePrimitive2DParticleGroup (0x00AF0880): type 0x20, interpolation set 0, channel bytes 0, then the arguments.
#include <cstdio>
struct Desc { unsigned char C0, C1, Blend, Pad; Desc() : C0(0), C1(0), Blend(0) {} };
struct Prim { int Type; int InterpolationSet; Desc ThemeChannelDesc; Prim(int t) : Type(t), InterpolationSet(0) {} };
struct M { float e[12]; };
struct V2 { float X, Y; };
struct Group : Prim { int Layer; bool ScreenDisplacement; bool ReadZBuffer; M InverseTransform; V2 Position;
    Group(int layer, const M& m, const V2& p, bool sd, bool rz) : Prim(0x20), Layer(layer), ScreenDisplacement(sd), ReadZBuffer(rz), InverseTransform(m), Position(p) {} };
int main()
{
    M m; for (int i = 0; i < 12; ++i) m.e[i] = (float)i; V2 p; p.X = 3.0f; p.Y = -4.0f;
    Group g(5, m, p, true, false);
    if (g.Type != 0x20 || g.InterpolationSet != 0 || g.ThemeChannelDesc.C0 != 0 || g.ThemeChannelDesc.Blend != 0 || g.Layer != 5 ||
        !g.ScreenDisplacement || g.ReadZBuffer || g.InverseTransform.e[11] != 11.0f || g.Position.X != 3.0f || g.Position.Y != -4.0f) {
        std::printf("00af0880_TEST FAIL\n"); return 1; }
    std::printf("00af0880_TEST PASS\n");
    return 0;
}