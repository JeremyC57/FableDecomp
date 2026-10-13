#include <cstdio>
struct N { N* next; N* prev; void* v; };
struct D { unsigned char pad[0x40]; N* head; long Count() { long n = 0; for (N* p = head->next; p != head; p = p->next) ++n; return n; } };
int main()
{
    N s, a, b; s.next = &a; a.next = &b; b.next = &s; D d; d.head = &s; if (d.Count() != 2) { std::printf("00647308_TEST FAIL\n"); return 1; }

    std::printf("00647308_TEST PASS\n");
    return 0;
}