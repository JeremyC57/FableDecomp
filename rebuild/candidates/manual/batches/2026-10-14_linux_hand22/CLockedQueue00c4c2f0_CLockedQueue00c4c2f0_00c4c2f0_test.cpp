#include <cstdio>
static void* g_initialised = 0;
struct CCriticalSection_00c1c810 {
    unsigned char _body[0x18];
    CCriticalSection_00c1c810() { g_initialised = this; }
};
struct CRetailVector_Retail {
    void* m_first; void* m_last; void* m_end;
    CRetailVector_Retail() : m_first(0), m_last(0), m_end(0) { }
};
struct CLockedQueue_00c4c2f0 {
    CRetailVector_Retail      m_items;
    void*                     m_pOwner;
    CCriticalSection_00c1c810 m_lock;
    unsigned char             m_flag;
    void*                     m_pTarget;
    CLockedQueue_00c4c2f0(void* owner, void* target);
};
CLockedQueue_00c4c2f0::CLockedQueue_00c4c2f0(void* owner, void* target)
    : m_items(), m_pOwner(owner), m_lock(), m_flag(0), m_pTarget(target)
{
}
int main()
{
    int owner, target;
    CLockedQueue_00c4c2f0 q(&owner, &target);
    if (q.m_items.m_first || q.m_items.m_last || q.m_items.m_end || q.m_pOwner != &owner || q.m_pTarget != &target || q.m_flag != 0
        || g_initialised != &q.m_lock || reinterpret_cast<unsigned char*>(&q.m_pTarget) - reinterpret_cast<unsigned char*>(&q) != 0x2c) { std::printf("00c4c2f0_TEST FAIL\n"); return 1; }
    std::printf("00c4c2f0_TEST PASS\n");
    return 0;
}
