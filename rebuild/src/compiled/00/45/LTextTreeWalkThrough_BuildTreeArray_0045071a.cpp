#pragma optimize("s",on)
struct Node {
    char pad0[8];
    Node* next;   /* +0x8 */
    int   value;  /* +0xc */
};

struct CTree {
    void Handle(int v);
};

extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

void __fastcall LTextTreeWalkThrough_BuildTreeArray(CTree* self, void* edx, Node* node)
{
    (void)edx;
    while (node != 0) {
        self->Handle(node->value);
        Node* nxt = node->next;
        free(node);
        node = nxt;
    }
}