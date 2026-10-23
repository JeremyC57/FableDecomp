#pragma optimize("sy",on)
struct Node {
    void* pad0;
    void* data;
    Node* next;
    Node* prev;
};
struct CTCCarryable {
    Node* head;
    int count;
};
extern void __stdcall FreeNode_00451b4b(void* data);
void __fastcall CTCCarryable_OnKill(CTCCarryable* self)
{
    if (self->count != 0) {
        Node* n = self->head;
        FreeNode_00451b4b(n->data);
        n = self->head; n->next = n;
        n = self->head; n->data = 0;
        n = self->head; n->prev = n;
        self->count = 0;
    }
}