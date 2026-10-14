#include <cstdio>
static int g_dtors = 0;
struct SEntry { unsigned char _body[0x40]; ~SEntry() { ++g_dtors; } };
struct CEntryTableOwner { virtual void v00() { } unsigned char _pad04[4]; SEntry* m_pEntries; ~CEntryTableOwner() { if (m_pEntries != 0) delete[] m_pEntries; } };
int main() { { CEntryTableOwner o; o.m_pEntries = new SEntry[3]; } if (g_dtors != 3) { std::printf("00c48db0_TEST FAIL\n"); return 1; } { CEntryTableOwner o; o.m_pEntries = 0; } if (g_dtors != 3) { std::printf("00c48db0_TEST FAIL\n"); return 1; } std::printf("00c48db0_TEST PASS\n"); return 0; }