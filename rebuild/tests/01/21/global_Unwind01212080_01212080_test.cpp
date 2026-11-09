// Behaviour test for PKCTRRedblackTree::AddValue (0x00C18E20): BST insert below the right parent, red node with sentinel
// children, root on an empty tree, the duplicate-key trace, InsertFixup on the new node and LastFound cleared.
#include <cstdio>
#include <cstring>

static int g_traces = 0, g_breaks = 0;
static char g_last[256];
struct CString {
    char buf[256];
    CString() { buf[0] = 0; }
    CString& operator<<(const char* t) { std::strcat(buf, t); return *this; }
    CString& operator<<(long v) { char n[32]; std::sprintf(n, "%ld", v); std::strcat(buf, n); return *this; }
};
static void Hook(const char* m) { ++g_traces; std::strcpy(g_last, m); }
static void (*g_pfnAssertHook)(const char*) = Hook;
struct CDebugBreak { CDebugBreak() { ++g_breaks; } };

struct Cmp {
    int EqualValue(const void* a, const void* b) { return *(const int*)a == *(const int*)b; }
    int LessValue(const void* a, const void* b) { return *(const int*)a < *(const int*)b; }
};
struct Node { Node* Left; Node* Right; Node* Parent; long Colour; void* Key; };
static Node g_pool[16]; static int g_used = 0;
struct Alloc { void* Allocate() { return &g_pool[g_used++]; } };
struct Tree {
    Alloc* PAllocator; Node Sentinel; Node* Root; Node* LastFound; Node* fixed;
    void InsertFixup(Node* x) { fixed = x; }
    void AddValue(Cmp* comparator, void* value)
    {
        Node* parent = 0;
        Node* node = Root;
        while (node != &Sentinel) {
            if (comparator->EqualValue(value, node->Key)) {
                {
                    CString line;
                    line << ".\\PKCTRRedblackTree.cpp" << "(" << 298L << ") : " << "Error: Duplicate key" << "\n";
                    if (g_pfnAssertHook) g_pfnAssertHook(line.buf);
                }
                CDebugBreak();
            }
            parent = node;
            if (comparator->LessValue(value, node->Key)) node = node->Left; else node = node->Right;
        }
        node = (Node*)PAllocator->Allocate();
        node->Parent = parent; node->Left = &Sentinel; node->Right = &Sentinel; node->Colour = 1; node->Key = value;
        if (parent) {
            if (comparator->LessValue(value, parent->Key)) parent->Left = node; else parent->Right = node;
        } else Root = node;
        InsertFixup(node);
        LastFound = 0;
    }
};

int main()
{
    Alloc al; Cmp c; Tree t; t.PAllocator = &al; t.Root = &t.Sentinel; t.LastFound = &t.Sentinel; t.fixed = 0;
    int k5 = 5, k3 = 3, k8 = 8, k4 = 4, d5 = 5;
    t.AddValue(&c, &k5);
    bool ok = t.Root == &g_pool[0] && t.Root->Parent == 0 && t.Root->Colour == 1 && t.Root->Left == &t.Sentinel && t.fixed == &g_pool[0] && t.LastFound == 0;
    t.AddValue(&c, &k3); t.AddValue(&c, &k8); t.AddValue(&c, &k4);
    ok = ok && t.Root->Left == &g_pool[1] && t.Root->Right == &g_pool[2] && g_pool[1].Right == &g_pool[3] && g_pool[3].Parent == &g_pool[1];
    ok = ok && g_traces == 0 && g_breaks == 0;
    t.AddValue(&c, &d5);
    ok = ok && g_traces == 1 && g_breaks == 1 && std::strcmp(g_last, ".\\PKCTRRedblackTree.cpp(298) : Error: Duplicate key\n") == 0 && g_pool[2].Left == &g_pool[4];
    if (!ok) { std::printf("00c18e20_TEST FAIL\n"); return 1; }
    std::printf("01212080_TEST PASS\n");
    return 0;
}