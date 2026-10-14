#include <cstdio>
#include <cstdlib>
#include <cstring>
static unsigned long g_table[8] = {0, 16, 32, 64, 128, 256, 512, 1024};
static unsigned long LookupAllocatorSize(unsigned long relativeId) { return g_table[relativeId]; }
struct SAllocatorIdTable { unsigned long m_count; unsigned long m_allocatorIds[4]; };
struct CAllocatorContext { unsigned long m_baseAllocatorId; };
struct CPoolOwner { CAllocatorContext* m_pContext; };
struct CPoolEntryTable {
    CPoolOwner* m_pOwner; SAllocatorIdTable* m_pIds;
    unsigned long GetEntryAllocatorSize(long index) const
    {
        unsigned long base = m_pOwner->m_pContext->m_baseAllocatorId;
        unsigned long id = m_pIds->m_allocatorIds[index];
        long slot = id - 1;
        if (slot < 0) return 0;
        return LookupAllocatorSize(id - base);
    }
};
static bool Check()
{
    CAllocatorContext ctx; ctx.m_baseAllocatorId = 100; CPoolOwner owner; owner.m_pContext = &ctx;
    SAllocatorIdTable ids = { 4, {0, 101, 103, 107} };
    CPoolEntryTable t; t.m_pOwner = &owner; t.m_pIds = &ids;
    return t.GetEntryAllocatorSize(0) == 0 && t.GetEntryAllocatorSize(1) == 16 && t.GetEntryAllocatorSize(2) == 64 && t.GetEntryAllocatorSize(3) == 1024;
}
int main()
{
    if (!Check()) { std::printf("00c90c80_TEST FAIL\n"); return 1; }
    std::printf("00c90c80_TEST PASS\n");
    return 0;
}
