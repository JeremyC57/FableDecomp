#include <cstdio>
// Behaviour test for CFontBank scalar deleting destructor (0x009A0B30): the bank name dies before CBase.
static int g_order[8]; static int g_n = 0;
class CCharString { public: void* m_pData; ~CCharString() { g_order[g_n++] = 2; } };
namespace N_0099a300 { struct CBase { virtual ~CBase() { g_order[g_n++] = 1; } }; } using N_0099a300::CBase;
class CFontBank : public CBase { public: unsigned long FontHeight; CCharString Name; void* PManager; virtual ~CFontBank() { } };
int main()
{
    CBase* p = new CFontBank;
    delete p;
    if (g_n != 2 || g_order[0] != 2 || g_order[1] != 1) { std::printf("009a0b30_TEST FAIL\n"); return 1; }
    std::printf("009a0b30_TEST PASS\n");
    return 0;
}