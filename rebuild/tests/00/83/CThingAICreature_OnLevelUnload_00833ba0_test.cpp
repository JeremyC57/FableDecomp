// Behaviour test for 00833ba0 (CThingAICreature::OnLevelUnload): the base unload runs, a null suspender is added (AISuspended becomes set), and the brain is told when present.
#include <cstdio>

static int g_baseCalls = 0, g_brainCalls = 0;

struct CSuspenderSet {
    void*         m_items[8];
    unsigned long m_size;
    void InsertUnique(void* const& value)
    {
        for (unsigned long i = 0; i < m_size; ++i)
            if (m_items[i] == value) return;
        m_items[m_size++] = value;
    }
    bool empty() const { return m_size == 0; }
};

class CAIBrain { public:
    void OnLevelUnload() { ++g_brainCalls; }
};

class CThing { public:
    void OnLevelUnload() { ++g_baseCalls; }
};

class CThingAICreature : public CThing { public:
    CAIBrain*     PBrain_Data;
    bool          PerformingVillageTaskFlag : 1;
    bool          AISuspended : 1;
    CSuspenderSet Suspenders;
    void AddSuspender(void* suspender)
    {
        Suspenders.InsertUnique(suspender);
        AISuspended = !Suspenders.empty();
    }
    void OnLevelUnload()
    {
        CThing::OnLevelUnload();
        AddSuspender(0);
        if (PBrain_Data)
            PBrain_Data->OnLevelUnload();
    }
};

int main()
{
    CAIBrain brain;
    CThingAICreature c;
    c.PBrain_Data = 0; c.PerformingVillageTaskFlag = true; c.AISuspended = false; c.Suspenders.m_size = 0;
    c.OnLevelUnload();
    if (g_baseCalls != 1 || g_brainCalls != 0 || !c.AISuspended || !c.PerformingVillageTaskFlag || c.Suspenders.m_size != 1) { std::printf("00833ba0_TEST FAIL\n"); return 1; }
    c.PBrain_Data = &brain;
    c.OnLevelUnload();
    if (g_baseCalls != 2 || g_brainCalls != 1 || !c.AISuspended || c.Suspenders.m_size != 1) { std::printf("00833ba0_TEST FAIL\n"); return 1; }
    std::printf("00833ba0_TEST PASS\n");
    return 0;
}