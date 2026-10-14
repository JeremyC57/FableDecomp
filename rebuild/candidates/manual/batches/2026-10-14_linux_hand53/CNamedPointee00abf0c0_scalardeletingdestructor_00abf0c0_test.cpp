#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CCharString { void* m_p; CCharString() : m_p(0) { } ~CCharString() { std::strcat(g_seq, "s"); } };
struct CIVCountedPointeeBase { virtual void Destroy(int) { } virtual void Delete() { } unsigned long IVRefCount; ~CIVCountedPointeeBase() { std::strcat(g_seq, "B"); } }; struct CNamed : CIVCountedPointeeBase { CCharString m_8, m_c, m_10, m_14; ~CNamed() { } };

int main() {
    CNamed* p = new CNamed; delete p; if (std::strcmp(g_seq, "ssssB")) { std::printf("00abf0c0_TEST FAIL\n"); return 1; }
    std::printf("00abf0c0_TEST PASS\n"); return 0;
}
