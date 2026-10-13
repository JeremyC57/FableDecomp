#include <cstdio>

struct N { N* Next; N* Prev; unsigned __int64 a, b; long State; bool g; };
struct H { unsigned char pad[0x1d8]; N* Head;
    bool Has(long st) { N* h = Head; for (N* n = h->Next; n != h; n = n->Next) if (n->State == st) return true; return false; } };
int main()
{
    N h, n1, n2; h.Next = &n1; n1.Next = &n2; n2.Next = &h; n1.State = 1; n2.State = 4; H s; s.Head = &h;
    if (!s.Has(4) || s.Has(3)) { std::printf("0057ac29_TEST FAIL\n"); return 1; }
    std::printf("0057ac29_TEST PASS\n");
    return 0;
}