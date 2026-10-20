// Behaviour test for 006a1330: body copied from src/006a1330.cpp (CTCCarrying::IsSlotPrimarySlot), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_destroyed = 0;
struct CCarrySlotDef {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long          m_refCount;
    unsigned char _pad08[0x30 - 0x08];
    bool          IsPrimary;
};
template <class T> struct CDefPointer {
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer()
    {
        T* p = m_p;
        long count = p->m_refCount - 1;
        p->m_refCount = count;
        if (count == 0)
            p->Destroy();
    }
    T* operator->() const { return m_p; }
};
static CCarrySlotDef g_slots[2]; static long g_askedIndex = -1; static int g_getDef = 0;
struct CDefinitionManager {
    void GetDef(long index, CDefPointer<CCarrySlotDef>* out);
};
void CDefinitionManager::GetDef(long index, CDefPointer<CCarrySlotDef>* out)
{
    ++g_getDef; g_askedIndex = index;
    CCarrySlotDef* d = &g_slots[index]; ++d->m_refCount; out->m_p = d;
}
static CDefinitionManager g_manager;
struct CGameDefinitionManager {
    static CDefinitionManager* Get() { return &g_manager; }
};
struct CTCCarrying {
    virtual void v00() {}
    bool IsSlotPrimarySlot(long slot);
};
bool CTCCarrying::IsSlotPrimarySlot(long slot)
{
    CDefPointer<CCarrySlotDef> def;
    CGameDefinitionManager::Get()->GetDef(slot, &def);
    return def->IsPrimary;
}
int main()
{
    g_slots[0].m_refCount = 1; g_slots[0].IsPrimary = true; g_slots[1].m_refCount = 1; g_slots[1].IsPrimary = false;
    CTCCarrying tc;
    CHECK(tc.IsSlotPrimarySlot(0)); CHECK(g_getDef == 1 && g_askedIndex == 0); CHECK(g_slots[0].m_refCount == 1 && g_destroyed == 0);
    CHECK(!tc.IsSlotPrimarySlot(1)); CHECK(g_askedIndex == 1);
    g_slots[1].m_refCount = 0; CHECK(!tc.IsSlotPrimarySlot(1)); CHECK(g_destroyed == 1);
    if (fails == 0) printf("006a1330_TEST PASS\n"); else printf("006a1330_TEST FAIL\n");
    return fails;
}