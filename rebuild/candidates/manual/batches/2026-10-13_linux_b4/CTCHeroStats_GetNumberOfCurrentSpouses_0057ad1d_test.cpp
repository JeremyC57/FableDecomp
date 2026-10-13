#include <cstdio>

struct N { N* Next; N* Prev; unsigned __int64 a, b; long State; bool g; };
struct H { unsigned char pad[0x1d8]; N* Head;
    long Count() { N* h = Head; long c = 0; for (N* n = h->Next; n != h; n = n->Next) { long s = n->State; if (s == 1 || s == 3 || s == 2 || s == 4) ++c; } return c; } };
int main()
{
    N h, n1, n2, n3; h.Next = &n1; n1.Next = &n2; n2.Next = &n3; n3.Next = &h; n1.State = 1; n2.State = 0; n3.State = 4; H s; s.Head = &h;
    if (s.Count() != 2) { std::printf("0057ad1d_TEST FAIL\n"); return 1; }
    h.Next = &h; if (s.Count() != 0) { std::printf("0057ad1d_TEST FAIL\n"); return 1; }
    std::printf("0057ad1d_TEST PASS\n");
    return 0;
}
