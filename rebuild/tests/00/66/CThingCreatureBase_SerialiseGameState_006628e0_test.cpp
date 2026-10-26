// Behaviour test for CThingCreatureBase::SerialiseGameState (0x006628E0)
#include <cstdio>
class CThingCreatureDef { public: int id; };
struct CDefPointerCreature { CThingCreatureDef* m_pObject; };
class CPersistContext { public: long m_mode; };
static CThingCreatureDef g_defs[4];
static bool g_baseResult;
static int g_fetches;
class CDefinitionManager { public:
    bool GetDef(long index, CDefPointerCreature* out) { ++g_fetches; out->m_pObject = &g_defs[index]; return true; }
};
static CDefinitionManager g_defManager;
class CThingPhysical { public:
    unsigned short m_defGlobalIndex;
    CDefinitionManager* GetDefinitionManager() const { return &g_defManager; }
    bool SerialiseGameState(CPersistContext&) { return g_baseResult; }
};
class CThingCreatureBase : public CThingPhysical { public:
    CDefPointerCreature m_def;
    bool SerialiseGameState(CPersistContext& context)
    {
        if (CThingPhysical::SerialiseGameState(context)) {
            if (context.m_mode != 1 && context.m_mode != 3)
                GetDefinitionManager()->GetDef(m_defGlobalIndex, &m_def);
            return true;
        }
        return false;
    }
};
int main()
{
    CThingCreatureBase c; c.m_defGlobalIndex = 2; c.m_def.m_pObject = 0;
    CPersistContext ctx; g_fetches = 0;
    g_baseResult = false; ctx.m_mode = 0;
    if (c.SerialiseGameState(ctx) || g_fetches != 0) { std::printf("006628e0_TEST FAIL\n"); return 1; }
    g_baseResult = true;
    ctx.m_mode = 1; if (!c.SerialiseGameState(ctx) || g_fetches != 0) { std::printf("006628e0_TEST FAIL\n"); return 1; }
    ctx.m_mode = 3; if (!c.SerialiseGameState(ctx) || g_fetches != 0) { std::printf("006628e0_TEST FAIL\n"); return 1; }
    ctx.m_mode = 2; if (!c.SerialiseGameState(ctx) || g_fetches != 1 || c.m_def.m_pObject != &g_defs[2]) { std::printf("006628e0_TEST FAIL\n"); return 1; }
    std::printf("006628e0_TEST PASS\n");
    return 0;
}