#pragma optimize("s",on)
// __fastcall this=ecx, node=stack. family len45 x5 (template 0042a1b6). self-recursive tree free.
struct Node { char pad[8]; Node* m8; Node* mc; };
struct Tree { void BuildTreeArray(Node* node); };
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)
void Tree::BuildTreeArray(Node* node) {
    while (node) {
        this->BuildTreeArray(node->mc);
        Node* next = node->m8;
        free(node);
        node = next;
    }
}