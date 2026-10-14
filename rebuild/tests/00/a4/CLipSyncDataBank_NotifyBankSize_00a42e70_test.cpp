#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SEntry { float m_0, m_4; SEntry() : m_0(0.0f), m_4(0.0f) { } }; static unsigned long g_size = 0; static float g_v0 = 1, g_v1 = 1;
struct CVec { SEntry* a; SEntry* b; SEntry* c; void resize(unsigned long n, const SEntry& v) { g_size = n; g_v0 = v.m_0; g_v1 = v.m_4; } }; struct CArray;
struct CBank { unsigned char _pad[0x164]; CVec m_entries; void NotifyBankSize(unsigned long size, const CArray* counts) { (void)counts; m_entries.resize(size, SEntry()); } };

int main() {
    CBank b; b.NotifyBankSize(7, 0); if (g_size != 7 || g_v0 != 0.0f || g_v1 != 0.0f) { std::printf("00a42e70_TEST FAIL\n"); return 1; }
    std::printf("00a42e70_TEST PASS\n"); return 0;
}