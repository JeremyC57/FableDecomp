#include <cstdio>
#include <cstdlib>

struct C2DVector { float X; float Y; };
struct C2DLineF { C2DVector Point1; C2DVector Point2; };

// Host model of STLport list<T>::operator=: overwrite the common prefix element by element, then erase this list's
// surplus [first1, last1) or insert the rest of x [first2, last2) before end(); self-assignment is a no-op.
struct Node { Node* Next; Node* Prev; C2DLineF Data; };
struct List {
    Node* Head;                                       // the sentinel
    List() { Head = (Node*)std::malloc(sizeof(Node)); Head->Next = Head->Prev = Head; }
    ~List() { erase(Head->Next, Head); std::free(Head); }
    void insert(Node* pos, const C2DLineF& x)
    {
        Node* n = (Node*)std::malloc(sizeof(Node));
        n->Data = x;
        Node* prev = pos->Prev;
        n->Next = pos; n->Prev = prev; prev->Next = n; pos->Prev = n;
    }
    void push_back(const C2DLineF& x) { insert(Head, x); }
    Node* erase(Node* first, Node* last)
    {
        while (first != last) {
            Node* next = first->Next;
            Node* prev = first->Prev;
            prev->Next = next; next->Prev = prev;
            std::free(first);
            first = next;
        }
        return last;
    }
    List& operator=(const List& x)
    {
        if (this != &x) {
            Node* first1 = Head->Next;
            Node* last1 = Head;
            const Node* first2 = x.Head->Next;
            const Node* last2 = x.Head;
            while (first1 != last1 && first2 != last2) {
                first1->Data = first2->Data;
                first1 = first1->Next;
                first2 = first2->Next;
            }
            if (first2 == last2)
                erase(first1, last1);
            else
                for (; first2 != last2; first2 = first2->Next)
                    insert(last1, first2->Data);
        }
        return *this;
    }
    int size() const { int n = 0; for (const Node* p = Head->Next; p != Head; p = p->Next) ++n; return n; }
private:
    List(const List&);
};

static C2DLineF line_(float a) { C2DLineF l; l.Point1.X = a; l.Point1.Y = a + 1; l.Point2.X = a + 2; l.Point2.Y = a + 3; return l; }
static bool same_(const List& a, const List& b)
{
    const Node* p = a.Head->Next; const Node* q = b.Head->Next;
    for (; p != a.Head && q != b.Head; p = p->Next, q = q->Next) {
        float d = p->Data.Point1.X - q->Data.Point1.X;
        float e = p->Data.Point2.Y - q->Data.Point2.Y;
        if (d < -0.001f || d > 0.001f || e < -0.001f || e > 0.001f) return false;
    }
    return p == a.Head && q == b.Head;
}
int main()
{
    List small_, big;
    small_.push_back(line_(1)); small_.push_back(line_(2));
    for (int i = 0; i < 5; ++i) big.push_back(line_(10.0f + i));
    List a; a.push_back(line_(99));
    a = big;                                          // grow: one overwritten, four inserted
    if (a.size() != 5 || !same_(a, big)) { std::printf("00517720_TEST FAIL\n"); return 1; }
    a = small_;                                       // shrink: two overwritten, three erased
    if (a.size() != 2 || !same_(a, small_)) { std::printf("00517720_TEST FAIL\n"); return 1; }
    a = a;                                            // self-assignment
    if (a.size() != 2 || !same_(a, small_)) { std::printf("00517720_TEST FAIL\n"); return 1; }
    List empty_;
    a = empty_;
    if (a.size() != 0) { std::printf("00517720_TEST FAIL\n"); return 1; }
    std::printf("00517720_TEST PASS\n");
    return 0;
}