#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
class CCharString { public: void* m_p; CCharString() : m_p(0) { } ~CCharString() { std::strcat(g_seq, "s"); } };
struct CRoot { virtual void v00() { } }; class CAnimFilterBase { public: virtual void v00() { } ~CAnimFilterBase() { std::strcat(g_seq, "A"); } }; struct CModeBase : CRoot { long ParentThing; CAnimFilterBase m_filter; CCharString m_name; virtual ~CModeBase() { } };

int main() {
    CModeBase* p = new CModeBase; delete p; if (std::strcmp(g_seq, "sA")) { std::printf("00739ee0_TEST FAIL\n"); return 1; }
    std::printf("00739ee0_TEST PASS\n"); return 0;
}