// Behaviour test for 0x00713660 NTCScriptedControl::CActionStopScripting::Update
#include <cstdio>
struct CAction;
struct SNode { SNode* next; SNode* prev; CAction* data; };
struct CAction {
    bool Activated, Terminated; int cleanups;
    void OnCleanup() { ++cleanups; }
};
struct CList {
    SNode* node; int clears;
    bool empty() const { return node->next == node; }
    unsigned long size() const { unsigned long n = 0; SNode* p; for (p = node->next; p != node; p = p->next) ++n; return n; }
    void clear() { ++clears; node->next = node; node->prev = node; }
};
struct CScript {
    CList PActionList;
    long GetNoActions() { return PActionList.size(); }
    CAction* GetCurrentAction() { if (PActionList.empty()) return 0; return PActionList.node->next->data; }
    void ClearActions() { CAction* a = GetCurrentAction(); if (a && a->Activated) a->OnCleanup(); PActionList.clear(); }
};
struct CStop {
    CScript* PTCScript; bool Terminated;
    void Update() { if (PTCScript->GetNoActions() <= 1) PTCScript->ClearActions(); else Terminated = true; }
};
int main()
{
    SNode sentinel, n1, n2;
    CAction a1, a2; a1.Activated = true; a1.Terminated = false; a1.cleanups = 0; a2 = a1;
    CScript s; s.PActionList.node = &sentinel; s.PActionList.clears = 0;
    sentinel.next = &n1; n1.next = &n2; n2.next = &sentinel; n1.data = &a1; n2.data = &a2;
    CStop stop; stop.PTCScript = &s; stop.Terminated = false;
    stop.Update();
    int ok = stop.Terminated && s.PActionList.clears == 0 && a1.cleanups == 0;
    sentinel.next = &n1; n1.next = &sentinel; stop.Terminated = false;
    stop.Update();
    ok = ok && !stop.Terminated && s.PActionList.clears == 1 && a1.cleanups == 1 && s.PActionList.empty();
    a1.Activated = false; sentinel.next = &n1; n1.next = &sentinel;
    stop.Update();
    ok = ok && a1.cleanups == 1 && s.PActionList.clears == 2;
    stop.Update();
    ok = ok && s.PActionList.clears == 3 && !stop.Terminated;
    if (!ok) { std::printf("00713660_TEST FAIL\n"); return 1; }
    std::printf("00713660_TEST PASS\n");
    return 0;
}