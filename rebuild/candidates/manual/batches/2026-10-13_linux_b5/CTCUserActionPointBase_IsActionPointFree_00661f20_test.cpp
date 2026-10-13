#include <cstdio>
struct N { N* next; N* prev; void* v; };
struct U { unsigned char pad[0x108]; N* head; bool Free() { return head->next != head; } };
int main()
{
    N s, a; s.next = &s; U u; u.head = &s; if (u.Free()) { std::printf("00661f20_TEST FAIL\n"); return 1; }
    s.next = &a; a.next = &s; if (!u.Free()) { std::printf("00661f20_TEST FAIL\n"); return 1; }

    std::printf("00661f20_TEST PASS\n");
    return 0;
}
