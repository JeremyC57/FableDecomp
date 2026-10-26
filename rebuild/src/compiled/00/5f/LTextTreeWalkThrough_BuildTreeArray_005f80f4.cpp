#pragma optimize("s", on)
struct AutoTinyTreeNode
{
    char reserved[8];
    AutoTinyTreeNode* next;
    AutoTinyTreeNode* child;
};
struct AutoTinyTreeWalker { char unused; };
struct AutoTinyTreeNodeArgument { AutoTinyTreeNode* node; };
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)
void __fastcall BuildTreeArray(AutoTinyTreeWalker* self, AutoTinyTreeNodeArgument argument)
{
    AutoTinyTreeNode* node = argument.node;
    while (node != 0)
    {
        AutoTinyTreeNodeArgument child = {node->child};
        BuildTreeArray(self, child);
        AutoTinyTreeNode* next = node->next;
        free(node);
        node = next;
    }
}