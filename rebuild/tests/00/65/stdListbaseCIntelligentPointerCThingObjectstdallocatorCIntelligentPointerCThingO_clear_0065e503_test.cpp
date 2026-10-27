#include <cstdio>
#include <cstdlib>
// Behaviour model of the list clear: destroy and free every node, relink the sentinel to itself.
struct NodeBase { NodeBase* next; NodeBase* prev; };
static int g_destroyed;
struct Node : NodeBase { int v; };
static void clear(NodeBase* head)
{
    NodeBase* cur = head->next;
    while (cur != head) { NodeBase* tmp = cur; cur = cur->next; ++g_destroyed; std::free(tmp); }
    head->next = head; head->prev = head;
}
int main()
{
    NodeBase head; head.next = head.prev = &head;
    for (int i = 0; i < 3; ++i) { Node* n = (Node*)std::malloc(sizeof(Node)); n->v = i; n->next = &head; n->prev = head.prev; head.prev->next = n; head.prev = n; }
    clear(&head);
    if (g_destroyed != 3 || head.next != &head || head.prev != &head) { std::printf("0065e503_TEST FAIL\n"); return 1; }
    clear(&head);
    if (g_destroyed != 3) { std::printf("0065e503_TEST FAIL\n"); return 1; }
    std::printf("0065e503_TEST PASS\n");
    return 0;
}