// Behaviour test for 0x00A7CB50 std::for_each with the node-delete functor: every non-null node except the shared
// blocked node is deleted through its virtual destructor; the functor comes back.
#include <cstdio>
static int g_deleted = 0;
class CNode {
public:
    virtual ~CNode() { ++g_deleted; }
};
static CNode* g_blocked = 0;
class CDeleteNavNode {
public:
    CDeleteNavNode() { }
    CDeleteNavNode(const CDeleteNavNode&) { }
    void operator()(CNode* node) const
    {
        if (node && node != g_blocked)
            delete node;
    }
};
template <class It, class F> F ForEach(It first, It last, F f)
{
    for (; first != last; ++first) f(*first);
    return f;
}
int main()
{
    CNode blocked;
    g_blocked = &blocked;
    CNode* v[5] = { new CNode, 0, &blocked, new CNode, new CNode };
    ForEach(v, v + 5, CDeleteNavNode());
    if (g_deleted != 3) { std::printf("00a7cb50_TEST FAIL\n"); return 1; }
    ForEach(v + 1, v + 3, CDeleteNavNode());
    if (g_deleted != 3) { std::printf("00a7cb50_TEST FAIL\n"); return 1; }
    std::printf("00a7cb50_TEST PASS\n");
    return 0;
}