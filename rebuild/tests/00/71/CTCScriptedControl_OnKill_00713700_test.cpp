// Behaviour test for CTCScriptedControl::OnKill (0x00713700): the inlined CleanupAllActions, then the CTCBase::OnKill base call.
static int g_baseKills = 0;
class CTCBase { public: virtual ~CTCBase() { } virtual void OnKill() { ++g_baseKills; } };
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
class CTCScriptedControl : public CTCBase {
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
    virtual void OnKill()
    {
        CleanupAllActions();
        CTCBase::OnKill();
    }
};
int main()
{
    CActionBase active(true), idle(false);
    SNode a = { 0, 0, &active }, b = { 0, 0, &idle }, n = { 0, 0, 0 };
    CTCScriptedControl c;
    c.OnKill();                                 // empty list
    int ok = (g_cleanups == 0);
    c.PActionList.push_back(&b); c.PActionList.push_back(&a);
    c.OnKill();                                 // front not activated
    ok = ok && g_cleanups == 0 && c.PActionList.empty();
    c.PActionList.push_back(&a); c.PActionList.push_back(&b);
    c.OnKill();                                 // front activated
    ok = ok && g_cleanups == 1 && c.PActionList.empty();
    c.PActionList.push_back(&n);
    c.OnKill();                                 // null front pointer
    ok = ok && g_cleanups == 1 && c.PActionList.empty();
    ok = ok && g_baseKills == 4;
    if (!ok) { std::printf("00713700_TEST FAIL\n"); return 1; }
    std::printf("00713700_TEST PASS\n");
    return 0;
}