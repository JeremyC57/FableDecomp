// Behaviour test for 0x007D0070: load mode reads both floats in order, save mode writes both, other modes only check CRCs.
#include <cstdio>
struct Ctx { int mode; int crcs; float in[2]; int nin; float out[2]; int nout;
  void Transfer(float& v) { ++crcs; if (mode == 2) v = in[nin++]; else if (mode == 3) out[nout++] = v; } };
struct Def { float Start, Score; void Transfer(Ctx& c) { c.Transfer(Start); c.Transfer(Score); } };
int main()
{
    Ctx c = {2, 0, {1.5f, 2.5f}, 0, {0, 0}, 0}; Def d = {0, 0}; d.Transfer(c);
    bool ok = d.Start == 1.5f && d.Score == 2.5f && c.crcs == 2;
    c.mode = 3; d.Transfer(c); ok = ok && c.out[0] == 1.5f && c.out[1] == 2.5f && c.crcs == 4;
    c.mode = 1; d.Transfer(c); ok = ok && c.crcs == 6 && c.nout == 2;
    if (!ok) { std::printf("007d0070_TEST FAIL\n"); return 1; }
    std::printf("007d0070_TEST PASS\n");
    return 0;
}