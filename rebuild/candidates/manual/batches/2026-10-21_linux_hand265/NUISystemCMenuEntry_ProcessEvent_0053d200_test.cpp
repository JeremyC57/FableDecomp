#include <cstdio>
// Behaviour test for NUISystem::CMenuEntry::ProcessEvent (0x0053D200): front-end gating, the input-delay gate, the def-enabled
// CClickable pass-through, select on events 2/5 and the conditional unselect on events 3/4.
struct CDef { long RefCount; long Destroyed; bool KeepSelection; bool ClickableEnabled; };
struct CDefPointer {
    CDef* m_p;
    CDefPointer(CDef* p) : m_p(p) { if (m_p) m_p->RefCount++; }
    CDefPointer(const CDefPointer& o) : m_p(o.m_p) { if (m_p) m_p->RefCount++; }
    ~CDefPointer() { if (m_p) { if (--m_p->RefCount == 0) m_p->Destroyed++; m_p = 0; } }
    CDef* operator->() const { return m_p; }
};
struct SListNode { SListNode* _M_next; };
static bool g_IgnoreInputDelay = false;
static double g_LastInputTime = 0.0;
static double g_Now = 100.0;
static SListNode g_sentinel;
static SListNode* g_PendingList = &g_sentinel;
static double GFGetTime() { return g_Now; }
struct CFrontEndManager { bool Block25; bool Busy; bool IsBlockingEvent25() { return Block25; } };
static CFrontEndManager g_fe;
static CFrontEndManager* GetInstance() { return &g_fe; }
struct CComponent {
    CDef* Def; long Type; float Time; CComponent* PParent; float InputDelay; float LastEventTime;
    int Selected, Unselected, Clicked;
    CDefPointer GetDef() { return CDefPointer(Def); }
    long GetType() { return Type; }
    CComponent* GetParentList() const { return PParent; }
    void OnSelected() { Selected++; }
    void OnUnselected() { Unselected++; }
    void ClickableProcessEvent(int) { Clicked++; }
    void ProcessEvent(int event)
    {
        if (GetInstance()->IsBlockingEvent25() && event == 25) return;
        if (GetInstance()->Busy) return;
        if (event != 25 && !g_IgnoreInputDelay) {
            if (Time - LastEventTime < InputDelay) return;
            if (GFGetTime() - g_LastInputTime < InputDelay) return;
        }
        if (GetDef()->ClickableEnabled) ClickableProcessEvent(event);
        switch (event) {
        case 2: case 5: OnSelected(); return;
        case 3: case 4: {
            bool keep = PParent && PParent->GetType() == 8 && GetParentList() && !GetParentList()->GetDef()->KeepSelection
                && g_PendingList->_M_next != g_PendingList;
            if (!keep) OnUnselected();
        } break;
        }
    }
};
static int Fail() { std::printf("0053d200_TEST FAIL\n"); return 1; }
int main()
{
    g_sentinel._M_next = &g_sentinel;
    CDef d1 = { 1, 0, false, true }, d2 = { 1, 0, false, false };
    CComponent parent = { &d2, 8, 0, 0, 0, 0, 0, 0, 0 };
    CComponent e = { &d1, 0, 10.0f, &parent, 1.0f, 0.0f, 0, 0, 0 };
    e.ProcessEvent(2);
    if (e.Selected != 1 || e.Clicked != 1) return Fail();
    e.ProcessEvent(3);                       // pending list empty -> unselect
    if (e.Unselected != 1) return Fail();
    SListNode other; other._M_next = &g_sentinel; g_sentinel._M_next = &other;
    e.ProcessEvent(4);                       // type-8 parent, def not keeping, list pending -> keep
    if (e.Unselected != 1) return Fail();
    d2.KeepSelection = true;
    e.ProcessEvent(4);
    if (e.Unselected != 2) return Fail();
    e.LastEventTime = 9.5f;                  // inside the delay
    e.ProcessEvent(5);
    if (e.Selected != 1) return Fail();
    e.ProcessEvent(25);                      // event 25 skips the delay gate
    if (e.Clicked != 5) return Fail();
    g_fe.Block25 = true;
    e.ProcessEvent(25);
    if (e.Clicked != 5) return Fail();
    g_fe.Busy = true; e.LastEventTime = 0.0f;
    e.ProcessEvent(2);
    if (e.Selected != 1) return Fail();
    if (d1.RefCount != 1 || d2.RefCount != 1 || d1.Destroyed || d2.Destroyed) return Fail();
    std::printf("0053d200_TEST PASS\n");
    return 0;
}
