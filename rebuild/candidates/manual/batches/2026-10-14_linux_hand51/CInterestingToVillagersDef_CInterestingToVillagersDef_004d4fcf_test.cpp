#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CDefBase { virtual void v00() { } CDefBase() { std::strcat(g_seq, "B"); } }; struct CInterestingToVillagersDef : CDefBase { unsigned char _pad04[0x24]; float m_28; virtual void v00() { } CInterestingToVillagersDef() : CDefBase(), m_28(1.0f) { } };

int main() {
    CInterestingToVillagersDef d; if (std::strcmp(g_seq, "B") || d.m_28 != 1.0f) { std::printf("004d4fcf_TEST FAIL\n"); return 1; }
    std::printf("004d4fcf_TEST PASS\n"); return 0;
}
