#include <cstdio>

struct N { N* Next; N* Prev; void* a; void* b; long Index; };
struct M { unsigned char pad[0x24]; N* Head;
    bool Is(long i) { N* h = Head; for (N* n = h->Next; n != h; n = n->Next) if (n->Index == i) return true; return false; } };
int main()
{
    N h, n1, n2; h.Next = &n1; n1.Next = &n2; n2.Next = &h; n1.Index = 4; n2.Index = 9; M m; m.Head = &h;
    if (!m.Is(9) || m.Is(5)) { std::printf("00a14e80_TEST FAIL\n"); return 1; }
    h.Next = &h; if (m.Is(4)) { std::printf("00a14e80_TEST FAIL\n"); return 1; }
    std::printf("00a14e80_TEST PASS\n");
    return 0;
}