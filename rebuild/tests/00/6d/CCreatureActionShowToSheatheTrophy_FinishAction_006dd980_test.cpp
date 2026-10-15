// Behaviour test for 0x006DD980 CCreatureAction_ShowToSheatheTrophy::FinishAction: base finish, then a stack sheathe action handed to the creature
// and the counted follow-up pointer released when it goes out of scope.
#include <cstdio>
static int g_log[16]; static int g_n = 0;
static void Log(int v) { if (g_n < 16) g_log[g_n++] = v; }
struct CCreatureActionBase;
struct CThing { int m_kind; bool SetCurrentAction(CCreatureActionBase* action); };
static int g_destroyed = 0;
struct SCountBlock { int m_references; void (*m_pfnDestroy)(CCreatureActionBase*); CCreatureActionBase* m_pObject; };
static void DestroyAction(CCreatureActionBase*) { ++g_destroyed; }
struct CCountedPointer {
    CCreatureActionBase* m_pObject; SCountBlock* m_pCount;
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; }
        }
        m_pObject = 0; m_pCount = 0;
    }
};
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
struct CCreatureActionBase {
    CBaseIntelligentPointer PCreature;
    virtual ~CCreatureActionBase() { Log(9); }
};
static SCountBlock* g_nextBlock = 0;
struct CCreatureAction_SheatheItemToInventory : CCreatureActionBase {
    CCountedPointer PNextAction; int m_kind;
    CCreatureAction_SheatheItemToInventory(CThing* owner, CCreatureActionBase*, int kind) : m_kind(kind)
    { PCreature.m_pItem = owner; PNextAction.m_pObject = 0; PNextAction.m_pCount = g_nextBlock; }
};
struct CCreatureAction_SheatheTrophy : CCreatureAction_SheatheItemToInventory {
    CCreatureAction_SheatheTrophy(CThing* owner) : CCreatureAction_SheatheItemToInventory(owner, 0, 1002) { }
};
bool CThing::SetCurrentAction(CCreatureActionBase* action) { m_kind = ((CCreatureAction_SheatheItemToInventory*)action)->m_kind; Log(2); return true; }
struct CAction : CCreatureActionBase {
    void FinishAction()
    {
        Log(1);   // CCreatureAction_ShowTrophy::FinishAction
        CCreatureAction_SheatheTrophy sheathe(PCreature.GetPItem());
        PCreature.GetPItem()->SetCurrentAction(&sheathe);
    }
};
static int Fail() { std::printf("006dd980_TEST FAIL\n"); return 1; }
int main()
{
    CThing me; me.m_kind = 0;
    CAction* a = new CAction; a->PCreature.m_pItem = &me;
    g_nextBlock = new SCountBlock; g_nextBlock->m_references = 1; g_nextBlock->m_pfnDestroy = DestroyAction; g_nextBlock->m_pObject = 0;
    a->FinishAction();
    if (g_n != 3 || g_log[0] != 1 || g_log[1] != 2 || g_log[2] != 9 || me.m_kind != 1002 || g_destroyed != 1) return Fail();
    SCountBlock shared; shared.m_references = 2; shared.m_pfnDestroy = DestroyAction; shared.m_pObject = 0;
    g_nextBlock = &shared; g_n = 0;
    a->FinishAction();   // still referenced elsewhere: no destroy
    if (shared.m_references != 1 || g_destroyed != 1) return Fail();
    std::printf("006dd980_TEST PASS\n");
    return 0;
}