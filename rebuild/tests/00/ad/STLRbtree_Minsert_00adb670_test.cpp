// Behaviour test for 0x00ADB670 _Rb_tree::_M_insert (map<unsigned long, CGroupInfo>): left/right linking and header upkeep.
#include <cstdio>
#include <cstdlib>

struct SNode { bool Color; SNode* Parent; SNode* Left; SNode* Right; unsigned long Key; unsigned long Count; long BlockIndex; };
struct SValue { unsigned long Key; unsigned long Count; long BlockIndex; };
static int g_rebalances = 0;
static void Rebalance(SNode* z, SNode*& root) { ++g_rebalances; if (root == z) z->Color = false; }   // stand-in for 0x0042971D
struct STree {
    SNode* Header; unsigned int Count;
    SNode*& Root() { return Header->Parent; }
    SNode* Create(const SValue& v) { SNode* n = (SNode*)std::malloc(sizeof(SNode)); n->Color = true; n->Key = v.Key; n->Count = v.Count; n->BlockIndex = v.BlockIndex; return n; }
    // reconstructed body
    SNode* Insert(SNode* x, SNode* y, const SValue& v, SNode* w)
    {
        SNode* z;
        if (y == Header || (w == 0 && (x != 0 || v.Key < y->Key))) {
            z = Create(v); y->Left = z;
            if (y == Header) { Root() = z; Header->Right = z; }
            else if (y == Header->Left) Header->Left = z;
        } else {
            z = Create(v); y->Right = z;
            if (y == Header->Right) Header->Right = z;
        }
        z->Parent = y; z->Left = 0; z->Right = 0;
        Rebalance(z, Root());
        ++Count;
        return z;
    }
};

int main()
{
    SNode header; header.Color = false; header.Parent = 0; header.Left = &header; header.Right = &header;
    STree t; t.Header = &header; t.Count = 0;
    SValue v5 = { 5, 1, 2 }, v3 = { 3, 0, -1 }, v9 = { 9, 4, 7 }, v7 = { 7, 0, 0 };
    SNode* n5 = t.Insert(0, &header, v5, 0);                      // first node: root, leftmost and rightmost
    if (header.Parent != n5 || header.Left != n5 || header.Right != n5 || n5->Parent != &header || t.Count != 1) { std::printf("00adb670_TEST FAIL\n"); return 1; }
    if (n5->Key != 5 || n5->Count != 1 || n5->BlockIndex != 2 || n5->Left != 0 || n5->Right != 0) { std::printf("00adb670_TEST FAIL\n"); return 1; }
    SNode* n3 = t.Insert(0, n5, v3, 0);                            // 3 < 5: left child, new leftmost
    if (n5->Left != n3 || header.Left != n3 || header.Right != n5 || n3->Parent != n5) { std::printf("00adb670_TEST FAIL\n"); return 1; }
    SNode* n9 = t.Insert(0, n5, v9, 0);                            // 9 > 5: right child, new rightmost
    if (n5->Right != n9 || header.Right != n9 || header.Left != n3) { std::printf("00adb670_TEST FAIL\n"); return 1; }
    SNode* n7 = t.Insert(0, n9, v7, n9);                           // w != 0 forces the right side even though 7 < 9
    if (n9->Right != n7 || header.Right != n7 || n9->Left != 0) { std::printf("00adb670_TEST FAIL\n"); return 1; }
    if (t.Count != 4 || g_rebalances != 4) { std::printf("00adb670_TEST FAIL\n"); return 1; }
    std::free(n3); std::free(n5); std::free(n7); std::free(n9);
    std::printf("00adb670_TEST PASS\n");
    return 0;
}