// Behaviour test for 0x0048D5C0 CopyFlaggedThings (model: list of thing pointers -> vector of the flagged ones).
#include <cstdio>
#include <cstdlib>

struct Thing { int id; bool flag; };
struct Node { Node* next; Node* prev; Thing* data; };
struct Vec {
    Thing** start; Thing** finish; Thing** eos;
    int overflows;
    void push_back(Thing* const& x)
    {
        if (finish != eos) { *finish = x; ++finish; return; }
        ++overflows;
        unsigned int n = (unsigned int)(finish - start), len = n + (n < 1 ? 1 : n);
        Thing** s = (Thing**)std::malloc(len * sizeof(Thing*));
        for (unsigned int i = 0; i < n; ++i) s[i] = start[i];
        s[n] = x;
        std::free(start);
        start = s; finish = s + n + 1; eos = s + len;
    }
};

static Vec* CopyFlagged(Node* first, Node* last, Vec* out)
{
    for (; first != last; first = first->next) {
        Thing* thing = first->data;
        if (thing->flag)
            out->push_back(thing);
    }
    return out;
}

int main()
{
    Thing t[5] = { { 0, true }, { 1, false }, { 2, true }, { 3, true }, { 4, false } };
    Node header; Node nodes[5];
    Node* prev = &header;
    for (int i = 0; i < 5; ++i) { nodes[i].data = &t[i]; nodes[i].prev = prev; prev->next = &nodes[i]; prev = &nodes[i]; }
    prev->next = &header; header.prev = prev;
    Vec v = { 0, 0, 0, 0 };
    Vec* r = CopyFlagged(header.next, &header, &v);
    int ok = r == &v && v.finish - v.start == 3 && v.start[0]->id == 0 && v.start[1]->id == 2 && v.start[2]->id == 3
          && v.overflows == 3;
    std::free(v.start);
    if (!ok) { std::printf("0048d5c0_TEST FAIL\n"); return 1; }
    std::printf("0048d5c0_TEST PASS\n");
    return 0;
}