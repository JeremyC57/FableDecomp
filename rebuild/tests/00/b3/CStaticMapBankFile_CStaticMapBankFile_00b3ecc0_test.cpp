#include <cstdio>

// Model of CStaticMapBankFile::CStaticMapBankFile() @ 0x00B3ECC0
static int g_baseCalls = 0; static int g_setArg = -1;
class CBankFile { public:
    virtual void v00() {}
    unsigned char _body[0x118 - 4];
    CBankFile() { ++g_baseCalls; }
    void SetIsStaticMap(bool b) { g_setArg = b ? 1 : 0; }
};
class CStaticMapBankFile : public CBankFile { public:
    virtual void v00() {}
    CStaticMapBankFile() { SetIsStaticMap(true); }
};
int main()
{
    CStaticMapBankFile f;
    if (g_baseCalls != 1 || g_setArg != 1) { std::printf("00b3ecc0_TEST FAIL\n"); return 1; }
    std::printf("00b3ecc0_TEST PASS\n"); return 0;
}