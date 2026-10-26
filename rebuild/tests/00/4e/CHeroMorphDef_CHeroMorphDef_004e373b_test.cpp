#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
static char g_seq[64] = "";
struct CDefBase { virtual void v00() { } CDefBase() { std::strcat(g_seq, "B"); } };
static void* g_strings[8]; static long g_nStrings = 0;
class CCharString { public: void* m_pStringData; CCharString() { m_pStringData = 0; g_strings[g_nStrings++] = this; std::strcat(g_seq, "s"); } ~CCharString() { g_strings[g_nStrings++] = this; std::strcat(g_seq, "d"); } };
struct SMorphRecord { long m_a, m_b, m_c; unsigned char _padc; bool m_flag; unsigned char _pade[2]; SMorphRecord() : m_a(0), m_b(0), m_c(0), m_flag(false) { } };
class CHeroMorphDef : public CDefBase { public: unsigned char _pad04[0x24]; SMorphRecord m_morph28, m_morph38, m_morph48, m_morph58; virtual void v00() { } CHeroMorphDef() : CDefBase() { } };

int main() {
    unsigned char raw[sizeof(CHeroMorphDef)]; std::memset(raw, 0xff, sizeof raw); CHeroMorphDef& d = *new (raw) CHeroMorphDef; if (std::strcmp(g_seq, "B") || d.m_morph28.m_a || d.m_morph28.m_flag || d.m_morph58.m_c || d.m_morph58.m_flag || d.m_morph38._padc != 0xff) { std::printf("004e373b_TEST FAIL\n"); return 1; }
    std::printf("004e373b_TEST PASS\n"); return 0;
}