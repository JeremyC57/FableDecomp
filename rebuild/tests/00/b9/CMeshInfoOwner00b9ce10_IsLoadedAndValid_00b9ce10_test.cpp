#include <cstdio>
struct CMeshInfoOwner_00b9ce10 { bool a, b; bool IsLoaded() { return a; } bool IsValid() { return b; } long IsLoadedAndValid(); };
long CMeshInfoOwner_00b9ce10::IsLoadedAndValid() { if (IsLoaded() && IsValid()) return 1; return 0; }
int main()
{
    CMeshInfoOwner_00b9ce10 o = { true, true }; if (o.IsLoadedAndValid() != 1) { std::printf("00b9ce10_TEST FAIL\n"); return 1; }
    o.b = false; if (o.IsLoadedAndValid() != 0) { std::printf("00b9ce10_TEST FAIL\n"); return 1; }
    o.a = false; o.b = true; if (o.IsLoadedAndValid() != 0) { std::printf("00b9ce10_TEST FAIL\n"); return 1; }
    std::printf("00b9ce10_TEST PASS\n"); return 0;
}