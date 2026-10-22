// Behaviour test for NUISystem::CMenuEntry::~CMenuEntry (0x0053E4E0): every UI/thing parameter is released to the manager,
// owned UI parameters (Type != 1) are deleted, the parameter lists and the six action lists are freed.
#include <cstdio>

static int g_releasedUI = 0, g_releasedThing = 0, g_deletedParams = 0, g_freedLists = 0, g_removed = 0;

struct Param { int type; ~Param() { ++g_deletedParams; } };
template <class T> struct Node { Node* next; T data; };
template <class T> struct List {
    Node<T>* head;
    List() : head(0) {}
    void Add(T v) { Node<T>* n = new Node<T>; n->data = v; n->next = head; head = n; }
    ~List() { while (head) { Node<T>* n = head; head = head->next; delete n; } ++g_freedLists; }
};

struct Entry {
    List<Param*>* ui;
    List<int>* thing;
    List<int>* actions[6];
    ~Entry()
    {
        ++g_removed;
        if (ui) for (Node<Param*>* n = ui->head; n; n = n->next) ++g_releasedUI;
        if (thing) for (Node<int>* n = thing->head; n; n = n->next) ++g_releasedThing;
        if (ui) {
            for (Node<Param*>* n = ui->head; n; n = n->next)
                if (n->data->type != 1) delete n->data;
            delete ui;
        }
        if (thing) { delete thing; thing = 0; }
        for (int i = 0; i < 6; ++i) delete actions[i];
    }
};

int main()
{
    Param shared; shared.type = 1;
    {
        Entry e;
        e.ui = new List<Param*>;
        Param* owned = new Param; owned->type = 0;
        e.ui->Add(owned); e.ui->Add(&shared);
        e.thing = new List<int>; e.thing->Add(3); e.thing->Add(4); e.thing->Add(5);
        for (int i = 0; i < 6; ++i) e.actions[i] = i % 2 ? new List<int> : 0;
    }
    if (g_removed != 1 || g_releasedUI != 2 || g_releasedThing != 3 || g_deletedParams != 1 || g_freedLists != 5) {
        std::printf("0053e4e0_TEST FAIL\n"); return 1;
    }
    std::printf("0053e4e0_TEST PASS\n");
    return 0;
}