// Behaviour test for CThing::GetDefName @ 0x004C7CC0: the def name comes from the definition manager by global index.
#include <cstdio>
class CDefString { public: long index; };
class CDefinitionManager { public:
    CDefString GetDefNameFromGlobalIndex(long globalIndex) { CDefString s; s.index = globalIndex * 10; return s; }
};
class CThingManager { public: CDefinitionManager* m_pDefinitionManager; };
static CDefinitionManager g_defs;
static CThingManager g_manager;
static CThingManager* g_pThingManager = &g_manager;
class CThing { public:
    unsigned short DefGlobalIndex;
    CDefString GetDefName() const { return g_pThingManager->m_pDefinitionManager->GetDefNameFromGlobalIndex(DefGlobalIndex); }
};
int main()
{
    g_manager.m_pDefinitionManager = &g_defs;
    CThing t; t.DefGlobalIndex = 0xfffe;
    if (t.GetDefName().index != 0xfffeL * 10) { std::printf("004c7cc0_TEST FAIL\n"); return 1; }
    std::printf("004c7cc0_TEST PASS\n");
    return 0;
}