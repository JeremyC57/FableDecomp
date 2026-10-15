// Behaviour test for 0x006C2710 (CLevelLoader::ProcessHoverAnimQueue): one front object processed and popped per call, progress text around it.
#include <cstdio>
#include <cstring>
static const char* g_text[8]; static int g_nText;
static void DisplayProgress(const char* t, bool, float, bool) { g_text[g_nText++] = t; }
struct Loader;
struct Obj { int loads; Loader* by; void Load(Loader* l) { ++loads; by = l; } };
struct Node { Node* next; Node* prev; Obj* value; };
struct List {
    Node* head;
    bool empty() const { return head->next == head; }
    Obj* front() { return head->next->value; }
    void pop_front() { Node* n = head->next; head->next = n->next; n->next->prev = head; delete n; }
    void push_back(Obj* o) { Node* n = new Node; n->value = o; n->next = head; n->prev = head->prev; head->prev->next = n; head->prev = n; }
};
struct Loader {
    List ObjectList;
    void Process()
    {
        if (!ObjectList.empty()) {
            DisplayProgress("Level loader update", false, -1.0f, false);
            ObjectList.front()->Load(this);
            DisplayProgress("Level loader update end", false, -1.0f, false);
            ObjectList.pop_front();
        }
    }
};
int main()
{
    Loader l; Node sentinel; sentinel.next = sentinel.prev = &sentinel; l.ObjectList.head = &sentinel;
    l.Process();
    int ok = g_nText == 0;
    Obj a = { 0, 0 }, b = { 0, 0 };
    l.ObjectList.push_back(&a); l.ObjectList.push_back(&b);
    l.Process();
    ok &= a.loads == 1 && a.by == &l && b.loads == 0 && g_nText == 2 && std::strcmp(g_text[1], "Level loader update end") == 0;
    l.Process(); l.Process();
    ok &= b.loads == 1 && l.ObjectList.empty() && g_nText == 4;
    if (!ok) { std::printf("006c2710_TEST FAIL\n"); return 1; }
    std::printf("006c2710_TEST PASS\n");
    return 0;
}