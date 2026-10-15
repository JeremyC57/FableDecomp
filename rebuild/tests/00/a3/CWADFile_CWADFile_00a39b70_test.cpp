#include <cstdio>
#include <cstring>
struct CBankFileAsync { int id; };
static int g_baseCtor;
struct CBase { virtual ~CBase() {} CBase() { ++g_baseCtor; } };
struct CWADFile : CBase {
    CBankFileAsync* Bank; unsigned long Index; long FilePos;
    CWADFile(CBankFileAsync* bank) : Bank(bank), Index(0), FilePos(0) {}
    CWADFile(const CWADFile& o) : Bank(o.Bank), Index(o.Index), FilePos(o.FilePos) {}
    virtual CWADFile* Clone() { return new CWADFile(*this); }
};
int main()
{
    int ok = 1;
    CBankFileAsync bank;
    CWADFile f(&bank); f.Index = 3; f.FilePos = 77;
    CWADFile g(f);
    if (g.Bank != &bank || g.Index != 3 || g.FilePos != 77 || g_baseCtor != 2) ok = 0;
    if (!ok) { std::printf("00a39b70_TEST FAIL\n"); return 1; }
    std::printf("00a39b70_TEST PASS\n");
    return 0;
}