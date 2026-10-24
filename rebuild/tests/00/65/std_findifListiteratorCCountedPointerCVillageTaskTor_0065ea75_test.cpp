// Behaviour test for 0x0065EA75 std::__find_if over list<CCountedPointer<CVillageTaskTorch>> (model copy)
#include <cstdio>
struct Thing { int id; };
static int g_live = 0;
struct Task { const Thing* who; bool Involved(const Thing& t) const { return who == &t; } };
struct Counted { Task* obj; Counted(Task* t) : obj(t) { ++g_live; } Counted(const Counted& o) : obj(o.obj) { ++g_live; } ~Counted() { --g_live; } };
struct Pred { const Thing* thing; bool operator()(const Counted& c) const { return c.obj->Involved(*thing); } };
struct Node { Node* next; Node* prev; Task* task; };
Node* find_if_model(Node* first, Node* last, Pred pred)
{
    while (first != last && !pred(Counted(first->task)))
        first = first->next;
    return first;
}
int main()
{
    Thing a = { 1 }, b = { 2 }, c = { 3 };
    Task t1 = { &a }, t2 = { &b };
    Node s, n1, n2;
    s.next = &n1; n1.next = &n2; n2.next = &s; n1.task = &t1; n2.task = &t2;
    Pred pb = { &b }, pc = { &c }, pa = { &a };
    if (find_if_model(s.next, &s, pb) != &n2 || find_if_model(s.next, &s, pa) != &n1 || find_if_model(s.next, &s, pc) != &s || g_live != 0)
        { std::printf("0065ea75_TEST FAIL\n"); return 1; }
    std::printf("0065ea75_TEST PASS\n");
    return 0;
}