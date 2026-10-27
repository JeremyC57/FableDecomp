// Test for 0x009D8240 NProfileTimer::StartProfile: the retail profiling hook is compiled out -- it touches nothing.
#include <stdio.h>
class CCharString { public: const char* PStringData; };
namespace NProfileTimer { void StartProfile(const CCharString& name, int id) { } }
int main()
{
    CCharString n; n.PStringData = "Frame";
    NProfileTimer::StartProfile(n, 3);
    bool ok = n.PStringData[0] == 'F';
    puts(ok ? "009d8240_TEST PASS" : "009d8240_TEST FAIL");
    return ok ? 0 : 1;
}