// Behaviour test for 0x00991240 CThreadedFileManager::CPendingObjectMap::CPendingObjectMap(): a fresh red header whose
// root is null and whose leftmost/rightmost point at itself, node count 0, CurrentPosition null; two maps get
// distinct headers.
#include <cstdio>
#include <cstdlib>
struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
struct Tree {
    NodeBase* header; unsigned int count; char compare;
    Tree() : header(0)
    {
        header = (NodeBase*)malloc(sizeof(NodeBase) + 8);
        count = 0;
        header->color = false; header->parent = 0; header->left = header; header->right = header;
    }
};
struct Iter { NodeBase* node; Iter() { node = 0; } };
struct CPendingObjectMap { Tree Objects; Iter CurrentPosition; };
int main()
{
    int fails = 0;
    CPendingObjectMap a, b;
    CPendingObjectMap* m[2] = { &a, &b };
    for (int i = 0; i < 2; ++i) {
        NodeBase* h = m[i]->Objects.header;
        if (!h || h->color || h->parent || h->left != h || h->right != h || m[i]->Objects.count || m[i]->CurrentPosition.node) ++fails;
    }
    if (a.Objects.header == b.Objects.header) ++fails;
    free(a.Objects.header); free(b.Objects.header);
    std::printf(fails ? "00991240_TEST FAIL\n" : "00991240_TEST PASS\n");
    return fails ? 1 : 0;
}