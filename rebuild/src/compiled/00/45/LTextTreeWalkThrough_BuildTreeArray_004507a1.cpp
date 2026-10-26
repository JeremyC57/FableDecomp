#pragma optimize("s",on)
class TreeNode { public:
    void* pad0;
    void* pad4;
    TreeNode* next; /* +0x8 */
    TreeNode* child; /* +0xc */
};

struct CTreeWalk {
    void BuildTreeArray(TreeNode* node);
};

extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

void CTreeWalk::BuildTreeArray(TreeNode* node) {
    while (node) {
        this->BuildTreeArray(node->child);
        TreeNode* next = node->next;
        free(node);
        node = next;
    }
}