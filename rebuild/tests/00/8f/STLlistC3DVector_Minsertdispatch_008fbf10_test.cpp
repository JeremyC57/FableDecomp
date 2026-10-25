// Behaviour test for 0x008FBF10 std::list<C3DVector>::_M_insert_dispatch: copies a range before a position, in order.
#include <cstdio>
#include <cstdlib>
#include <new>

struct C3DVector { float X, Y, Z; };

struct Node { Node* next; Node* prev; C3DVector data; };

struct List {
    Node* head;
    List() { head = (Node*)std::malloc(sizeof(Node)); head->next = head; head->prev = head; }
    Node* insert(Node* n, const C3DVector& x)
    {
        Node* tmp = (Node*)std::malloc(sizeof(Node));
        if (&tmp->data) new (&tmp->data) C3DVector(x);
        Node* p = n->prev;
        tmp->next = n; tmp->prev = p; p->next = tmp; n->prev = tmp;
        return tmp;
    }
    // the function under test
    void _M_insert_dispatch(Node* pos, const Node* first, const Node* last)
    {
        for ( ; first != last; first = first->next)
            insert(pos, first->data);
    }
    int count() const { int c = 0; for (Node* n = head->next; n != head; n = n->next) ++c; return c; }
};

static bool near(float a, float b) { float d = a - b; return d < 0.001f && d > -0.001f; }

int main()
{
    List src, dst;
    C3DVector a = { 1.0f, 2.0f, 3.0f }, b = { 4.0f, 5.0f, 6.0f }, c = { 7.0f, 8.0f, 9.0f }, z = { -1.0f, -2.0f, -3.0f };
    src.insert(src.head, a); src.insert(src.head, b); src.insert(src.head, c);
    dst.insert(dst.head, z);
    int fail = 0;
    dst._M_insert_dispatch(dst.head->next, src.head->next, src.head);    // before z
    if (dst.count() != 4 || src.count() != 3) fail = 1;
    Node* n = dst.head->next;
    if (!near(n->data.X, 1.0f) || !near(n->next->data.Y, 5.0f) || !near(n->next->next->data.Z, 9.0f)
        || !near(n->next->next->next->data.X, -1.0f)) fail = 1;
    if (dst.head->prev->next != dst.head || n->prev != dst.head) fail = 1;
    dst._M_insert_dispatch(dst.head, src.head, src.head);                // empty range: no change
    if (dst.count() != 4) fail = 1;
    if (fail) { std::printf("008fbf10_TEST FAIL\n"); return 1; }
    std::printf("008fbf10_TEST PASS\n");
    return 0;
}