#include <cstdio>

struct Task { bool put; bool Taken; virtual void v00(){} virtual void v04(){} virtual void v08(){} virtual void v0c(){} virtual bool IsPutDownTask() { return put; } };
struct N { N* Next; N* Prev; Task* Data; void* Info; };
struct V { unsigned char pad[0x60]; N* Head;
    bool Is() { N* h = Head; for (N* n = h->Next; n != h; n = n->Next) { Task* t = n->Data; if (!t->Taken && t->IsPutDownTask()) return true; } return false; } };
int main()
{
    Task t1, t2; t1.put = true; t1.Taken = true; t2.put = false; t2.Taken = false; N h, n1, n2; h.Next = &n1; n1.Next = &n2; n2.Next = &h; n1.Data = &t1; n2.Data = &t2; V v; v.Head = &h;
    if (v.Is()) { std::printf("00658e09_TEST FAIL\n"); return 1; }
    t2.put = true; if (!v.Is()) { std::printf("00658e09_TEST FAIL\n"); return 1; }
    std::printf("00658e09_TEST PASS\n");
    return 0;
}