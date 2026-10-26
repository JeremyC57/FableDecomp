#pragma optimize("s",on)
class TreeNode { public: char pad0[8]; TreeNode* next; TreeNode* child; };
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)
struct LTextTreeWalkThrough {
    void BuildTreeArray(TreeNode* node);
};
void LTextTreeWalkThrough::BuildTreeArray(TreeNode* node)
{
    while (node) {
        this->BuildTreeArray(node->child);
        TreeNode* nxt = node->next;
        free(node);
        node = nxt;
    }
}