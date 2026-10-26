#pragma optimize("s",on)
class TreeNode { public:
    char pad[8];
    TreeNode* next;   /* +0x8 */
    TreeNode* child;  /* +0xc */
};

struct LTextTreeWalkThrough { char dummy; };

/* one pointer-sized struct passed BY VALUE -> stays on the stack under fastcall,
   so ecx=this, single stack arg, ret 4 (matches the __fastcall retail). */
struct NodeArg { TreeNode* p; };

extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

void __fastcall LTextTreeWalkThrough_BuildTreeArray(LTextTreeWalkThrough* self, NodeArg arg)
{
    TreeNode* node = arg.p;
    while (node)
    {
        NodeArg ca; ca.p = node->child;
        LTextTreeWalkThrough_BuildTreeArray(self, ca);
        TreeNode* nxt = node->next;
        free(node);
        node = nxt;
    }
}