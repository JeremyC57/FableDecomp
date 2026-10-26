#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CDefBase { virtual void v00() { } CDefBase() { std::strcat(g_seq, "B"); } }; class CInterestingToVillagersDef : public CDefBase { public: unsigned char _pad04[0x24]; float Interest; virtual void v00() { } CInterestingToVillagersDef() : CDefBase(), Interest(1.0f) { } };

int main() {
    CInterestingToVillagersDef d; if (std::strcmp(g_seq, "B") || d.Interest != 1.0f) { std::printf("004d4fcf_TEST FAIL\n"); return 1; }
    std::printf("004d4fcf_TEST PASS\n"); return 0;
}