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
    CWADFile f(&bank); f.Index = 5; f.FilePos = 12;
    CWADFile* c = f.Clone();
    if (c == 0 || c == &f || c->Bank != &bank || c->Index != 5 || c->FilePos != 12 || g_baseCtor != 2) ok = 0;
    delete c;
    if (!ok) { std::printf("00a39c80_TEST FAIL\n"); return 1; }
    std::printf("00a39c80_TEST PASS\n");
    return 0;
}