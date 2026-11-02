// Behaviour test for NGraphicAppearance::CTCGraphicAppearance::OnKill (0x004BF8F0): every registered component gets
// OnAppearanceClear() (in list order), then the primitive list is emptied.
#include <cstdio>
static int g_log[16]; static int g_n = 0;
struct Node { Node* next; Node* prev; void* data; };
struct List {                                   // circular sentinel list, as the STLport list
    Node head;
    List() { head.next = head.prev = &head; }
    void push_back(Node* n, void* d) { n->data = d; n->prev = head.prev; n->next = &head; head.prev->next = n; head.prev = n; }
    void clear() { head.next = head.prev = &head; g_log[g_n++] = 99; }
    bool empty() const { return head.next == &head; }
};
class CAGraphicAppearanceComponent { public:
    int Id;
    virtual bool OnAppearanceDraw() { return false; }
    virtual void OnAppearanceClear() { g_log[g_n++] = Id; }
};
class CTCGraphicAppearance { public:
    List Primitives; List Components;
    virtual ~CTCGraphicAppearance() { }
    virtual void OnKill()
    {
        for (Node* it = Components.head.next; it != &Components.head; it = it->next)
            ((CAGraphicAppearanceComponent*)it->data)->OnAppearanceClear();
        Primitives.clear();
    }
};
int main()
{
    CTCGraphicAppearance app;
    CAGraphicAppearanceComponent c1, c2; c1.Id = 1; c2.Id = 2;
    Node n1, n2, p1; int prim = 0;
    app.Components.push_back(&n1, &c1); app.Components.push_back(&n2, &c2); app.Primitives.push_back(&p1, &prim);
    app.OnKill();
    if (g_n != 3 || g_log[0] != 1 || g_log[1] != 2 || g_log[2] != 99 || !app.Primitives.empty() || app.Components.empty()) {
        std::printf("004bf8f0_TEST FAIL\n"); return 1;
    }
    CTCGraphicAppearance none; g_n = 0; none.OnKill();
    if (g_n != 1 || g_log[0] != 99) { std::printf("004bf8f0_TEST FAIL\n"); return 1; }
    std::printf("004bf8f0_TEST PASS\n");
    return 0;
}