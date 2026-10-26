#pragma optimize("s",on)
class TreeNode;
struct NodePtr { TreeNode* p; };
class TreeNode { public:
    char pad0[8];
    NodePtr sib;   /* +0x8 */
    NodePtr child; /* +0xc */
};
struct LTextTree { char dummy; };
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)
/* __fastcall: this in ecx, node pushed on stack (ret 0x4). NodePtr-by-value keeps edx free. */
void __fastcall LTextTree_BuildTreeArray(LTextTree* self, NodePtr node)
{
    while (node.p) {
        LTextTree_BuildTreeArray(self, node.p->child);
        NodePtr next = node.p->sib;
        free(node.p);
        node = next;
    }
}