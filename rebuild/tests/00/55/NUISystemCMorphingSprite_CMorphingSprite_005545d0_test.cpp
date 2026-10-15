#include <cstdio>
#include <cstdlib>
// CMorphingSprite constructor model: RenderCount 0, a self-linked list sentinel, an empty tree header, then GetDataFromDef.
struct SListNode { SListNode* next; SListNode* prev; void* data; };
struct STreeNode { char color; STreeNode* parent; STreeNode* left; STreeNode* right; unsigned int k, v; };
struct CMorphingSprite {
    long RenderCount; SListNode* list; STreeNode* header; unsigned long count; int loaded;
    CMorphingSprite() : RenderCount(0), loaded(0)
    {
        list = 0;
        SListNode* node = (SListNode*)std::malloc(sizeof(SListNode));
        node->next = node; node->prev = node; list = node;
        header = 0;
        header = (STreeNode*)std::malloc(sizeof(STreeNode));
        count = 0;
        header->color = 0; header->parent = 0; header->left = header; header->right = header;
        GetDataFromDef();
    }
    void GetDataFromDef() { ++loaded; }
};
int main()
{
    CMorphingSprite s;
    bool ok = s.RenderCount == 0 && s.list->next == s.list && s.list->prev == s.list && s.count == 0
              && s.header->parent == 0 && s.header->left == s.header && s.header->right == s.header && s.loaded == 1;
    std::free(s.list); std::free(s.header);
    if (!ok) { std::printf("005545d0_TEST FAIL\n"); return 1; }
    std::printf("005545d0_TEST PASS\n");
    return 0;
}