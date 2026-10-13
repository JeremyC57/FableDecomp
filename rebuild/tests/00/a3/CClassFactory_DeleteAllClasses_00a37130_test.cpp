#include <cstdio>

struct Node { Node* Next; Node* Prev; struct CClass* Value; };
static Node g_head; static int g_deleted = 0;
struct CClass { Node* node; virtual ~CClass() { ++g_deleted; node->Prev->Next = node->Next; node->Next->Prev = node->Prev; } };
struct F { void* vt; unsigned char v[12]; unsigned long a, b; Node* Head;
    void DeleteAllClasses() { while (Head->Next != Head) { CClass* c = Head->Next->Value; if (c) delete c; } } };
int main()
{
    CClass* c1 = new CClass; CClass* c2 = new CClass; Node n1, n2; n1.Value = c1; n2.Value = c2; c1->node = &n1; c2->node = &n2;
    g_head.Next = &n1; n1.Prev = &g_head; n1.Next = &n2; n2.Prev = &n1; n2.Next = &g_head; g_head.Prev = &n2;
    F f; f.Head = &g_head; f.DeleteAllClasses();
    if (g_deleted != 2 || g_head.Next != &g_head) { std::printf("00a37130_TEST FAIL\n"); return 1; }
    std::printf("00a37130_TEST PASS\n");
    return 0;
}