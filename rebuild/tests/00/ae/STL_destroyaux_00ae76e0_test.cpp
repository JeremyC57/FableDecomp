// behaviour test for 0x00AE76E0 std::__destroy_aux over std::set<long>*: destroy each set of the range -- erase every
// tree node (right subtree recursively, then walk left) and free the header
#include <cstdio>
#include <cstdlib>
static int gLive = 0;
static void* Alloc(unsigned int n) { ++gLive; return std::malloc(n); }
static void Free(void* p) { if (p) { --gLive; std::free(p); } }
struct SNode { bool Color; SNode* Parent; SNode* Left; SNode* Right; long Value; };
struct SSet {
    SNode* Header; unsigned int Count;
    void Init() { Header = (SNode*)Alloc(sizeof(SNode)); Header->Color = false; Header->Parent = 0; Header->Left = Header->Right = Header; Count = 0; }
    void Erase(SNode* x) { while (x != 0) { Erase(x->Right); SNode* y = x->Left; Free(x); x = y; } }
    void Clear() { if (Count != 0) { Erase(Header->Parent); Header->Left = Header; Header->Parent = 0; Header->Right = Header; Count = 0; } }
    void Destroy() { Clear(); Free(Header); }
};
static SNode* Leaf(long v) { SNode* n = (SNode*)Alloc(sizeof(SNode)); n->Left = n->Right = 0; n->Value = v; return n; }
static void DestroyAux(SSet* first, SSet* last) { for (; first != last; ++first) first->Destroy(); }
int main()
{
    SSet s[3];
    for (int i = 0; i < 3; ++i) s[i].Init();
    SNode* root = Leaf(5); root->Left = Leaf(2); root->Right = Leaf(9); root->Left->Right = Leaf(3);
    s[0].Header->Parent = root; s[0].Count = 4;
    s[2].Header->Parent = Leaf(1); s[2].Count = 1;
    if (gLive != 8) { std::printf("00ae76e0_TEST FAIL\n"); return 1; }
    DestroyAux(s, s + 3);
    if (gLive != 0) { std::printf("00ae76e0_TEST FAIL\n"); return 1; }
    std::printf("00ae76e0_TEST PASS\n");
    return 0;
}