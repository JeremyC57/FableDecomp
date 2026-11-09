// Behaviour test for PKCTRRedblackTree::RemoveValue (0x00C19340): the remembered node shortcut, search by value, splice of
// a leaf / one-child node, successor copy for a two-child node, DeleteFixup only for a black spliced node, the node freed
// through the allocator and LastFound cleared.
#include <cstdio>

struct Cmp {
    int EqualValue(const void* a, const void* b) { return *(const int*)a == *(const int*)b; }
    int LessValue(const void* a, const void* b) { return *(const int*)a < *(const int*)b; }
};
struct Node { Node* Left; Node* Right; Node* Parent; long Colour; void* Key; };
static Node* g_freed = 0; static int g_frees = 0;
struct Alloc {
    void Free(void* p) { g_freed = (Node*)p; ++g_frees; }
    void FreeAndClear(Node** pp) { Free(*pp); *pp = 0; }
};
struct Tree {
    Alloc* PAllocator; Node Sentinel; Node* Root; Node* LastFound; Node* fixup; int fixups;
    void DeleteFixup(Node* x) { fixup = x; ++fixups; }
    void RemoveValue(Cmp* comparator, const void* value)
    {
        Node* z;
        Node* y;
        if (LastFound && comparator->EqualValue(value, LastFound->Key)) {
            z = LastFound;
        } else {
            z = Root;
            while (z != &Sentinel) {
                if (comparator->EqualValue(value, z->Key)) break;
                if (comparator->LessValue(value, z->Key)) z = z->Left; else z = z->Right;
            }
        }
        Node* x;
        if (z->Left == &Sentinel || z->Right == &Sentinel) y = z;
        else { y = z->Right; while (y->Left != &Sentinel) y = y->Left; }
        if (y->Left != &Sentinel) x = y->Left; else x = y->Right;
        x->Parent = y->Parent;
        if (y->Parent) { if (y == y->Parent->Left) y->Parent->Left = x; else y->Parent->Right = x; }
        else Root = x;
        if (y != z) z->Key = y->Key;
        if (y->Colour == 0) DeleteFixup(x);
        PAllocator->FreeAndClear(&y);
        LastFound = 0;
    }
};

int main()
{
    //      5(B)
    //    3(B)  8(R)
    //         7(B) 9(B)
    int k[] = { 5, 3, 8, 7, 9 };
    Node n[5]; Tree t; Alloc al; Cmp c;
    t.PAllocator = &al; t.fixups = 0; t.fixup = 0;
    Node* S = &t.Sentinel;
    for (int i = 0; i < 5; ++i) { n[i].Left = n[i].Right = S; n[i].Key = &k[i]; n[i].Colour = 0; }
    n[0].Parent = 0; n[0].Left = &n[1]; n[0].Right = &n[2];
    n[1].Parent = &n[0]; n[2].Parent = &n[0]; n[2].Colour = 1;
    n[2].Left = &n[3]; n[2].Right = &n[4]; n[3].Parent = &n[2]; n[4].Parent = &n[2];
    t.Root = &n[0]; t.LastFound = &n[3];
    bool ok = true;
    int v8 = 8;                          // two children: successor 9 is spliced, its key copied into node 8
    t.RemoveValue(&c, &v8);
    ok = ok && *(int*)n[2].Key == 9 && n[2].Right == S && g_freed == &n[4] && t.fixups == 1 && t.fixup == S && t.LastFound == 0;
    int v7 = 7; t.LastFound = &n[3];    // remembered node
    t.RemoveValue(&c, &v7);
    ok = ok && n[2].Left == S && g_freed == &n[3] && t.fixups == 2;
    n[2].Colour = 1; int v9 = 9;        // red leaf: no fixup
    t.RemoveValue(&c, &v9);
    ok = ok && n[0].Right == S && g_freed == &n[2] && t.fixups == 2 && g_frees == 3;
    if (!ok) { std::printf("00c19340_TEST FAIL\n"); return 1; }
    std::printf("012120c6_TEST PASS\n");
    return 0;
}