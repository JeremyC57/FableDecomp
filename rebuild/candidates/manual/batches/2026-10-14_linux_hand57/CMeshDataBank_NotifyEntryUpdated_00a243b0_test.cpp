#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
static unsigned long g_a = 0, g_i = 0; static const void* g_d = 0; struct CEntry { virtual void v000() { } virtual void Release() { } virtual unsigned long Sizeof() { return 0; } virtual void Reload() { std::strcat(g_seq, "r"); } }; struct CVec { CEntry** m_first; CEntry** m_last; CEntry** m_end; unsigned long size() const { return m_last - m_first; } };
struct CBank { virtual void OnEntryMissing(unsigned long a, unsigned long i, const void* d) { g_a = a; g_i = i; g_d = d; std::strcat(g_seq, "M"); } virtual void OnEntryReloaded(unsigned long a, unsigned long i, const void* d) { g_a = a; g_i = i; g_d = d; std::strcat(g_seq, "R"); }
    unsigned char _pad[0x37c]; CVec m_entries; virtual void NotifyEntryUpdated(unsigned long a, unsigned long i, const void* d) { CEntry* e = i < m_entries.size() ? m_entries.m_first[i] : 0; if (e) { e->Reload(); OnEntryReloaded(a, i, d); } else { OnEntryMissing(a, i, d); } } };

int main() {
    CEntry e; CEntry* p[2] = { &e, 0 }; CBank* b = new CBank; b->m_entries.m_first = p; b->m_entries.m_last = p + 2; int arr; b->NotifyEntryUpdated(7, 0, &arr); if (std::strcmp(g_seq, "rR") || g_a != 7 || g_i != 0 || g_d != &arr) { std::printf("00a243b0_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; b->NotifyEntryUpdated(8, 1, &arr); if (std::strcmp(g_seq, "M") || g_i != 1) { std::printf("00a243b0_TEST FAIL\n"); return 1; } g_seq[0] = 0; b->NotifyEntryUpdated(8, 5, &arr); if (std::strcmp(g_seq, "M") || g_i != 5) { std::printf("00a243b0_TEST FAIL\n"); return 1; } delete b;
    std::printf("00a243b0_TEST PASS\n"); return 0;
}
