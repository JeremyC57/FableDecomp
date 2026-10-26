#pragma optimize("s",on)
class TreeNode { public: int pad0; int pad4; TreeNode* next; TreeNode* child; };
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

struct LTextTreeWalkThrough {
    void BuildTreeArray(TreeNode* node);
};

void LTextTreeWalkThrough::BuildTreeArray(TreeNode* node)
{
    while (node) {
        BuildTreeArray(node->child);
        TreeNode* nxt = node->next;
        free(node);
        node = nxt;
    }
}