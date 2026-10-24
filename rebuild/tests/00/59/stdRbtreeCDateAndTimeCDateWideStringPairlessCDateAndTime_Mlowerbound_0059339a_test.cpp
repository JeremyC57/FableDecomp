// Behaviour test for 0x0059339A _Rb_tree<CDateAndTime, ...>::_M_lower_bound (model copy)
#include <cstdio>
struct Date { int days; bool operator<(const Date& o) const { return days < o.days; } };
struct Node { bool color; Node* parent; Node* left; Node* right; Date key; };
struct Tree {
    Node* header;
    Node* lower_bound(const Date& k) const
    {
        Node* y = header;
        Node* x = header->parent;
        while (x != 0)
            if (!(x->key < k)) y = x, x = x->left;
            else x = x->right;
        return y;
    }
};
int main()
{
    Node h, n10, n20, n30;
    n20.parent = &h; n20.left = &n10; n20.right = &n30; n20.key.days = 20;
    n10.parent = &n20; n10.left = 0; n10.right = 0; n10.key.days = 10;
    n30.parent = &n20; n30.left = 0; n30.right = 0; n30.key.days = 30;
    h.parent = &n20; h.left = &n10; h.right = &n30;
    Tree t; t.header = &h;
    Date d;
    d.days = 5;  if (t.lower_bound(d) != &n10) { std::printf("0059339a_TEST FAIL\n"); return 1; }
    d.days = 20; if (t.lower_bound(d) != &n20) { std::printf("0059339a_TEST FAIL\n"); return 1; }
    d.days = 21; if (t.lower_bound(d) != &n30) { std::printf("0059339a_TEST FAIL\n"); return 1; }
    d.days = 31; if (t.lower_bound(d) != &h)   { std::printf("0059339a_TEST FAIL\n"); return 1; }
    std::printf("0059339a_TEST PASS\n");
    return 0;
}