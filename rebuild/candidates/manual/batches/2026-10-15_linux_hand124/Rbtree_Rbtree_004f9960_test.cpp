#include <cstdio>
#include <stdlib.h>
struct SRbNode { unsigned char Color; SRbNode* Parent; SRbNode* Left; SRbNode* Right; long Key; long Value; };
struct _Rb_tree_base { SRbNode* Header; _Rb_tree_base() : Header(0) { Header = (SRbNode*)malloc(sizeof(SRbNode)); } };
struct _Rb_tree : _Rb_tree_base {
    unsigned long NodeCount;
    _Rb_tree() : NodeCount(0) { Header->Color = 0; Header->Parent = 0; Header->Left = Header->Right = Header; }
    SRbNode* _M_copy(SRbNode* x, SRbNode* p)
    {
        SRbNode* top = (SRbNode*)malloc(sizeof(SRbNode)); *top = *x; top->Parent = p; top->Left = top->Right = 0;
        if (x->Left) top->Left = _M_copy(x->Left, top);
        if (x->Right) top->Right = _M_copy(x->Right, top);
        return top;
    }
    static SRbNode* _S_minimum(SRbNode* x) { while (x->Left != 0) x = x->Left; return x; }
    static SRbNode* _S_maximum(SRbNode* x) { while (x->Right != 0) x = x->Right; return x; }
    SRbNode*& _M_root() const { return Header->Parent; }
    SRbNode*& _M_leftmost() const { return Header->Left; }
    SRbNode*& _M_rightmost() const { return Header->Right; }
    static unsigned char& _S_color(SRbNode* x) { return x->Color; }
    _Rb_tree(const _Rb_tree& other) : _Rb_tree_base(), NodeCount(0)
    {
        if (other._M_root() == 0) { _S_color(Header) = 0; _M_root() = 0; _M_leftmost() = Header; _M_rightmost() = Header; }
        else { _S_color(Header) = 0; _M_root() = _M_copy(other._M_root(), Header); _M_leftmost() = _S_minimum(_M_root()); _M_rightmost() = _S_maximum(_M_root()); }
        NodeCount = other.NodeCount;
    }
};
int main()
{
    _Rb_tree empty;
    _Rb_tree e2(empty);
    if (e2.Header == empty.Header || e2._M_root() != 0 || e2._M_leftmost() != e2.Header || e2._M_rightmost() != e2.Header || e2.NodeCount != 0)
    { std::printf("004f9960_TEST FAIL\n"); return 1; }
    _Rb_tree t;
    SRbNode a, b, c;
    b.Key = 2; b.Parent = t.Header; b.Left = &a; b.Right = &c; b.Color = 1;
    a.Key = 1; a.Parent = &b; a.Left = a.Right = 0; a.Color = 0;
    c.Key = 3; c.Parent = &b; c.Left = c.Right = 0; c.Color = 0;
    t._M_root() = &b; t._M_leftmost() = &a; t._M_rightmost() = &c; t.NodeCount = 3;
    _Rb_tree u(t);
    if (u.NodeCount != 3 || u._M_root() == &b || u._M_root()->Key != 2 || u._M_leftmost()->Key != 1 || u._M_rightmost()->Key != 3 ||
        u._M_root()->Parent != u.Header || u.Header->Color != 0)
    { std::printf("004f9960_TEST FAIL\n"); return 1; }
    std::printf("004f9960_TEST PASS\n");
    return 0;
}
