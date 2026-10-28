#include <cstdio>
#include <map>
// Behaviour test for CNavNavigableLeafNode::ConvertIndexesToPointers (0x00A779B0).
struct Node { int id; };
struct Leaf {
    const Node** Start; signed char Size;
    void ConvertIndexesToPointers(const std::map<long, Node*>& nodes)
    {
        for (long i = 0; i < Size; ++i) {
            std::map<long, Node*>::const_iterator it = nodes.find((long)(size_t)Start[i]);
            Start[i] = it->second;
        }
    }
};
int main()
{
    Node a = { 1 }, b = { 2 }, c = { 3 };
    std::map<long, Node*> m; m[10] = &a; m[20] = &b; m[5] = &c;
    const Node* slots[3] = { (const Node*)(size_t)20, (const Node*)(size_t)5, (const Node*)(size_t)10 };
    Leaf l = { slots, 3 };
    l.ConvertIndexesToPointers(m);
    bool ok = slots[0] == &b && slots[1] == &c && slots[2] == &a;
    Leaf e = { slots, 0 }; e.ConvertIndexesToPointers(m);
    ok = ok && slots[0] == &b;
    std::printf(ok ? "00a779b0_TEST PASS\n" : "00a779b0_TEST FAIL\n");
    return ok ? 0 : 1;
}