// Behaviour test for CThing::CreateUID @ 0x004C7FF0: UID allocated for the map with the class flag and the def's LevelLess bit.
#include <cstdio>
struct SThingClassInfo { bool m_flag; };
class CThingManager { public:
    SThingClassInfo* m_pClassInfo;
    long map; bool cls; bool lvl;
    unsigned long AllocateNewThingUID(long mapNumber, bool classFlag, bool levelLess)
    { map = mapNumber; cls = classFlag; lvl = levelLess; return 1000 + mapNumber; }
};
static CThingManager g_manager;
static CThingManager* g_pThingManager = &g_manager;
class CThingBaseDef { public: unsigned char low : 3; unsigned char LevelLess : 1; };
class CThing { public:
    unsigned char  m_thingClass;
    unsigned long  UniqueID;
    CThingBaseDef* PBaseDef;
    short          CurrentMapNumber;
    void CreateUID()
    {
        bool levelLess = false;
        if (PBaseDef && PBaseDef->LevelLess)
            levelLess = true;
        bool classFlag = g_pThingManager->m_pClassInfo[m_thingClass].m_flag;
        UniqueID = g_pThingManager->AllocateNewThingUID(CurrentMapNumber, classFlag, levelLess);
    }
};
int main()
{
    SThingClassInfo info[3] = { { false }, { true }, { false } };
    g_manager.m_pClassInfo = info;
    CThingBaseDef def; def.low = 0; def.LevelLess = 1;
    CThing t; t.m_thingClass = 1; t.UniqueID = 0; t.PBaseDef = &def; t.CurrentMapNumber = 12;
    t.CreateUID();
    if (t.UniqueID != 1012 || g_manager.map != 12 || !g_manager.cls || !g_manager.lvl) { std::printf("004c7ff0_TEST FAIL\n"); return 1; }
    t.PBaseDef = 0; t.m_thingClass = 2; t.CurrentMapNumber = -1;
    t.CreateUID();
    if (t.UniqueID != 999 || g_manager.cls || g_manager.lvl) { std::printf("004c7ff0_TEST FAIL\n"); return 1; }
    def.LevelLess = 0; t.PBaseDef = &def; t.CreateUID();
    if (g_manager.lvl) { std::printf("004c7ff0_TEST FAIL\n"); return 1; }
    std::printf("004c7ff0_TEST PASS\n");
    return 0;
}