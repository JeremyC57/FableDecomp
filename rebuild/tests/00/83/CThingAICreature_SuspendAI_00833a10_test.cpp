// Behaviour test for CThingAICreature::SuspendAI @ 0x00833A10: suspender set add/remove, AISuspended follows non-emptiness.
#include <cstdio>
struct CSuspenderSet {
    void* items[8]; unsigned long m_size;
    void InsertUnique(void* const& value)
    {
        for (unsigned long i = 0; i < m_size; ++i) if (items[i] == value) return;
        items[m_size++] = value;
    }
    unsigned long Erase(void* const& value)
    {
        for (unsigned long i = 0; i < m_size; ++i)
            if (items[i] == value) { items[i] = items[--m_size]; return 1; }
        return 0;
    }
    bool empty() const { return m_size == 0; }
};
struct CThingAICreature {
    bool PerformingVillageTaskFlag : 1;
    bool AISuspended : 1;
    CSuspenderSet Suspenders;
    void SuspendAI(bool suspend, void* suspender)
    {
        if (suspend)
            Suspenders.InsertUnique(suspender);
        else
            Suspenders.Erase(suspender);
        AISuspended = !Suspenders.empty();
    }
};
int main()
{
    int x, y;
    CThingAICreature c; c.PerformingVillageTaskFlag = true; c.AISuspended = false; c.Suspenders.m_size = 0;
    c.SuspendAI(true, &x);
    c.SuspendAI(true, &x);
    c.SuspendAI(true, &y);
    if (!c.AISuspended || c.Suspenders.m_size != 2 || !c.PerformingVillageTaskFlag) { std::printf("00833a10_TEST FAIL\n"); return 1; }
    c.SuspendAI(false, &x);
    if (!c.AISuspended) { std::printf("00833a10_TEST FAIL\n"); return 1; }
    c.SuspendAI(false, &y);
    if (c.AISuspended || !c.PerformingVillageTaskFlag) { std::printf("00833a10_TEST FAIL\n"); return 1; }
    std::printf("00833a10_TEST PASS\n");
    return 0;
}