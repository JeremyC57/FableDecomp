#include <cstdio>

struct Base { void* vt; long RefCount; };
struct P { Base* Object; P(Base* o) : Object(o) { if (o) o->RefCount++; } };
struct C { unsigned char pad[0x38]; Base** First; Base** Last; unsigned char p2[4]; long Index;
    P Get() { long n = Last - First; return P(First[Index % n]); } };
int main()
{
    Base a, b; a.RefCount = b.RefCount = 0; Base* list[2] = { &a, &b }; C c; c.First = list; c.Last = list + 2; c.Index = 3;
    P p = c.Get(); if (p.Object != &b || b.RefCount != 1) { std::printf("00733480_TEST FAIL\n"); return 1; }
    std::printf("00733480_TEST PASS\n");
    return 0;
}
