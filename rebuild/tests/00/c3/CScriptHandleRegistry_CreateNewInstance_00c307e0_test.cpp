#include <cstdio>
struct SHandleEntry { void* m_pOwner; long m_id; };
void* g_pCurrentOwner = 0;
long s_nextHandleId = 0;
struct SHandleNode {
    bool m_bClaimed; SHandleEntry m_entry;
    SHandleNode() : m_bClaimed(false) { m_entry.m_pOwner = g_pCurrentOwner; m_entry.m_id = s_nextHandleId++; }
};
static SHandleNode* g_registered = 0;
struct CScriptHandleRegistry {
    virtual void Register(SHandleNode* node) { g_registered = node; }
    SHandleEntry* CreateNewInstance() { SHandleNode* node = new SHandleNode; Register(node); return &node->m_entry; }
};
int main()
{
    int owner = 0; g_pCurrentOwner = &owner; s_nextHandleId = 41;
    CScriptHandleRegistry registry;
    SHandleEntry* entry = registry.CreateNewInstance();
    if (entry == 0 || g_registered == 0 || entry != &g_registered->m_entry || g_registered->m_bClaimed || entry->m_pOwner != &owner || entry->m_id != 41 || s_nextHandleId != 42) { std::printf("00c307e0_TEST FAIL\n"); return 1; }
    delete g_registered;
    std::printf("00c307e0_TEST PASS\n");
    return 0;
}