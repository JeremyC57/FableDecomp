#include <cstdio>

struct QT { unsigned char pad[5]; bool Activated; };
struct N { N* Next; N* Prev; QT* Data; void* Info; long Index; };
struct M { unsigned char pad[0x24]; N* Head;
    bool Is(long i) { N* h = Head; for (N* n = h->Next; n != h; n = n->Next) if (n->Index == i) return n->Data->Activated; return false; } };
int main()
{
    QT q1, q2; q1.Activated = false; q2.Activated = true; N h, a, b; h.Next = &a; a.Next = &b; b.Next = &h; a.Index = 1; a.Data = &q1; b.Index = 2; b.Data = &q2; M m; m.Head = &h;
    if (m.Is(1) || !m.Is(2) || m.Is(3)) { std::printf("00a14eb0_TEST FAIL\n"); return 1; }
    std::printf("00a14eb0_TEST PASS\n");
    return 0;
}