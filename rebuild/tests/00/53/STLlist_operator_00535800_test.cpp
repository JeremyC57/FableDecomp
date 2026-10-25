#include <cstdio>
#include <cstdlib>

struct CComponent { int Id; };
typedef CComponent* TElem;

// Host model of STLport list<T>::operator=: overwrite the common prefix element by element, then erase this list's
// surplus [first1, last1) or insert the rest of x [first2, last2) before end(); self-assignment is a no-op.
struct Node { Node* Next; Node* Prev; TElem Data; };
struct List {
    Node* Head;                                       // the sentinel
    List() { Head = (Node*)std::malloc(sizeof(Node)); Head->Next = Head->Prev = Head; }
    ~List() { erase(Head->Next, Head); std::free(Head); }
    void insert(Node* pos, const TElem& x)
    {
        Node* n = (Node*)std::malloc(sizeof(Node));
        n->Data = x;
        Node* prev = pos->Prev;
        n->Next = pos; n->Prev = prev; prev->Next = n; pos->Prev = n;
    }
    void push_back(const TElem& x) { insert(Head, x); }
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

static bool same_(const List& a, const List& b)
{
    const Node* p = a.Head->Next; const Node* q = b.Head->Next;
    for (; p != a.Head && q != b.Head; p = p->Next, q = q->Next)
        if (p->Data != q->Data) return false;
    return p == a.Head && q == b.Head;
}
int main()
{
    CComponent c[6] = { { 0 }, { 1 }, { 2 }, { 3 }, { 4 }, { 5 } };
    List parents, other;
    parents.push_back(&c[0]);
    for (int i = 1; i < 6; ++i) other.push_back(&c[i]);
    parents = other;
    if (parents.size() != 5 || !same_(parents, other) || parents.Head->Next->Data != &c[1]) { std::printf("00535800_TEST FAIL\n"); return 1; }
    List one; one.push_back(&c[3]);
    parents = one;
    if (parents.size() != 1 || parents.Head->Next->Data != &c[3]) { std::printf("00535800_TEST FAIL\n"); return 1; }
    parents = parents;
    if (parents.size() != 1) { std::printf("00535800_TEST FAIL\n"); return 1; }
    std::printf("00535800_TEST PASS\n");
    return 0;
}