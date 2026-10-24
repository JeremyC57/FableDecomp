// Behaviour test for 0x00A371F0 CClassFactory::RegisterAllocatedClass: link into the class list and the type list (back or front), store both positions, bump counts.
#include <cstdio>
#include <cstdlib>

struct CClass;
struct Node { Node* next; Node* prev; CClass* data; };
struct List {
    Node* head;
    void Init() { head = (Node*)std::malloc(sizeof(Node)); head->next = head; head->prev = head; }
    void Insert(Node* n, CClass* x)
    {
        Node* tmp = (Node*)std::malloc(sizeof(Node));
        tmp->data = x;
        Node* p = n->prev;
        tmp->next = n; tmp->prev = p; p->next = tmp; n->prev = tmp;
    }
    void PushBack(CClass* x) { Insert(head, x); }
    void PushFront(CClass* x) { Insert(head->next, x); }
};
struct CClass { int id; Node* ClassListPos; Node* TypeListPos; unsigned char Type; };
struct CClassTypeInfo { List TypeList; unsigned int NoAllocated; };
struct CClassFactory {
    CClassTypeInfo* Info;
    unsigned int NoAllocatedClasses;
    List ClassList;
    void RegisterAllocatedClass(CClass& cls, int type, bool atBack)
    {
        CClassTypeInfo& info = Info[type];
        if (atBack) {
            ClassList.PushBack(&cls);
            info.TypeList.PushBack(&cls);
            cls.ClassListPos = ClassList.head->prev;
            cls.TypeListPos = info.TypeList.head->prev;
        } else {
            ClassList.PushFront(&cls);
            info.TypeList.PushFront(&cls);
            cls.ClassListPos = ClassList.head->next;
            cls.TypeListPos = info.TypeList.head->next;
        }
        ++NoAllocatedClasses;
        ++info.NoAllocated;
    }
};

int main()
{
    CClassTypeInfo infos[2];
    infos[0].TypeList.Init(); infos[0].NoAllocated = 0;
    infos[1].TypeList.Init(); infos[1].NoAllocated = 0;
    CClassFactory f;
    f.Info = infos; f.NoAllocatedClasses = 0; f.ClassList.Init();
    CClass a, b, c;
    a.id = 1; b.id = 2; c.id = 3;
    f.RegisterAllocatedClass(a, 1, true);
    f.RegisterAllocatedClass(b, 1, true);
    f.RegisterAllocatedClass(c, 0, false);
    if (f.NoAllocatedClasses != 3 || infos[1].NoAllocated != 2 || infos[0].NoAllocated != 1) { std::printf("00a371f0_TEST FAIL\n"); return 1; }
    // class list: c, a, b
    Node* n = f.ClassList.head->next;
    if (n->data != &c || n->next->data != &a || n->next->next->data != &b || n->next->next->next != f.ClassList.head) { std::printf("00a371f0_TEST FAIL\n"); return 1; }
    if (a.ClassListPos->data != &a || b.ClassListPos->data != &b || c.ClassListPos != f.ClassList.head->next) { std::printf("00a371f0_TEST FAIL\n"); return 1; }
    if (a.TypeListPos != infos[1].TypeList.head->next || b.TypeListPos != infos[1].TypeList.head->prev || c.TypeListPos != infos[0].TypeList.head->next) { std::printf("00a371f0_TEST FAIL\n"); return 1; }
    std::printf("00a371f0_TEST PASS\n");
    return 0;
}