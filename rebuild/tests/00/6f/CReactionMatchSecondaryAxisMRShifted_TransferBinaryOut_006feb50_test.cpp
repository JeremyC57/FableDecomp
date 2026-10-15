// Behaviour test for 0x006feb50: secondary-axis reaction match TransferBinaryOut writes 2 longs then 6 floats in member order.
#include <cstdio>
struct Stream { int n; long l[16]; float f[16]; int kind[16];
  void WriteLong(long v) { kind[n] = 0; l[n++] = v; } void WriteFloat(float v) { kind[n] = 1; f[n++] = v; } };
struct M { long ReactionType, Axis; float A, B, MShiftZero, MShiftWeight, RShiftZero, RShiftWeight;
  void TransferBinaryOut(Stream& s) const { s.WriteLong(ReactionType); s.WriteLong(Axis); s.WriteFloat(A); s.WriteFloat(B);
    s.WriteFloat(MShiftZero); s.WriteFloat(MShiftWeight); s.WriteFloat(RShiftZero); s.WriteFloat(RShiftWeight); } };
int main()
{
    M m = {5, 1, 1.5f, 2.5f, 3.5f, 4.5f, 5.5f, 6.5f};
    Stream s; s.n = 0; m.TransferBinaryOut(s);
    bool ok = s.n == 8 && s.l[0] == 5 && s.l[1] == 1 && s.kind[2] == 1 && s.f[2] == 1.5f && s.f[7] == 6.5f;
    if (!ok) { std::printf("006feb50_TEST FAIL\n"); return 1; }
    std::printf("006feb50_TEST PASS\n");
    return 0;
}