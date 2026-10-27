#include <cstdio>
// Behaviour model of list::remove_if with CIsTask: drop every node whose task is the predicate's task.
struct Node { Node* next; Node* prev; void* task; };
static int remove_if(Node* head, void* task)
{
    int erased = 0;
    Node* first = head->next;
    while (first != head) {
        Node* next = first->next;
        if (first->task == task) { first->prev->next = first->next; first->next->prev = first->prev; ++erased; }
        first = next;
    }
    return erased;
}
int main()
{
    int a, b; Node head, n[4];
    void* tasks[4] = { &a, &b, &a, &b };
    head.next = &n[0]; head.prev = &n[3];
    for (int i = 0; i < 4; ++i) { n[i].task = tasks[i]; n[i].next = i < 3 ? &n[i + 1] : &head; n[i].prev = i > 0 ? &n[i - 1] : &head; }
    if (remove_if(&head, &a) != 2 || head.next != &n[1] || n[1].next != &n[3] || n[3].next != &head) { std::printf("00660298_TEST FAIL\n"); return 1; }
    if (remove_if(&head, &a) != 0) { std::printf("00660298_TEST FAIL\n"); return 1; }
    std::printf("00660298_TEST PASS\n");
    return 0;
}