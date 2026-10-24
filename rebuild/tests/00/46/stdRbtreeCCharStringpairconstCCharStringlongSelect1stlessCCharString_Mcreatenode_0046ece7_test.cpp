// Behaviour test for 0x0046ECE7 _Rb_tree<CCharString, pair<const CCharString, long>>::_M_create_node: one 0x18-byte node, value copied at +0x10
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
struct CCharString {
    char* PData; static int copies;
    CCharString(const char* s) { PData = (char*)std::malloc(std::strlen(s) + 1); std::strcpy(PData, s); }
    CCharString(const CCharString& o) { ++copies; PData = (char*)std::malloc(std::strlen(o.PData) + 1); std::strcpy(PData, o.PData); }
};
int CCharString::copies = 0;
struct MapValue { CCharString first; int second; MapValue(const CCharString& k, int v) : first(k), second(v) {} MapValue(const MapValue& o) : first(o.first), second(o.second) {} };
struct NodeBase { int color; NodeBase* parent; NodeBase* left; NodeBase* right; };
struct Node : NodeBase { MapValue value; };
static Node* CreateNode(const MapValue& x)
{
    Node* tmp = (Node*)std::malloc(sizeof(NodeBase) + sizeof(MapValue));
    new (&tmp->value) MapValue(x);
    return tmp;
}
int main()
{
    MapValue v(CCharString("Bowerstone"), 42);
    int before = CCharString::copies;
    Node* n = CreateNode(v);
    bool ok = n != 0 && CCharString::copies == before + 1 && std::strcmp(n->value.first.PData, "Bowerstone") == 0
        && n->value.first.PData != v.first.PData && n->value.second == 42
        && (char*)&n->value - (char*)n == (int)sizeof(NodeBase);
    if (!ok) { std::printf("0046ece7_TEST FAIL\n"); return 1; }
    std::printf("0046ece7_TEST PASS\n");
    return 0;
}