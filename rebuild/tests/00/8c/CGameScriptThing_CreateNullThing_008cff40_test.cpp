// behaviour test for CGameScriptThing::CreateNullThing (0x008CFF40): the result is the manager's null handle copy
#include <cstdio>
class CScriptThing {
public:
    virtual ~CScriptThing() { }
    void* PImp_Data;
    void* PImp_Info;
};
static int gCalls = 0;
static void* gManagerSeen = 0;
class CGameScriptThingManager {
public:
    int Tag;
    CScriptThing GetNullScriptThing() { ++gCalls; gManagerSeen = this; CScriptThing t; t.PImp_Data = &Tag; t.PImp_Info = 0; return t; }
};
template <class T> class CCountedPointer {
public:
    T* Data;
    void* Info;
};
class CQuestManager {
public:
    virtual ~CQuestManager() { }
    CCountedPointer<CGameScriptThingManager> PGameScriptThingManager;
};
static CGameScriptThingManager gManager;
static CQuestManager gQuest;
CQuestManager* g_pQuestManager = &gQuest;
class CGameScriptThing : public CScriptThing {
public:
    static CScriptThing __fastcall CreateNullThing();
};
CScriptThing __fastcall CGameScriptThing::CreateNullThing()
{
    return g_pQuestManager->PGameScriptThingManager.Data->GetNullScriptThing();
}
int main()
{
    gQuest.PGameScriptThingManager.Data = &gManager;
    gQuest.PGameScriptThingManager.Info = 0;
    CScriptThing t = CGameScriptThing::CreateNullThing();
    if (gCalls != 1 || gManagerSeen != &gManager || t.PImp_Data != &gManager.Tag || t.PImp_Info != 0) {
        std::printf("008cff40_TEST FAIL\n");
        return 1;
    }
    std::printf("008cff40_TEST PASS\n");
    return 0;
}
