// 005d8548 behaviour test for std::map<unsigned long, long>::find
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
struct Node : NodeBase { unsigned long Key; long Value; };
struct Iterator { NodeBase* Node; Iterator(NodeBase* n) : Node(n) { } };
struct Tree {
    NodeBase* Header;
    Node* Nodes; int Count;
    Node* FindNode(const unsigned long& k) const              // stands in for _M_find 0x005D83B3: the node or the header
    {
        for (int i = 0; i < Count; ++i)
            if (Nodes[i].Key == k)
                return &Nodes[i];
        return (Node*)Header;
    }
    Iterator find(const unsigned long& k) { return Iterator(FindNode(k)); }
};
struct Map {
    Tree t;
    Iterator find(const unsigned long& k) { return t.find(k); }
};
static void run()
{
    Node header; header.Key = 0; header.Value = 0;
    Node nodes[3];
    nodes[0].Key = 4; nodes[0].Value = 40; nodes[1].Key = 9; nodes[1].Value = 90; nodes[2].Key = 12; nodes[2].Value = 120;
    Map m; m.t.Header = &header; m.t.Nodes = nodes; m.t.Count = 3;
    CHECK(m.find(9).Node == &nodes[1]);
    CHECK(((Node*)m.find(12).Node)->Value == 120);
    CHECK(m.find(5).Node == &header);
}

int main()
{
    run();
    if (g_fail) {
        std::printf("005d8548_TEST FAIL\n");
        return 1;
    }
    std::printf("005d8548_TEST PASS\n");
    return 0;
}