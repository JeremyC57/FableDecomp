// Behaviour test for CThingCreatureBase::ConstructFromParams (0x00662880)
#include <cstdio>
struct C3DVector { float x, y, z; };
struct CCharString { const char* text; };
struct CThingPhysical { int id; };
struct CThingCreatureDef { int id; };
struct CDefPointerCreature { CThingCreatureDef* m_pObject; };
static CThingCreatureDef g_defs[4];
static long g_baseArgs[8];
static bool g_baseResult;
struct CDefinitionManager {
    bool GetDef(long index, CDefPointerCreature* out) { out->m_pObject = &g_defs[index]; return true; }
};
static CDefinitionManager g_defManager;
struct CThingGameObject {
    unsigned short m_defGlobalIndex;
    CDefinitionManager* GetDefinitionManager() const { return &g_defManager; }
    bool ConstructFromParams(long defIndex, const C3DVector& position, long a3, unsigned long a4,
                             const CCharString& name, CThingPhysical* p1, CThingPhysical* p2, bool flag)
    {
        g_baseArgs[0] = defIndex; g_baseArgs[1] = (long)position.x; g_baseArgs[2] = a3; g_baseArgs[3] = (long)a4;
        g_baseArgs[4] = name.text[0]; g_baseArgs[5] = p1->id; g_baseArgs[6] = p2->id; g_baseArgs[7] = flag;
        m_defGlobalIndex = (unsigned short)defIndex;
        return g_baseResult;
    }
};
struct CThingCreatureBase : CThingGameObject {
    CDefPointerCreature m_def;
    bool ConstructFromParams(long defIndex, const C3DVector& position, long a3, CThingPhysical* p1,
                             CThingPhysical* p2, const CCharString& name, bool flag)
    {
        if (CThingGameObject::ConstructFromParams(defIndex, position, a3, 0, name, p1, p2, flag)) {
            GetDefinitionManager()->GetDef(m_defGlobalIndex, &m_def);
            return true;
        }
        return false;
    }
};
int main()
{
    CThingCreatureBase c; c.m_def.m_pObject = 0;
    C3DVector pos = { 7.0f, 0.0f, 0.0f }; CCharString name = { "Q" };
    CThingPhysical p1 = { 11 }, p2 = { 22 };
    g_baseResult = false;
    if (c.ConstructFromParams(2, pos, 5, &p1, &p2, name, true) || c.m_def.m_pObject != 0) { std::printf("00662880_TEST FAIL\n"); return 1; }
    g_baseResult = true;
    if (!c.ConstructFromParams(3, pos, 5, &p1, &p2, name, true) || c.m_def.m_pObject != &g_defs[3]) { std::printf("00662880_TEST FAIL\n"); return 1; }
    if (g_baseArgs[0] != 3 || g_baseArgs[1] != 7 || g_baseArgs[2] != 5 || g_baseArgs[3] != 0 || g_baseArgs[4] != 'Q'
        || g_baseArgs[5] != 11 || g_baseArgs[6] != 22 || g_baseArgs[7] != 1) { std::printf("00662880_TEST FAIL\n"); return 1; }
    std::printf("00662880_TEST PASS\n");
    return 0;
}