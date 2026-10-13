#include <cstdio>

struct R { void* vt; void* p; void* list; R* Prev; R* Next;
    void ListRemove() { if (Next) { if (Prev->Next == this && Next->Prev == this) { Prev->Next = Next; Next->Prev = Prev; } Prev = 0; Next = 0; } } };
int main()
{
    R a, b, c; a.Next = &b; b.Prev = &a; b.Next = &c; c.Prev = &b; c.Next = &a; a.Prev = &c;
    b.ListRemove(); if (a.Next != &c || c.Prev != &a || b.Next || b.Prev) { std::printf("009fbfb0_TEST FAIL\n"); return 1; }
    b.ListRemove(); if (a.Next != &c) { std::printf("009fbfb0_TEST FAIL\n"); return 1; }
    std::printf("009fbfb0_TEST PASS\n");
    return 0;
}