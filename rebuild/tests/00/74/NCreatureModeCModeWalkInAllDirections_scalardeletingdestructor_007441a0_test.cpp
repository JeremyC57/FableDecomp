#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CEventContainer { virtual void v00() { } unsigned char _body[0x3c]; ~CEventContainer() { std::strcat(g_seq, "E"); } }; class CAnimFilterBase { public: virtual void v00() { } ~CAnimFilterBase() { std::strcat(g_seq, "A"); } }; struct CFiltered : CEventContainer { CAnimFilterBase m_filter; ~CFiltered() { } };

int main() {
    CFiltered* p = new CFiltered; delete p; if (std::strcmp(g_seq, "AE")) { std::printf("007441a0_TEST FAIL\n"); return 1; }
    std::printf("007441a0_TEST PASS\n"); return 0;
}