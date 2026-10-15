// Behaviour test for 0x006FEC30: CReactionMatch_AsymmetricPyramid::TransferBinaryOut writes 3 longs then 6 floats in member order.
#include <cstdio>
struct Stream { int n; long l[16]; float f[16]; int kind[16];
  void WriteLong(long v) { kind[n] = 0; l[n++] = v; } void WriteFloat(float v) { kind[n] = 1; f[n++] = v; } };
struct M { long ReactionType, XAxisOpinionType, YAxisOpinionType; float XCentre, NegInvXRadius, PosInvXRadius, YCentre, NegInvYRadius, PosInvYRadius;
  void TransferBinaryOut(Stream& s) const { s.WriteLong(ReactionType); s.WriteLong(XAxisOpinionType); s.WriteLong(YAxisOpinionType);
    s.WriteFloat(XCentre); s.WriteFloat(NegInvXRadius); s.WriteFloat(PosInvXRadius); s.WriteFloat(YCentre); s.WriteFloat(NegInvYRadius); s.WriteFloat(PosInvYRadius); } };
int main()
{
    M m = {7, 2, 3, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
    Stream s; s.n = 0; m.TransferBinaryOut(s);
    bool ok = s.n == 9 && s.l[0] == 7 && s.l[1] == 2 && s.l[2] == 3 && s.kind[3] == 1 && s.f[3] == 1.0f && s.f[8] == 6.0f;
    if (!ok) { std::printf("006fec30_TEST FAIL\n"); return 1; }
    std::printf("006fec30_TEST PASS\n");
    return 0;
}
