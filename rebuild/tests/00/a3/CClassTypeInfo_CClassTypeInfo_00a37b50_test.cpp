// Behaviour test for 0x00A37B50 CClassTypeInfo copy constructor: a fresh TypeList with the same pointers, scalars copied, name copied.
#include <cstdio>
#include <cstdlib>
#include <cstring>

class CClass { public: int id; };
struct Node { Node* next; Node* prev; CClass* data; };
struct List {
    Node* head;
    void Init() { head = (Node*)std::malloc(sizeof(Node)); head->next = head; head->prev = head; }
    void InsertRange(Node* pos, Node* first, Node* last)
    {
        for (; first != last; first = first->next) {
            Node* tmp = (Node*)std::malloc(sizeof(Node));
            tmp->data = first->data;
            Node* p = pos->prev;
            tmp->next = pos; tmp->prev = p; p->next = tmp; pos->prev = tmp;
        }
    }
    List() { Init(); }
    List(const List& x)
    {
        head = 0;
        Node* n = (Node*)std::malloc(sizeof(Node));
        n->next = n; n->prev = n;
        head = n;
        InsertRange(head->next, x.head->next, x.head);
    }
};
static int g_copies = 0;
class CCharString { public:
    char text[16];
    CCharString(const char* t) { std::strcpy(text, t); }
    CCharString(const CCharString& o) { std::strcpy(text, o.text); ++g_copies; }
};
typedef CClass* (*AllocFunc)();
static CClass* MakeOne() { return 0; }
class CClassTypeInfo { public:
    List TypeList;
    unsigned int NoAllocated;
    bool Used;
    bool Local;
    AllocFunc Alloc;
    CCharString Name;
    CClassTypeInfo() : Name("CCreature") { }
    CClassTypeInfo(const CClassTypeInfo& o)
        : TypeList(o.TypeList), NoAllocated(o.NoAllocated), Used(o.Used), Local(o.Local), Alloc(o.Alloc), Name(o.Name) { }
};

int main()
{
    CClass c[3];
    CClassTypeInfo a;
    for (int i = 0; i < 3; ++i) {
        Node* tmp = (Node*)std::malloc(sizeof(Node));
        tmp->data = &c[i];
        Node* p = a.TypeList.head->prev;
        tmp->next = a.TypeList.head; tmp->prev = p; p->next = tmp; a.TypeList.head->prev = tmp;
    }
    a.NoAllocated = 3; a.Used = true; a.Local = false; a.Alloc = MakeOne;
    CClassTypeInfo b(a);
    if (b.TypeList.head == a.TypeList.head || b.NoAllocated != 3 || !b.Used || b.Local || b.Alloc != MakeOne ||
        std::strcmp(b.Name.text, "CCreature") != 0 || g_copies != 1) { std::printf("00a37b50_TEST FAIL\n"); return 1; }
    Node* n = b.TypeList.head->next;
    for (int i = 0; i < 3; ++i, n = n->next)
        if (n == b.TypeList.head || n->data != &c[i] || n == a.TypeList.head->next) { std::printf("00a37b50_TEST FAIL\n"); return 1; }
    if (n != b.TypeList.head) { std::printf("00a37b50_TEST FAIL\n"); return 1; }
    std::printf("00a37b50_TEST PASS\n");
    return 0;
}