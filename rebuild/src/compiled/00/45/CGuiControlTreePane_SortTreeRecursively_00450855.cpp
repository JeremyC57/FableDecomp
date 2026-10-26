#pragma optimize("s",on)
// Leaf: SortTreeRecursively
struct Node {
    Node* next;   // +0x0
    Node* prev;   // +0x4
};
struct Tree {
    Node* sentinel;  // +0x0 : pointer to the list sentinel node
};

extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

void __fastcall CGuiControlTreePane_SortTreeRecursively(Tree* self)
{
    Node* head = self->sentinel;
    Node* n = head->next;
    if (n != head) {
        do {
            Node* cur = n;
            n = n->next;
            free(cur);
        } while (n != self->sentinel);
    }
    self->sentinel->next = self->sentinel;
    self->sentinel->prev = self->sentinel;
}