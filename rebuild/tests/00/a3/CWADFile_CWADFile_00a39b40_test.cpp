#include <cstdio>
#include <cstring>
class CBankFileAsync { public: int id; };
static int g_baseCtor;
class CBaseClassNonCopyable {
public:  virtual ~CBaseClassNonCopyable() {} CBaseClassNonCopyable() { ++g_baseCtor; } };
class CWADFile : public CBaseClassNonCopyable { public:
    CBankFileAsync* Bank; unsigned long Index; long FilePos;
    CWADFile(CBankFileAsync* bank) : Bank(bank), Index(0), FilePos(0) {}
    CWADFile(const CWADFile& o) : Bank(o.Bank), Index(o.Index), FilePos(o.FilePos) {}
    virtual CWADFile* Clone() { return new CWADFile(*this); }
};
int main()
{
    int ok = 1;
    CBankFileAsync bank;
    CWADFile f(&bank);
    if (f.Bank != &bank || f.Index != 0 || f.FilePos != 0 || g_baseCtor != 1) ok = 0;
    if (!ok) { std::printf("00a39b40_TEST FAIL\n"); return 1; }
    std::printf("00a39b40_TEST PASS\n");
    return 0;
}