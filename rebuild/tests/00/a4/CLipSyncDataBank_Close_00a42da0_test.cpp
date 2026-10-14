#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct SE { unsigned char m_body[4]; }; struct SMoveTag { SMoveTag() { } }; struct SAllocTag { SAllocTag() { } }; static SE* g_mf; static SE* g_ml; static SE* g_md; static SE* g_df; static SE* g_dl;
static SE* Move(SE* f, SE* l, SE* d, const SMoveTag&, bool) { g_mf = f; g_ml = l; g_md = d; std::strcat(g_seq, "M"); return d; } static void Destroy(SE* f, SE* l, const SAllocTag&) { g_df = f; g_dl = l; std::strcat(g_seq, "D"); }
struct CBankFile { unsigned char _body[0x164]; void Close() { std::strcat(g_seq, "C"); } }; struct CBank : CBankFile { SE* m_first; SE* m_last; SE* m_end; void Close() { SE* last = m_last; SE* first = m_first; SE* kept = Move(last, last, first, SMoveTag(), false); Destroy(kept, m_last, SAllocTag()); m_last = kept; CBankFile::Close(); } };

int main() {
    CBank b; SE buf[4]; b.m_first = buf; b.m_last = buf + 3; b.m_end = buf + 4; b.Close(); if (b.m_last != buf || g_mf != buf + 3 || g_ml != buf + 3 || g_md != buf || g_df != buf || g_dl != buf + 3 || std::strcmp(g_seq, "MDC")) { std::printf("00a42da0_TEST FAIL\n"); return 1; }
    std::printf("00a42da0_TEST PASS\n"); return 0;
}