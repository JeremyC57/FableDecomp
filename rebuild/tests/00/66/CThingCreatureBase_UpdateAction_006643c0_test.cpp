// behaviour test for CThingCreatureBase::UpdateAction (0x006643C0): update gating, counted copy add-ref/release, queue promotion
#include <cstdio>
static int g_updates = 0, g_finish = 0, g_destroyed = 0, g_set = 0, g_erased = 0;
struct CCreatureActionBase {
    bool AddedActionAnim, Finished; long m_hold; bool m_yield; int m_refsSeen;
    virtual void FinishAction() { ++g_finish; Finished = true; }
    virtual long VSlot07() { return m_hold; }
    virtual bool VSlot27() { return m_yield; }
    void Update();
};
struct SCountBlock { long m_references; void (*m_pfnDestroy)(CCreatureActionBase*); CCreatureActionBase* m_pObject; };
static SCountBlock* g_block = 0;
void CCreatureActionBase::Update() { ++g_updates; m_refsSeen = g_block ? g_block->m_references : -1; }
static void DestroyAction(CCreatureActionBase*) { ++g_destroyed; }
struct CCountedPointer_CCreatureActionBase {
    CCreatureActionBase* Data; SCountBlock* Info;
    CCountedPointer_CCreatureActionBase(CCreatureActionBase* d, SCountBlock* i) : Data(d), Info(i) {}
    CCountedPointer_CCreatureActionBase(const CCountedPointer_CCreatureActionBase& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->m_references; }
    ~CCountedPointer_CCreatureActionBase()
    { if (Info) { if (--Info->m_references == 0) { Info->m_pfnDestroy(Info->m_pObject); /* the real block is deleted here; the test block is static */ } } Data = 0; Info = 0; }
    CCreatureActionBase* operator->() const { return Data; }
};
struct node { node* next; node* prev; CCreatureActionBase* Data; SCountBlock* Info; };
struct CCreatureActionList {
    struct iterator { node* _Ptr; iterator(node* p) : _Ptr(p) {} iterator(const iterator& o) : _Ptr(o._Ptr) {} };
    node* _Myhead;
    bool empty() const { return _Myhead->next == _Myhead; }
    node& front() { return *_Myhead->next; }
    iterator begin() { return iterator(_Myhead->next); }
    iterator erase(iterator w) { ++g_erased; node* n = w._Ptr; n->prev->next = n->next; n->next->prev = n->prev; return iterator(n->next); }
};
struct CThingCreatureBase {
    CCountedPointer_CCreatureActionBase PCurrentAction; CCreatureActionList PQueuedActions;
    CThingCreatureBase(const CCountedPointer_CCreatureActionBase& p) : PCurrentAction(p) {}
    bool IsPerformingAnyAction() { CCreatureActionBase* a = PCurrentAction.Data; if (a != 0) { if (!a->Finished) return true; } return false; }
    bool SetCurrentAction(CCreatureActionBase* a) { ++g_set; PCurrentAction.Data = a; return true; }
    void UpdateAction()
    {
        if (!PCurrentAction.Data) return;
        if (!PCurrentAction->AddedActionAnim && PCurrentAction->VSlot07() && !PCurrentAction->Finished) return;
        { CCountedPointer_CCreatureActionBase current(PCurrentAction); current->Update(); }
        if (!PQueuedActions.empty()) {
            if (!IsPerformingAnyAction() || PCurrentAction->VSlot27()) {
                if (IsPerformingAnyAction()) PCurrentAction->FinishAction();
                if (!PQueuedActions.empty()) { SetCurrentAction(PQueuedActions.front().Data); PQueuedActions.erase(PQueuedActions.begin()); }
            }
        }
    }
};
int main()
{
    static CCreatureActionBase a, q;
    a.AddedActionAnim = false; a.Finished = false; a.m_hold = 1; a.m_yield = false; a.m_refsSeen = 0;
    q.AddedActionAnim = true; q.Finished = false; q.m_hold = 0; q.m_yield = false;
    static SCountBlock blk = { 1, DestroyAction, &a }; g_block = &blk;
    CThingCreatureBase c(CCountedPointer_CCreatureActionBase(&a, &blk));
    blk.m_references = 1;                                   // the member's own reference
    node head; head.next = head.prev = &head; c.PQueuedActions._Myhead = &head;
    int bad = 0;
    c.UpdateAction();                                       // waiting to start (slot 7 holds, anim not added, not finished)
    if (g_updates != 0) bad = 1;
    a.m_hold = 0; c.UpdateAction();                         // updates on a counted copy (2 refs during the call), released after
    if (g_updates != 1 || a.m_refsSeen != 2 || blk.m_references != 1 || g_destroyed != 0 || g_set != 0) bad = 1;
    node n; n.Data = &q; n.Info = 0; n.next = n.prev = &head; head.next = head.prev = &n;
    c.UpdateAction();                                       // queued but current busy and not yielding
    if (g_updates != 2 || g_set != 0 || g_finish != 0) bad = 1;
    a.m_yield = true; c.UpdateAction();                     // yields: finished, front promoted, erased
    if (g_updates != 3 || g_finish != 1 || g_set != 1 || g_erased != 1 || c.PCurrentAction.Data != &q || !c.PQueuedActions.empty()) bad = 1;
    blk.m_references = 1; c.PCurrentAction.Data = &a; c.PCurrentAction.Info = &blk; a.m_hold = 0;
    blk.m_references = 0;                                   // copy brings it to 1, release to 0: destroy runs
    c.UpdateAction();
    if (g_destroyed != 1) bad = 1;
    c.PCurrentAction.Data = 0; c.PCurrentAction.Info = 0; c.UpdateAction();
    if (g_updates != 4) bad = 1;                            // no current action: nothing
    if (bad) { std::printf("006643c0_TEST FAIL\n"); return 1; }
    std::printf("006643c0_TEST PASS\n");
    return 0;
}