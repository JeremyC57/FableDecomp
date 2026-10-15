// Behaviour test for 0x004B43D0: DeactivateQuest(name, reason) deactivates the first active quest with that name (counted copy passed).
#include <cstdio>
#include <cstring>
struct Info { int count; };
struct Quest { const char* name; };
struct CP { Quest* Data; Info* Info_; CP(const CP& o) : Data(o.Data), Info_(o.Info_) { if (Info_) ++Info_->count; } CP(Quest* q, Info* i) : Data(q), Info_(i) {} };
struct Node { Node* next; CP quest; };
struct QM { Node* head; int calls; Quest* last; unsigned int reason; int countDuring;
  void DeactivateQuest(CP q, unsigned int r) { ++calls; last = q.Data; reason = r; countDuring = q.Info_ ? q.Info_->count : -1; }
  void DeactivateQuest(const char* name, unsigned int r) { for (Node* n = head->next; n != head; n = n->next) { Quest* q = n->quest.Data; if (std::strcmp(q->name, name) == 0) { DeactivateQuest(n->quest, r); return; } } } };
int main()
{
    Quest a = {"A"}, b = {"B"}, b2 = {"B"}; Info ia = {1}, ib = {1}, ib2 = {1};
    Node head = {0, CP(0, 0)}; Node n3 = {&head, CP(&b2, &ib2)}, n2 = {&n3, CP(&b, &ib)}, n1 = {&n2, CP(&a, &ia)}; head.next = &n1;
    QM m = {&head, 0, 0, 0, 0};
    m.DeactivateQuest("B", 7);
    bool ok = m.calls == 1 && m.last == &b && m.reason == 7 && m.countDuring == 2;
    m.DeactivateQuest("C", 1); ok = ok && m.calls == 1;
    if (!ok) { std::printf("004b43d0_TEST FAIL\n"); return 1; }
    std::printf("004b43d0_TEST PASS\n");
    return 0;
}
