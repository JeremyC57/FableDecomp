// Behaviour test for 0x00AD4620 std::_Rb_tree::_M_insert (map<pair<long,long>, CCountedPointer<CVertexPairContraction> >)
#include <cstdio>
#include <cstdlib>
#include <new>

struct Info { unsigned int RefCount; };
struct Value { long A, B; void* Data; Info* Info_; };
struct Node { bool Color; Node* Parent; Node* Left; Node* Right; Value V; };
static int g_rebalances = 0;
static void Rebalance(Node*, Node*&) { ++g_rebalances; }              // 0x0042971D (not under test)
static bool KeyLess(const Value& a, const Value& b) { return a.A < b.A || (!(b.A < a.A) && a.B < b.B); }
struct Tree {
    Node* Header; unsigned int Count;
    Tree() : Count(0) { Header = (Node*)std::malloc(sizeof(Node)); Header->Parent = 0; Header->Left = Header; Header->Right = Header; }
    Node* Insert(Node* x, Node* y, const Value& v, Node* w)            // the row's body
    {
        Node* z;
        if (y == Header || (w == 0 && (x != 0 || KeyLess(v, y->V)))) {
            z = (Node*)std::malloc(sizeof(Node));
            new (&z->V) Value(v);
            if (z->V.Info_) ++z->V.Info_->RefCount;
            y->Left = z;
            if (y == Header) { Header->Parent = z; Header->Right = z; }
            else if (y == Header->Left) Header->Left = z;
        }
        else {
            z = (Node*)std::malloc(sizeof(Node));
            new (&z->V) Value(v);
            if (z->V.Info_) ++z->V.Info_->RefCount;
            y->Right = z;
            if (y == Header->Right) Header->Right = z;
        }
        z->Parent = y; z->Left = 0; z->Right = 0;
        Rebalance(z, Header->Parent);
        ++Count;
        return z;
    }
};

int main()
{
    int ok = 1;
    Tree t;
    Info info = { 1 };
    Value a = { 5, 5, 0, &info }, b = { 5, 9, 0, 0 }, c = { 2, 100, 0, 0 };
    Node* ra = t.Insert(0, t.Header, a, 0);                            // empty tree: root, leftmost, rightmost
    ok &= t.Header->Parent == ra && t.Header->Left == ra && t.Header->Right == ra && info.RefCount == 2;
    Node* rb = t.Insert(0, ra, b, 0);                                  // (5,9) is not less than (5,5): right child
    ok &= ra->Right == rb && t.Header->Right == rb && t.Header->Left == ra;
    Node* rc = t.Insert(0, ra, c, 0);                                  // (2,100) < (5,5): left child, new leftmost
    ok &= ra->Left == rc && t.Header->Left == rc && rc->Parent == ra;
    Node* rd = t.Insert(0, rb, c, rb);                                 // a hint w forces the right side
    ok &= rb->Right == rd && t.Header->Right == rd;
    ok &= t.Count == 4 && g_rebalances == 4;
    if (ok) std::printf("00ad4620_TEST PASS\n"); else std::printf("00ad4620_TEST FAIL\n");
    return ok ? 0 : 1;
}