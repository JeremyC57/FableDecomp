// Behaviour test for 0x004BAF10 _Rb_tree<CCharString,...>::equal_range(k): returns {lower_bound(k), upper_bound(k)};
// upper_bound is evaluated first (right-to-left argument order), both from the same key.
#include <cstdio>
#include <cstring>

struct Node { const char* key; };
static int g_order[2]; static int g_n = 0;
struct Tree {
    Node* nodes; int count;
    Node* _M_lower_bound(const char* const& k) const { g_order[g_n++] = 1; int i = 0; while (i < count && std::strcmp(nodes[i].key, k) < 0) ++i; return nodes + i; }
    Node* _M_upper_bound(const char* const& k) const { g_order[g_n++] = 2; int i = 0; while (i < count && std::strcmp(nodes[i].key, k) <= 0) ++i; return nodes + i; }
    struct Range { Node* first; Node* second; Range(Node* a, Node* b) : first(a), second(b) {} };
    Range equal_range(const char* const& k)
    {
        Node* upper = _M_upper_bound(k);            // right-to-left evaluation of the pair arguments
        Node* lower = _M_lower_bound(k);
        return Range(lower, upper);
    }
};

int main()
{
    Node n[5] = { {"a"}, {"b"}, {"b"}, {"c"}, {"d"} };
    Tree t = { n, 5 };
    const char* k = "b";
    Tree::Range r = t.equal_range(k);
    if (r.first != n + 1 || r.second != n + 3 || g_order[0] != 2 || g_order[1] != 1) { std::printf("004baf10_TEST FAIL\n"); return 1; }
    const char* z = "bb";
    r = t.equal_range(z);
    if (r.first != n + 3 || r.second != n + 3) { std::printf("004baf10_TEST FAIL\n"); return 1; }
    std::printf("004baf10_TEST PASS\n");
    return 0;
}