// Behaviour test for 0x004CC820 list<CTCBase*>::push_back(x): a new node holding x is linked just before the sentinel,
// so it becomes the last element and the existing order is kept.
#include <cstdio>
#include <cstdlib>

struct Node { Node* next; Node* prev; void* v; };
static void PushBack(Node* s, void* const& x)
{
    Node* tmp = static_cast<Node*>(std::malloc(sizeof(Node)));
    tmp->v = x;
    Node* p = s->prev;
    tmp->next = s; tmp->prev = p; p->next = tmp; s->prev = tmp;
}

int main()
{
    Node s = { &s, &s, 0 };
    int a, b, c;
    void* pa = &a; void* pb = &b; void* pc = &c;
    PushBack(&s, pa); PushBack(&s, pb); PushBack(&s, pc);
    Node* n = s.next;
    if (n->v != pa || n->next->v != pb || n->next->next->v != pc || n->next->next->next != &s || s.prev->v != pc ||
        s.prev->prev->v != pb || n->prev != &s) {
        std::printf("004cc820_TEST FAIL\n"); return 1;
    }
    std::printf("004cc820_TEST PASS\n");
    return 0;
}