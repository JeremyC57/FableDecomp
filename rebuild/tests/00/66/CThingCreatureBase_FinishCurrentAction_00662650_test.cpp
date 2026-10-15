// Behaviour test for CThingCreatureBase::FinishCurrentAction (0x00662650)
#include <cstdio>
static int g_destroyed = 0;
static int g_finishCalls = 0;
static int g_flagDuringCall = -1;
struct SCountBlock {
    long m_references;
    void (*m_pfnDestroy)(void*);
    void* m_pObject;
};
static void DestroyAction(void*) { ++g_destroyed; }
struct CThingCreatureBase;
struct CCreatureActionBase {
    CThingCreatureBase* m_owner;
    unsigned char m_finished;
    virtual void FinishAction();
};
struct CActionPointer {
    CCreatureActionBase* m_pObject;
    SCountBlock*         m_pCount;
    CActionPointer() : m_pObject(0), m_pCount(0) {}
    CActionPointer(const CActionPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    { if (m_pCount) ++m_pCount->m_references; }
    ~CActionPointer()
    {
        if (m_pCount) {
            if (--m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
    }
    CCreatureActionBase* operator->() const { return m_pObject; }
};
struct CThingCreatureBase {
    CActionPointer m_currentAction;
    unsigned char  m_flags15d_0 : 2;
    unsigned char  m_finishingAction : 1;
    unsigned char  m_flags15d_3 : 4;
    unsigned char  m_hiddenOnMiniMap : 1;
    void FinishCurrentAction()
    {
        if (m_currentAction.m_pObject != 0 && !m_currentAction.m_pObject->m_finished) {
            m_finishingAction = true;
            {
                CActionPointer action(m_currentAction);
                action->FinishAction();
            }
            m_finishingAction = false;
        }
    }
};
void CCreatureActionBase::FinishAction()
{
    ++g_finishCalls;
    g_flagDuringCall = m_owner->m_finishingAction;
    // the action drops the creature's own reference while running: the local copy keeps it alive
    SCountBlock* b = m_owner->m_currentAction.m_pCount;
    --b->m_references;
    m_owner->m_currentAction.m_pCount = 0;
}
int main()
{
    CThingCreatureBase c;
    c.m_flags15d_0 = 0; c.m_finishingAction = 0; c.m_flags15d_3 = 0; c.m_hiddenOnMiniMap = 1;
    c.FinishCurrentAction();                                     // no action: nothing happens
    if (g_finishCalls != 0) { std::printf("00662650_TEST FAIL\n"); return 1; }
    CCreatureActionBase a; a.m_owner = &c; a.m_finished = 1;
    SCountBlock* blk = new SCountBlock; blk->m_references = 1; blk->m_pfnDestroy = DestroyAction; blk->m_pObject = &a;
    c.m_currentAction.m_pObject = &a; c.m_currentAction.m_pCount = blk;
    c.FinishCurrentAction();                                     // finished action: skipped
    if (g_finishCalls != 0 || blk->m_references != 1) { std::printf("00662650_TEST FAIL\n"); return 1; }
    a.m_finished = 0;
    c.FinishCurrentAction();                                     // runs, the copy releases the last reference
    if (g_finishCalls != 1 || g_flagDuringCall != 1 || g_destroyed != 1 || c.m_finishingAction != 0 || c.m_hiddenOnMiniMap != 1)
    { std::printf("00662650_TEST FAIL\n"); return 1; }
    c.m_currentAction.m_pObject = 0;
    std::printf("00662650_TEST PASS\n");
    return 0;
}