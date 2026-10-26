#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
static char g_seq[64] = "";
class CAIStateGroupBase { public: virtual void v00() { } unsigned char _pad[0x14]; CAIStateGroupBase() { std::strcat(g_seq, "B"); } }; struct CGroup : CAIStateGroupBase { long m_word; CGroup() : CAIStateGroupBase(), m_word(0) { } };

int main() {
    unsigned char raw[sizeof(CGroup)]; std::memset(raw, 0xff, sizeof raw); CGroup& g = *new (raw) CGroup; if (std::strcmp(g_seq, "B") || g.m_word != 0) { std::printf("008fdb27_TEST FAIL\n"); return 1; }
    std::printf("008fdb27_TEST PASS\n"); return 0;
}