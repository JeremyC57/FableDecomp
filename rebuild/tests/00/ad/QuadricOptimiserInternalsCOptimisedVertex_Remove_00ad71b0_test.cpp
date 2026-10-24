// Behaviour test for 0x00AD71B0 COptimisedVertex::Remove (Exists = false + two STLport clear() calls).
#include <cstdio>

struct SNode { bool Color; SNode* Parent; SNode* Left; SNode* Right; };
static int g_erased[2] = { 0, 0 };
static SNode* g_erasedRoot[2] = { 0, 0 };
struct STree {
    SNode* Header; unsigned int Count; int Kind;
    void Erase(SNode* root) { ++g_erased[Kind]; g_erasedRoot[Kind] = root; }
    void clear()
    {
        if (Count != 0) { Erase(Header->Parent); Header->Left = Header; Header->Parent = 0; Header->Right = Header; Count = 0; }
    }
};
struct COptimisedVertex {
    STree FaceList; bool Exists; unsigned int UniqueIndex; STree Groups; STree PairContractions;
    void Remove() { Exists = false; FaceList.clear(); PairContractions.clear(); }
};

int main()
{
    SNode h1, h2, h3, r1, r3;
    COptimisedVertex v;
    v.FaceList.Header = &h1; v.FaceList.Count = 3; v.FaceList.Kind = 0; h1.Parent = &r1; h1.Left = &r1; h1.Right = &r1;
    v.Groups.Header = &h2; v.Groups.Count = 2; v.Groups.Kind = 1; h2.Parent = &r3; h2.Left = &r3; h2.Right = &r3;
    v.PairContractions.Header = &h3; v.PairContractions.Count = 1; v.PairContractions.Kind = 1; h3.Parent = &r3; h3.Left = &r3; h3.Right = &r3;
    v.Exists = true;
    v.Remove();
    if (v.Exists || g_erased[0] != 1 || g_erasedRoot[0] != &r1 || g_erased[1] != 1 || g_erasedRoot[1] != &r3) { std::printf("00ad71b0_TEST FAIL\n"); return 1; }
    if (v.FaceList.Count != 0 || h1.Parent != 0 || h1.Left != &h1 || h1.Right != &h1) { std::printf("00ad71b0_TEST FAIL\n"); return 1; }
    if (v.PairContractions.Count != 0 || h3.Parent != 0 || h3.Left != &h3 || h3.Right != &h3) { std::printf("00ad71b0_TEST FAIL\n"); return 1; }
    if (v.Groups.Count != 2 || h2.Parent != &r3) { std::printf("00ad71b0_TEST FAIL\n"); return 1; }   // Groups untouched
    // empty trees: no erase call
    v.Exists = true; v.Remove();
    if (v.Exists || g_erased[0] != 1 || g_erased[1] != 1) { std::printf("00ad71b0_TEST FAIL\n"); return 1; }
    std::printf("00ad71b0_TEST PASS\n");
    return 0;
}