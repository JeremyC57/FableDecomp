#include <cstdio>
// Behaviour test: set<long>::equal_range -- the lower bound (first key not below k) and upper bound (first key above k)
// found by two root-to-leaf descents.
struct Node { Node* _M_parent; Node* _M_left; Node* _M_right; long Value; };
struct Pair { Node* first; Node* second; };
struct Tree {
    Node Header; // parent = root
    Node* _M_lower_bound(const long& k) const
    {
        Node* y = (Node*)&Header;
        Node* x = Header._M_parent;
        while (x != 0)
            if (!(x->Value < k))
                y = x, x = x->_M_left;
            else
                x = x->_M_right;
        return y;
    }
    Node* _M_upper_bound(const long& k) const
    {
        Node* y = (Node*)&Header;
        Node* x = Header._M_parent;
        while (x != 0)
            if (k < x->Value)
                y = x, x = x->_M_left;
            else
                x = x->_M_right;
        return y;
    }
    Pair equal_range(const long& k) { Pair p; p.first = _M_lower_bound(k); p.second = _M_upper_bound(k); return p; }
};
int main()
{
    Node n[5];
    long vals[5] = { 10, 20, 30, 40, 50 };
    for (int i = 0; i < 5; ++i) { n[i].Value = vals[i]; n[i]._M_left = n[i]._M_right = 0; }
    Tree t;
    t.Header._M_parent = &n[2]; n[2]._M_parent = &t.Header;
    n[2]._M_left = &n[1]; n[1]._M_left = &n[0]; n[2]._M_right = &n[3]; n[3]._M_right = &n[4];
    Pair p = t.equal_range(30);
    if (p.first != &n[2] || p.second != &n[3]) { std::printf("00639250_TEST FAIL\n"); return 1; }
    long k25 = 25; p = t.equal_range(k25);
    if (p.first != &n[2] || p.second != &n[2]) { std::printf("00639250_TEST FAIL\n"); return 1; }
    long k60 = 60; p = t.equal_range(k60);
    if (p.first != &t.Header || p.second != &t.Header) { std::printf("00639250_TEST FAIL\n"); return 1; }
    long k5 = 5; p = t.equal_range(k5);
    if (p.first != &n[0] || p.second != &n[0]) { std::printf("00639250_TEST FAIL\n"); return 1; }
    std::printf("00639250_TEST PASS\n");
    return 0;
}