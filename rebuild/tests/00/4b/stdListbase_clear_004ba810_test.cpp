// Test for std::_List_base<CGameScriptThingManager::CScriptThingInfo>::clear() -- releases every node's counted pointer, frees the node, relinks the sentinel to itself.
#include <cstdio>
#include <cstdlib>
struct CInfo { long RefCount; };
struct CScriptThingInfo { unsigned long Lo, Hi; CInfo* PScriptThing; };
struct Node { Node* next; Node* prev; CScriptThingInfo data; };
static int g_freed = 0;
static void clear(Node* head)
{
    Node* cur = head->next;
    while (cur != head) {
        Node* tmp = cur;
        cur = cur->next;
        if (tmp->data.PScriptThing) --tmp->data.PScriptThing->RefCount;
        std::free(tmp); ++g_freed;
    }
    head->next = head; head->prev = head;
}
int main()
{
    Node head; head.next = head.prev = &head;
    CInfo infos[3] = { {2}, {1}, {5} };
    for (int i = 0; i < 3; ++i) {
        Node* n = (Node*)std::malloc(sizeof(Node));
        n->data.Lo = i; n->data.Hi = 0; n->data.PScriptThing = &infos[i];
        n->next = &head; n->prev = head.prev; head.prev->next = n; head.prev = n;
    }
    clear(&head);
    if (g_freed != 3 || head.next != &head || head.prev != &head || infos[0].RefCount != 1 || infos[1].RefCount != 0 || infos[2].RefCount != 4)
    { std::printf("004ba810_TEST FAIL\n"); return 1; }
    std::printf("004ba810_TEST PASS\n");
    return 0;
}