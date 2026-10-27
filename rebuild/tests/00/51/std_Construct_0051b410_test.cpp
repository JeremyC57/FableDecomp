// Behaviour test for 0051b410 (plain C++98 model of the reconstructed logic).
#include <cstdio>
#include <new>
#include <new>
static int g_Copies = 0;
class CRegion { public: int Id; CRegion(int i) : Id(i) { } CRegion(const CRegion& o) : Id(o.Id) { ++g_Copies; } };
template <class T1, class T2> void Construct(T1* p, const T2& v) { new (p) T1(v); }
int main() {
    CRegion src(42); unsigned char buf[sizeof(CRegion) + 8];
    CRegion* p = (CRegion*)(void*)buf; Construct(p, src);
    if (p->Id != 42 || g_Copies != 1) { std::printf("0051b410_TEST FAIL\n"); return 1; }
    { std::printf("0051b410_TEST PASS\n"); return 0; }
}