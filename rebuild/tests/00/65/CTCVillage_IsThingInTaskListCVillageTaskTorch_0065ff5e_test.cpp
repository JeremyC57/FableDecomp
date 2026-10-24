// Behaviour test for 0x0065FF5E CTCVillage::IsThingInTaskList<CVillageTaskTorch> (model copy)
#include <cstdio>
struct Thing { int id; };
struct Task { const Thing* who; };
struct Node { Node* next; Node* prev; Task* task; };
struct List { Node* node; };
bool IsThingInTaskList(const Thing& thing, List& tasks)
{
    Node* last = tasks.node;
    Node* it = last->next;
    while (it != last && it->task->who != &thing) it = it->next;
    return it != tasks.node;
}
int main()
{
    Thing a = { 1 }, b = { 2 };
    Task t1 = { &a };
    Node s, n1; s.next = &n1; n1.next = &s; n1.task = &t1;
    List l; l.node = &s;
    if (!IsThingInTaskList(a, l) || IsThingInTaskList(b, l)) { std::printf("0065ff5e_TEST FAIL\n"); return 1; }
    s.next = &s;
    if (IsThingInTaskList(a, l)) { std::printf("0065ff5e_TEST FAIL\n"); return 1; }
    std::printf("0065ff5e_TEST PASS\n");
    return 0;
}