#include <cstdio>
// Behaviour model of CNamedDataStream::~CNamedDataStream @ 0x00996D80: the name member is destroyed before the base.
static int g_order[4]; static int g_n = 0;
namespace N_0099a300 { struct CBase { virtual void v00() {} ~CBase() { g_order[g_n++] = 2; } }; } using N_0099a300::CBase;
class CCharString { public: void* m_pData; ~CCharString() { g_order[g_n++] = 1; } };
struct CDataStream : CBase { virtual void v00() {} ~CDataStream() {} };
struct CNamedDataStream : CDataStream { CCharString m_name; virtual void v00() {} ~CNamedDataStream() {} };
int main()
{
    { CNamedDataStream s; s.m_name.m_pData = 0; }
    bool ok = g_n == 2 && g_order[0] == 1 && g_order[1] == 2;
    if (!ok) { std::printf("00996d80_TEST FAIL\n"); return 1; }
    std::printf("00996d80_TEST PASS\n");
    return 0;
}