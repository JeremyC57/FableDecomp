#pragma optimize("s",on)
struct Node {
    char pad0[8];
    Node* next;   /* +0x8 */
    Node* child;  /* +0xc */
};

extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

struct CFoo {
    void BuildTreeArray(Node* node);
};

void CFoo::BuildTreeArray(Node* node)
{
    while (node)
    {
        BuildTreeArray(node->child);
        Node* nxt = node->next;
        free(node);
        node = nxt;
    }
}