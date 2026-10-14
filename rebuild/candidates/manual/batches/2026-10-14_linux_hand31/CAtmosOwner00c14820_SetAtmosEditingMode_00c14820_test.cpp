#include <cstdio>
static int g_locks = 0, g_unlocks = 0;
struct CProfileLock { void* m_pSection; long m_entryTick; CProfileLock(void* s) : m_pSection(s) { ++g_locks; } ~CProfileLock() { ++g_unlocks; } };
static unsigned char g_section[1];
struct SAtmosRecord { unsigned char _pad[0xc8]; long m_atmosMode; };
struct CAtmosOwner { unsigned char _pad[8]; SAtmosRecord* m_pAtmos; void SetAtmosEditingMode(long mode) { CProfileLock lock(g_section); m_pAtmos->m_atmosMode = mode; } };
int main() { SAtmosRecord r; r.m_atmosMode = 0; CAtmosOwner o; o.m_pAtmos = &r; o.SetAtmosEditingMode(5); if (r.m_atmosMode != 5 || g_locks != 1 || g_unlocks != 1) { std::printf("00c14820_TEST FAIL\n"); return 1; } std::printf("00c14820_TEST PASS\n"); return 0; }
