// Circular-list teardown: free every node, re-point the sentinel at itself, then free
// Leaf: SortTreeRecursively
// the sentinel. __fastcall this=ecx, no args.
struct Node { Node* next; Node* prev; };
struct List { Node* head; void Destroy(); };
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)
void List::Destroy() {
    Node* n = this->head->next;
    while (n != this->head) {
        Node* cur = n;
        n = n->next;
        free(cur);
    }
    this->head->next = this->head;
    this->head->prev = this->head;
    Node* h = this->head;
    if (h) free(h);
}