// 00569bc0 behaviour test for std::_Rb_tree::_M_create_node (map<EOnlineScoreboardActivity, vector<pair<float, CWideString> > >)
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
#include <new>
static int g_valueCopies = 0;
enum EOnlineScoreboardActivity { ACTIVITY_A = 2, ACTIVITY_B = 5 };
struct CScoreList {                                         // stands in for the vector value: copy is counted
    int Count;
    CScoreList(int n) : Count(n) { }
    CScoreList(const CScoreList& o) : Count(o.Count) { ++g_valueCopies; }
};
struct TValue {
    EOnlineScoreboardActivity first;
    CScoreList second;
    TValue(EOnlineScoreboardActivity a, const CScoreList& b) : first(a), second(b) { }
    TValue(const TValue& o) : first(o.first), second(o.second) { }
};
struct NodeBase { bool Color; NodeBase* Parent; NodeBase* Left; NodeBase* Right; };
struct Node : NodeBase { TValue Value; };
struct Tree {
    Node* AllocateNode() { return (Node*)std::malloc(sizeof(Node)); }
    Node* CreateNode(const TValue& x)
    {
        Node* tmp = AllocateNode();
        new (&tmp->Value) TValue(x);
        return tmp;
    }
};
static void run()
{
    Tree tree;
    TValue v(ACTIVITY_B, CScoreList(4));
    g_valueCopies = 0;
    Node* n = tree.CreateNode(v);
    CHECK(n != 0);
    CHECK(n->Value.first == ACTIVITY_B);
    CHECK(n->Value.second.Count == 4);
    CHECK(g_valueCopies == 1);
    std::free(n);
}

int main()
{
    run();
    if (g_fail) {
        std::printf("00569bc0_TEST FAIL\n");
        return 1;
    }
    std::printf("00569bc0_TEST PASS\n");
    return 0;
}