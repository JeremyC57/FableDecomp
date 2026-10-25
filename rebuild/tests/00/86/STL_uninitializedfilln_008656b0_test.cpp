// Behaviour test for 0x008656B0: uninitialized_fill_n of {list, frame} records (fresh sentinel + copied entries each)
#include <cstdio>
#include <cstdlib>
#include <new>

struct Node { Node* next; Node* prev; float v[7]; };

struct List {
    Node* node;
    List() : node((Node*)std::malloc(sizeof(Node))) { node->next = node; node->prev = node; }
    List(const List& x) : node(0)
    {
        node = (Node*)std::malloc(sizeof(Node));
        node->next = node; node->prev = node;
        for (Node* p = x.node->next; p != x.node; p = p->next) push_back(p->v[0]);
    }
    void push_back(float f)
    {
        Node* n = (Node*)std::malloc(sizeof(Node));
        for (int i = 0; i < 7; ++i) n->v[i] = f + (float)i;
        n->next = node; n->prev = node->prev; node->prev->next = n; node->prev = n;
    }
    int size() const { int c = 0; for (Node* p = node->next; p != node; p = p->next) ++c; return c; }
};

struct Flash { List geometry; long frame; };

static Flash* FillN(Flash* first, unsigned int n, const Flash& x)
{
    Flash* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) Flash(x);
    return cur;
}

int main()
{
    Flash src;
    src.geometry.push_back(1.0f); src.geometry.push_back(2.0f); src.geometry.push_back(3.0f);
    src.frame = 42;
    Flash* raw = (Flash*)std::malloc(4 * sizeof(Flash));
    if (FillN(raw, 0, src) != raw) { std::printf("008656b0_TEST FAIL\n"); return 1; }
    if (FillN(raw, 4, src) != raw + 4) { std::printf("008656b0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 4; ++i) {
        if (raw[i].frame != 42 || raw[i].geometry.size() != 3 || raw[i].geometry.node == src.geometry.node) { std::printf("008656b0_TEST FAIL\n"); return 1; }
        if (raw[i].geometry.node->next->v[0] != 1.0f || raw[i].geometry.node->prev->v[6] != 9.0f) { std::printf("008656b0_TEST FAIL\n"); return 1; }
    }
    if (raw[0].geometry.node == raw[1].geometry.node) { std::printf("008656b0_TEST FAIL\n"); return 1; }
    std::printf("008656b0_TEST PASS\n");
    return 0;
}