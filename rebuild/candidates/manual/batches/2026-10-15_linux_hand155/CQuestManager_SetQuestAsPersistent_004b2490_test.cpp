// Behaviour test for 0x004B2490: every active quest with the name gets the flag; its live card (if any) is told too.
#include <cstdio>
#include <cstring>
struct Thing { bool Dead; int calls; bool last; void SetAsGameStatePersisted(bool p) { ++calls; last = p; } };
struct Quest { const char* name; bool Persistent; };
struct Node { Node* next; Quest* quest; };
struct QM { Node head; Thing* card; int lookups;
  Thing* GetActiveQuestCardFromScriptName(const char*) { ++lookups; return card; }
  void SetQuestAsPersistent(const char* name, bool p) {
    for (Node* n = head.next; n != &head; n = n->next) { Quest* q = n->quest;
      if (std::strcmp(q->name, name) == 0) { q->Persistent = p; Thing* c = GetActiveQuestCardFromScriptName(name); if (c && !c->Dead) c->SetAsGameStatePersisted(p); } } } };
int main()
{
    Quest a = {"Q1", false}, b = {"Q2", false}, c = {"Q1", false};
    QM m; Node n3 = {&m.head, &c}, n2 = {&n3, &b}, n1 = {&n2, &a}; m.head.next = &n1; Thing t = {false, 0, false}; m.card = &t; m.lookups = 0;
    m.SetQuestAsPersistent("Q1", true);
    bool ok = a.Persistent && !b.Persistent && c.Persistent && t.calls == 2 && t.last && m.lookups == 2;
    t.Dead = true; m.SetQuestAsPersistent("Q2", true); ok = ok && b.Persistent && t.calls == 2;
    m.card = 0; m.SetQuestAsPersistent("Q1", false); ok = ok && !a.Persistent && t.calls == 2;
    if (!ok) { std::printf("004b2490_TEST FAIL\n"); return 1; }
    std::printf("004b2490_TEST PASS\n");
    return 0;
}
