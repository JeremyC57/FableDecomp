// Behaviour test for 0x00881780: InsertDisplacementEntry: frames = (long)(time * fps), entry appended at the back of its priority stack, its address registered as key and returned
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %%d\n", __LINE__); } } while (0)
struct Colour { float r, g, b, a; };
struct Entry { long unknown; long frames; bool finished; Colour colour; };
struct Node { Node* next; Node* prev; Entry val; };
struct List {
    Node head;
    List() { head.next = &head; head.prev = &head; }
    void push_back(const Entry& e) { Node* where = &head; Node* n = new Node; n->val = e; Node* p = where->prev; n->prev = p; n->next = where; p->next = n; where->prev = n; }
    Entry& back() { return head.prev->val; }
};
static const long g_fps = 30;
struct Stack {
    List stacks[3]; void* keys[8]; int nkeys;
    Stack() : nkeys(0) {}
    void* Insert(float time, const Colour& c, int prio)
    {
        Entry e; e.unknown = 0; e.frames = (long)(time * g_fps); e.finished = false; e.colour = c;
        List& l = stacks[prio]; l.push_back(e);
        Entry* key = &l.back(); keys[nkeys++] = key; return key;
    }
};
static void Run()
{
    Stack s; Colour c = { 0.5f, 0.25f, 1.0f, 1.0f };
    Entry* a = (Entry*)s.Insert(2.0f, c, 1);
    Entry* b = (Entry*)s.Insert(0.5f, c, 1);
    CHECK(a->frames == 60 && b->frames == 15 && a->colour.g == 0.25f && !a->finished && a->unknown == 0);
    CHECK(s.stacks[1].head.next->val.frames == 60 && s.stacks[1].head.prev->val.frames == 15);
    CHECK(s.stacks[0].head.next == &s.stacks[0].head && s.nkeys == 2 && s.keys[0] == a && s.keys[1] == b);
    delete s.stacks[1].head.next->next; delete s.stacks[1].head.next;
}

int main()
{
    Run();
    if (g_fail) { std::printf("00881780_TEST FAIL\n"); return 1; }
    std::printf("00881780_TEST PASS\n");
    return 0;
}