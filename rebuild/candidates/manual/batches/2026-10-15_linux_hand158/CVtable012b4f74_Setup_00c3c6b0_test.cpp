// Behaviour test for 0x00C3C6B0: kind range check, SetDesc result passthrough, conditional create + close on failure.
#include <cstdio>
struct Desc { unsigned int kind; };
struct Obj { int setResult, createResult, creates, closes; bool hw;
  int SetDesc(const Desc&) { return setResult; }
  int Create() { ++creates; return createResult; }
  void Close() { ++closes; }
  int Setup(const Desc& d) { if (d.kind > 6) return -3; int r = SetDesc(d);
    if (r >= 0 && hw && d.kind <= 2) { r = Create(); if (r < 0) Close(); } return r; } };
int main()
{
    Obj o = {0, -7, 0, 0, true}; Desc d7 = {7}, d1 = {1}, d4 = {4};
    bool ok = o.Setup(d7) == -3 && o.creates == 0;
    ok = ok && o.Setup(d1) == -7 && o.creates == 1 && o.closes == 1;
    ok = ok && o.Setup(d4) == 0 && o.creates == 1;
    o.setResult = -5; ok = ok && o.Setup(d1) == -5 && o.creates == 1;
    o.setResult = 0; o.hw = false; ok = ok && o.Setup(d1) == 0 && o.creates == 1;
    o.hw = true; o.createResult = 3; ok = ok && o.Setup(d1) == 3 && o.closes == 1;
    if (!ok) { std::printf("00c3c6b0_TEST FAIL\n"); return 1; }
    std::printf("00c3c6b0_TEST PASS\n");
    return 0;
}
