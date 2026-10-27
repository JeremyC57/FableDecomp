// behaviour test for 0x004405f0 std::list<T>::insert(position, value): allocate a node, copy-construct the value into
// it and splice it in before position, returning the new node
#include <cstdio>
#include <cstdlib>
#include <new>
struct Val { int text; int a, b, c; };
struct NodeBase { NodeBase* next; NodeBase* prev; };
struct Node : NodeBase { Val data; };
static int gAllocs = 0;
static Node* Insert(NodeBase* pos, const Val& x)
{
    Node* tmp = (Node*)std::malloc(sizeof(Node)); ++gAllocs;
    new (&tmp->data) Val(x);
    NodeBase* n = pos; NodeBase* p = n->prev;
    tmp->next = n; tmp->prev = p; p->next = tmp; n->prev = tmp;
    return tmp;
}
int main()
{
    int fails = 0;
    NodeBase head; head.next = head.prev = &head;
    Val v1 = { 1, 2, 3, 4 }; Val v2 = { 5, 6, 7, 8 };
    Node* a = Insert(&head, v1);
    Node* b = Insert(a, v2);
    if (gAllocs != 2) fails |= 1;
    if (head.next != b || b->next != a || a->next != &head) fails |= 2;
    if (head.prev != a || a->prev != b || b->prev != &head) fails |= 4;
    if (a->data.text != 1 || a->data.c != 4 || b->data.a != 6) fails |= 8;
    std::free(a); std::free(b);
    if (fails) { std::printf("004405f0_TEST FAIL %d\n", fails); return 1; }
    std::printf("004405f0_TEST PASS\n");
    return 0;
}