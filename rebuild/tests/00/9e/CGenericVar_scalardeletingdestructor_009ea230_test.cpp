#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CCharString { void* m_p; CCharString() : m_p(0) { } ~CCharString() { std::strcat(g_seq, "s"); } };
struct CRoot { virtual void v00() { } }; struct CGenericVar : CRoot { long Type; CCharString m_name; virtual ~CGenericVar() { } };

int main() {
    CGenericVar* p = new CGenericVar; delete p; if (std::strcmp(g_seq, "s")) { std::printf("009ea230_TEST FAIL\n"); return 1; }
    std::printf("009ea230_TEST PASS\n"); return 0;
}