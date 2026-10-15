// Behaviour test for CHTG_Audience::CHTG_Audience (0x00D72160): quest interface from the parent (+0x40),
// the thing copied, parent and master data stored, the three script things empty.
#include <cstdio>

static int g_baseCtors = 0, g_copies = 0;
struct CBase { CBase() { ++g_baseCtors; } virtual ~CBase() {} };
struct SCountBlock { long refs; };
struct CCountedPointer {
    void* m_pObject; SCountBlock* m_pCount;
    CCountedPointer() : m_pObject(0), m_pCount(0) {}
};
struct CScriptThing : CBase {
    CCountedPointer PImp;
    CScriptThing() : CBase() {}
    CScriptThing(const CScriptThing& o) : CBase() { PImp = o.PImp; ++g_copies; }
};
struct CGameScriptInterface { int dummy; };
struct CQ_HangingTreeGoodScript { CGameScriptInterface* GSI; };
struct CQ_SunnyvaleMasterData { int dummy; };
struct CEntityScriptRoot {
    virtual ~CEntityScriptRoot() {}
    CGameScriptInterface* PQuest;
    CEntityScriptRoot(CGameScriptInterface* q) : PQuest(q) {}
};
struct CEntityScriptBase : CEntityScriptRoot {
    CScriptThing Thing;
    CEntityScriptBase(CQ_HangingTreeGoodScript* parent, const CScriptThing& thing) : CEntityScriptRoot(parent->GSI), Thing(thing) {}
};
struct CHTG_Audience : CEntityScriptBase {
    CQ_HangingTreeGoodScript* ParentClass;
    CQ_SunnyvaleMasterData* PMasterData;
    CScriptThing ChopBlockMarker, ExecutionerGuy, PrisonerGuy;
    CHTG_Audience(CQ_HangingTreeGoodScript* parent, CQ_SunnyvaleMasterData* masterData, const CScriptThing& thing)
        : CEntityScriptBase(parent, thing), ParentClass(parent)
    {
        PMasterData = masterData;
    }
};

int main()
{
    CGameScriptInterface gsi; CQ_HangingTreeGoodScript parent; parent.GSI = &gsi;
    CQ_SunnyvaleMasterData data; SCountBlock block; block.refs = 1;
    CScriptThing thing; thing.PImp.m_pObject = &data; thing.PImp.m_pCount = &block;
    g_baseCtors = 0;
    CHTG_Audience a(&parent, &data, thing);
    bool ok = a.PQuest == &gsi && a.ParentClass == &parent && a.PMasterData == &data && g_copies == 1
        && a.Thing.PImp.m_pObject == &data && a.ChopBlockMarker.PImp.m_pObject == 0 && a.ExecutionerGuy.PImp.m_pCount == 0
        && a.PrisonerGuy.PImp.m_pObject == 0 && g_baseCtors == 4;
    if (!ok) { std::printf("00d72160_TEST FAIL\n"); return 1; }
    std::printf("00d72160_TEST PASS\n");
    return 0;
}