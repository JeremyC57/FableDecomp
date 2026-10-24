// Behaviour test for 0x00AC7A90 QuadricOptimiserInternals::CVertexPairContraction::ContainsAllVertices
#include <cstdio>
struct SNode { long color; SNode* parent; SNode* left; SNode* right; unsigned long value; };
static SNode* Increment(SNode* x)
{
    if (x->right != 0) { x = x->right; while (x->left != 0) x = x->left; }
    else { SNode* y = x->parent; while (x == y->right) { x = y; y = y->parent; } if (x->right != y) x = y; }
    return x;
}
struct CSub { unsigned long Index1, Index2; };
struct CContraction {
    CSub* first; CSub* last;
    bool HasVertex(unsigned long index)
    {
        unsigned long i;
        for (i = 0; i < (unsigned long)(last - first); ++i)
            if (first[i].Index1 == index || first[i].Index2 == index) return true;
        return false;
    }
    bool ContainsAllVertices(SNode* header)
    {
        SNode* it;
        for (it = header->left; it != header; it = Increment(it))
            if (!HasVertex(it->value)) return false;
        return true;
    }
};
int main()
{
    // STLport layout: header->parent = root, header->left = leftmost, header->right = rightmost; root->parent = header.
    SNode header, n2, n5, n9;
    header.color = 0; header.parent = &n5; header.left = &n2; header.right = &n9;
    n5.parent = &header; n5.left = &n2; n5.right = &n9; n5.value = 5;
    n2.parent = &n5; n2.left = 0; n2.right = 0; n2.value = 2;
    n9.parent = &n5; n9.left = 0; n9.right = 0; n9.value = 9;
    CSub subs[2] = { { 2, 5 }, { 7, 9 } };
    CContraction c; c.first = subs; c.last = subs + 2;
    int ok = c.ContainsAllVertices(&header);
    subs[1].Index2 = 8;
    ok = ok && !c.ContainsAllVertices(&header);
    SNode empty; empty.parent = 0; empty.left = &empty; empty.right = &empty;
    ok = ok && c.ContainsAllVertices(&empty);
    if (!ok) { std::printf("00ac7a90_TEST FAIL\n"); return 1; }
    std::printf("00ac7a90_TEST PASS\n");
    return 0;
}