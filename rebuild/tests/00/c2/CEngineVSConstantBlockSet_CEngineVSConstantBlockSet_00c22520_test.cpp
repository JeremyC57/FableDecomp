// Behaviour test for 0x00C22520: the aggregate destructor tears members down in reverse declaration order, each parameter block destroying its CParam before its base.
#include <cstdio>
#include <cstring>
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ","); }
struct CParamBlock { ~CParamBlock() { Log("p"); } };
struct CBankArray { ~CBankArray() throw() { Log("bank"); } };
struct CResourceA { ~CResourceA() { Log("A"); } };
struct CResourceC { ~CResourceC() { Log("C"); } };
struct CVSConstantBase { virtual ~CVSConstantBase() { Log("base"); } long Register; };
struct CVSConstantMain : CVSConstantBase { virtual ~CVSConstantMain() { Log("main"); } CParamBlock Param; };
struct CVSConstantSpot : CVSConstantBase { virtual ~CVSConstantSpot() { Log("spot"); } CParamBlock Param; };
struct CSet {
    CVSConstantMain Main;
    CResourceA ResA;
    CBankArray Bank0;
    CResourceC ResC;
    CVSConstantSpot Spot;
    ~CSet() { }
};
int main()
{
    {
        CSet set;
        set.Main.Register = 1;
    }
    const char* want = "spot,p,base,C,bank,A,main,p,base,";
    int ok = std::strcmp(g_log, want) == 0;
    if (!ok)
        std::printf("got %s\n", g_log);
    std::printf(ok ? "00c22520_TEST PASS\n" : "00c22520_TEST FAIL\n");
    return ok ? 0 : 1;
}