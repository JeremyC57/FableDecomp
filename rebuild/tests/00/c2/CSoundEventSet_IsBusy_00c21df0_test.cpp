// Behaviour test for CSoundEventSet::IsBusy (0x00C21DF0): the call forwards to the member free list's Pop (virtual slot 1): it hands back
// the next free element while any is left and 0 once the pool is exhausted (the retail pop 0x00C23BE0: `if (!count) return 0;
// return items[index], --count`).
#include <cstdio>
struct FreeList {
    void* items[4]; unsigned count;
    virtual ~FreeList() {}
    virtual void* Pop() { if (!count) return 0; --count; return items[count]; }
};
struct Owner {
    FreeList m_pool;
    void* Call() { return m_pool.Pop(); }
};
int main()
{
    int a, b;
    Owner o;
    o.m_pool.items[0] = &a; o.m_pool.items[1] = &b; o.m_pool.count = 2;
    if (o.Call() != &b || o.Call() != &a) { std::printf("00c21df0_TEST FAIL 1\n"); return 1; }
    if (o.Call() != 0 || o.Call() != 0) { std::printf("00c21df0_TEST FAIL 2\n"); return 1; }
    std::printf("00c21df0_TEST PASS\n");
    return 0;
}