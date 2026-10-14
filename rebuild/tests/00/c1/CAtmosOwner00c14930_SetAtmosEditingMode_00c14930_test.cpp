#include <cstdio>
static int g_locks = 0, g_unlocks = 0;
struct CProfileLock { void* m_pSection; long m_entryTick; CProfileLock(void* s) : m_pSection(s) { ++g_locks; } ~CProfileLock() { ++g_unlocks; } };
static unsigned char g_section[1];
struct SAtmosRecord { unsigned char _pad[0xce]; unsigned char m_atmosFlag; };
struct CAtmosOwner { unsigned char _pad[8]; SAtmosRecord* m_pAtmos; void SetAtmosEditingMode(unsigned char mode) { CProfileLock lock(g_section); m_pAtmos->m_atmosFlag = mode; } };
int main() { SAtmosRecord r; r.m_atmosFlag = 0; CAtmosOwner o; o.m_pAtmos = &r; o.SetAtmosEditingMode(7); if (r.m_atmosFlag != 7 || g_locks != 1 || g_unlocks != 1) { std::printf("00c14930_TEST FAIL\n"); return 1; } std::printf("00c14930_TEST PASS\n"); return 0; }