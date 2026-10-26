#include <cstdio>
#include <cstdlib>
#include <new>

// Behaviour model of CQuestManager::AddPendingQuestToDeactivate (0x004B0A30): push_back a (name, flags) pair onto the
// pending list (new 16-byte-style node linked before the head; the temporary pair destroyed).
static int g_alive = 0;
class CCharString { public:
    const char* p;
    CCharString(const char* s) : p(s) { ++g_alive; }
    CCharString(const CCharString& o) : p(o.p) { ++g_alive; }
    ~CCharString() { --g_alive; }
};
struct Pair { CCharString first; unsigned long second; Pair(const CCharString& a, const unsigned long& b) : first(a), second(b) { } Pair(const Pair& o) : first(o.first), second(o.second) { } };
struct NodeBase { NodeBase* next; NodeBase* prev; };
struct Node : NodeBase { Pair data; };
struct List {
    NodeBase* node;
    void push_back(const Pair& x)
    {
        Node* tmp = (Node*)std::malloc(sizeof(Node));
        new (&tmp->data) Pair(x);
        NodeBase* n = node; NodeBase* p = n->prev;
        tmp->next = n; tmp->prev = p; p->next = tmp; n->prev = tmp;
    }
};
class CQuestManager { public:
    List PendingQuestsToDeactivate;
    void AddPendingQuestToDeactivate(const CCharString& name, unsigned long flags) { PendingQuestsToDeactivate.push_back(Pair(name, flags)); }
};

int main()
{
    NodeBase head; head.next = head.prev = &head;
    CQuestManager qm; qm.PendingQuestsToDeactivate.node = &head;
    CCharString a("Q_Arena"), b("Q_Guild");
    qm.AddPendingQuestToDeactivate(a, 2);
    qm.AddPendingQuestToDeactivate(b, 5);
    Node* first = (Node*)head.next; Node* second = (Node*)first->next;
    if (first->data.first.p != a.p || first->data.second != 2 || second->data.first.p != b.p || second->data.second != 5) { std::printf("004b0a30_TEST FAIL\n"); return 1; }
    if (second->next != &head || head.prev != second || second->prev != first || first->prev != &head) { std::printf("004b0a30_TEST FAIL\n"); return 1; }
    if (g_alive != 4) { std::printf("004b0a30_TEST FAIL\n"); return 1; }                       // a, b + the two node copies (temporaries gone)
    first->data.~Pair(); second->data.~Pair(); std::free(first); std::free(second);
    std::printf("004b0a30_TEST PASS\n");
    return 0;
}