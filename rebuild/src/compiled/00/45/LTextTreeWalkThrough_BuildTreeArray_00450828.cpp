#pragma optimize("s",on)
struct Node {
    char pad[8];
    Node* next;   // +0x8
    Node* child;  // +0xc
};

extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

struct CTree {
    void BuildTreeArray(Node* n);
};

void CTree::BuildTreeArray(Node* n)
{
    if (n) {
        do {
            this->BuildTreeArray(n->child);
            Node* next = n->next;
            free(n);
            n = next;
        } while (n);
    }
}