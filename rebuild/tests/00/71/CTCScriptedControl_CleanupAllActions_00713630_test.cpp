// Behaviour test for CTCScriptedControl::CleanupAllActions (0x00713630): the front action's OnCleanup runs only when it exists and
// has been activated, then every queued action is dropped.
#include <cstdio>
static int g_cleanups = 0;
class CActionBase {
public:
    CActionBase(bool activated) : Activated(activated) { }
    virtual ~CActionBase() { }
    virtual void OnCleanup() { ++g_cleanups; }
    bool IsActive() const { return Activated; }
    bool Activated;
};
struct SNode { SNode* Next; SNode* Prev; CActionBase* Data; };
class CActionList {
public:
    CActionList() { Head.Next = &Head; Head.Prev = &Head; Head.Data = 0; }
    bool empty() const { return Head.Next == &Head; }
    CActionBase* front() const { return Head.Next->Data; }
    void push_back(SNode* n) { n->Prev = Head.Prev; n->Next = &Head; Head.Prev->Next = n; Head.Prev = n; }
    void clear() { Head.Next = &Head; Head.Prev = &Head; }
    SNode Head;
};
class CTCScriptedControl {
public:
    CActionList PActionList;
    void CleanupAllActions()
    {
        if (!PActionList.empty()) {
            CActionBase* action = PActionList.front();
            if (action && action->IsActive())
                action->OnCleanup();
        }
        PActionList.clear();
    }
};
int main()
{
    CActionBase active(true), idle(false);
    SNode a = { 0, 0, &active }, b = { 0, 0, &idle }, n = { 0, 0, 0 };
    CTCScriptedControl c;
    c.CleanupAllActions();                                 // empty list
    int ok = (g_cleanups == 0);
    c.PActionList.push_back(&b); c.PActionList.push_back(&a);
    c.CleanupAllActions();                                 // front not activated
    ok = ok && g_cleanups == 0 && c.PActionList.empty();
    c.PActionList.push_back(&a); c.PActionList.push_back(&b);
    c.CleanupAllActions();                                 // front activated
    ok = ok && g_cleanups == 1 && c.PActionList.empty();
    c.PActionList.push_back(&n);
    c.CleanupAllActions();                                 // null front pointer
    ok = ok && g_cleanups == 1 && c.PActionList.empty();
    if (!ok) { std::printf("00713630_TEST FAIL\n"); return 1; }
    std::printf("00713630_TEST PASS\n");
    return 0;
}