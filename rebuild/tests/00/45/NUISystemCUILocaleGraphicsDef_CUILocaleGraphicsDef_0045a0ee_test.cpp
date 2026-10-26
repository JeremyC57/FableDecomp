#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
class CParentDefClassBase { public: virtual void v00() { } CParentDefClassBase() { std::strcat(g_seq, "B"); } };
struct CMember12 { unsigned char _body[0xc]; CMember12() { std::strcat(g_seq, "M"); } };
struct CUILocaleGraphicsDef : CParentDefClassBase { unsigned char _pad04[0x38]; CMember12 m_member3c; long m_48, m_4c, m_50; virtual void v00() { } CUILocaleGraphicsDef() : CParentDefClassBase(), m_member3c(), m_48(0), m_4c(0), m_50(0) { } };

int main() {
    CUILocaleGraphicsDef d; if (std::strcmp(g_seq, "BM") || d.m_48 || d.m_4c || d.m_50) { std::printf("0045a0ee_TEST FAIL\n"); return 1; }
    std::printf("0045a0ee_TEST PASS\n"); return 0;
}