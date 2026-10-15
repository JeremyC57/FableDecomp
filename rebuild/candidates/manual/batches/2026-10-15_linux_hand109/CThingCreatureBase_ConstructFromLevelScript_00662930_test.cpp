// Behaviour test for CThingCreatureBase::ConstructFromLevelScript (0x00662930)
#include <cstdio>
struct CThingCreatureDef { int id; };
struct CDefPointerCreature { CThingCreatureDef* m_pObject; };
static CThingCreatureDef g_defs[4];
struct CDefinitionManager {
    bool GetDef(long index, CDefPointerCreature* out) { out->m_pObject = &g_defs[index]; return true; }
};
static CDefinitionManager g_defManager;
struct CThingPhysical {
    unsigned short m_defGlobalIndex;
    CDefinitionManager* GetDefinitionManager() const { return &g_defManager; }
    bool ConstructFromLevelScript(const char* script)
    {
        if (script[0] == 0) return false;
        m_defGlobalIndex = (unsigned short)(script[0] - '0');
        return true;
    }
};
struct CThingCreatureBase : CThingPhysical {
    CDefPointerCreature m_def;
    bool ConstructFromLevelScript(const char* script)
    {
        if (CThingPhysical::ConstructFromLevelScript(script)) {
            GetDefinitionManager()->GetDef(m_defGlobalIndex, &m_def);
            return true;
        }
        return false;
    }
};
int main()
{
    CThingCreatureBase c; c.m_defGlobalIndex = 0; c.m_def.m_pObject = 0;
    if (c.ConstructFromLevelScript("") || c.m_def.m_pObject != 0) { std::printf("00662930_TEST FAIL\n"); return 1; }
    if (!c.ConstructFromLevelScript("3") || c.m_def.m_pObject != &g_defs[3]) { std::printf("00662930_TEST FAIL\n"); return 1; }
    std::printf("00662930_TEST PASS\n");
    return 0;
}
