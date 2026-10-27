// 005791cc behaviour test for std::_Rb_tree<CUnprovenKey_005791cc, ...>::begin()
#include <cstdio>
#include <cstdlib>
#ifndef _MSC_VER
#ifndef __fastcall
#define __fastcall
#endif
#ifndef __cdecl
#define __cdecl
#endif
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { std::printf("check failed at line %d\n", __LINE__); g_fail = 1; } } while (0)
struct NodeBase { bool Color; NodeBase* Parent; NodeBase* Left; NodeBase* Right; };
struct Iterator { NodeBase* Node; Iterator(NodeBase* n) : Node(n) { } };
struct Tree {
    NodeBase* Header;
    unsigned int NodeCount;
    NodeBase*& Leftmost() const { return Header->Left; }
    Iterator begin() { return Iterator(Leftmost()); }
};
static void run()
{
    NodeBase header = { false, 0, 0, 0 };
    NodeBase a = { true, 0, 0, 0 }, b = { false, 0, 0, 0 };
    Tree t = { &header, 0 };
    header.Left = &header;                                   // empty tree: leftmost is the header
    CHECK(t.begin().Node == &header);
    header.Parent = &b; b.Left = &a; header.Left = &a; t.NodeCount = 2;
    CHECK(t.begin().Node == &a);
}

int main()
{
    run();
    if (g_fail) {
        std::printf("005791cc_TEST FAIL\n");
        return 1;
    }
    std::printf("005791cc_TEST PASS\n");
    return 0;
}