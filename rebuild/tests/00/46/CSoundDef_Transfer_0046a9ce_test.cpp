// Behaviour test for 0x0046A9CE: transfer order, five vectors then three named floats with zero defaults.
#include <cstdio>
#include <cstring>
struct Ctx { const char* names[8]; int n; int vectors;
  void TransferBinary(void*) { ++vectors; names[n++] = ""; }
  void Transfer(const char* name, float& v, const float& d) { names[n++] = name; if (d != 0.0f) v = -1; } };
struct Def { int vec[5]; float a, b, c;
  void Transfer(Ctx& c2) { for (int i = 0; i < 5; ++i) c2.TransferBinary(&vec[i]);
    c2.Transfer("AtmosListenerDampingMin", a, 0.0f); c2.Transfer("AtmosListenerDampingRange", b, 0.0f); c2.Transfer("DistanceModelCurve", c, 0.0f); } };
int main()
{
    Ctx c; c.n = 0; c.vectors = 0; Def d; d.a = d.b = d.c = 1; d.Transfer(c);
    bool ok = c.vectors == 5 && c.n == 8 && std::strcmp(c.names[5], "AtmosListenerDampingMin") == 0 && std::strcmp(c.names[7], "DistanceModelCurve") == 0 && d.a == 1;
    if (!ok) { std::printf("0046a9ce_TEST FAIL\n"); return 1; }
    std::printf("0046a9ce_TEST PASS\n");
    return 0;
}