// Behaviour test for _Rb_tree<unsigned __int64, pair<const unsigned __int64, CThing*>>::_M_create_node (0x006F1250): malloc a node and copy-construct the value.
#include <cstdio>
#include <cstdlib>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
class CThing { public: int Id; };
struct Value { u64 first; CThing* second; };
struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
struct Node : NodeBase { Value value; };
static Node* create_node(const Value& x)
{
    Node* tmp = (Node*)std::malloc(sizeof(Node));
    tmp->value = x;
    return tmp;
}
int main()
{
    CThing t;
    Value v; v.first = ((u64)0x01234567 << 32) | 0x89ABCDEF; v.second = &t;
    Node* n = create_node(v);
    if (!n || n->value.first != (((u64)0x01234567 << 32) | 0x89ABCDEF) || n->value.second != &t) { std::printf("006f1250_TEST FAIL\n"); return 1; }
    std::free(n);
    std::printf("006f1250_TEST PASS\n");
    return 0;
}