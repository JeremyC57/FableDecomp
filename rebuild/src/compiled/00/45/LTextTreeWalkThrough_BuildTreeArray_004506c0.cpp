#pragma optimize("s",on)
class TreeNode { public:
    char pad0[8];
    TreeNode* next;   // +0x8
    TreeNode* child;  // +0xc
};

extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

struct CTextTreeWalkThrough {
    void BuildTreeArray(TreeNode* node);
};

void CTextTreeWalkThrough::BuildTreeArray(TreeNode* node)
{
    if (node) {
        do {
            this->BuildTreeArray(node->child);
            TreeNode* next = node->next;
            free(node);
            node = next;
        } while (node);
    }
}