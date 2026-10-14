#include <cstdio>
static int g_locks = 0, g_unlocks = 0;
struct CProfileLock { void* m_pSection; long m_entryTick; CProfileLock(void* s) : m_pSection(s) { ++g_locks; } ~CProfileLock() { ++g_unlocks; } };
static unsigned char g_section[1];
struct SAtmosOwnerRecord { unsigned char _pad[0x18]; long m_id; };
struct CAtmosOwner { unsigned char _pad[4]; SAtmosOwnerRecord* m_pOwner; long GetAtmosOwnerId() { CProfileLock lock(g_section); return m_pOwner->m_id; } };
int main() { SAtmosOwnerRecord r; r.m_id = 31; CAtmosOwner o; o.m_pOwner = &r; if (o.GetAtmosOwnerId() != 31 || g_locks != 1 || g_unlocks != 1) { std::printf("00c1e570_TEST FAIL\n"); return 1; } std::printf("00c1e570_TEST PASS\n"); return 0; }