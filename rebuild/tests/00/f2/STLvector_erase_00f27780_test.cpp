#include <cstdio>
#include <new>
// Behaviour test for 00f27780: vector<CHeroSoulsCreatureDef>::erase(first, last) assigns the tail down over the hole, destroys the leftover
// elements through their virtual destructor, shrinks finish and returns first.
static int g_dtors = 0;
struct Base { virtual ~Base() {} };
struct Elem : Base { int v; Elem() : v(0) {} Elem& operator=(const Elem& o) { v = o.v; return *this; } virtual ~Elem() { ++g_dtors; } };
struct Vec {
    Elem* start; Elem* finish;
    Elem* erase(Elem* first, Elem* last)
    {
        Elem* r = first;
        for (long n = (long)(finish - last); n > 0; --n) { *r = *last; ++r; ++last; }
        for (Elem* p = r; p != finish; ++p) p->~Elem();
        finish = r;
        return first;
    }
};
int main()
{
    void* raw = operator new(sizeof(Elem) * 6);
    Elem* buf = (Elem*)raw;
    for (int i = 0; i < 6; ++i) { new (buf + i) Elem(); buf[i].v = i; }
    Vec v; v.start = buf; v.finish = buf + 6;
    Elem* r = v.erase(buf + 1, buf + 3);   // remove 1, 2
    int ok = r == buf + 1 && v.finish == buf + 4 && buf[0].v == 0 && buf[1].v == 3 && buf[2].v == 4 && buf[3].v == 5 && g_dtors == 2;
    Elem* r2 = v.erase(buf + 4, buf + 4);  // empty range at the end
    ok = ok && r2 == buf + 4 && v.finish == buf + 4 && g_dtors == 2;
    for (Elem* p = v.start; p != v.finish; ++p) p->~Elem();
    operator delete(raw);
    if (!ok) { std::printf("00f27780_TEST FAIL\n"); return 1; }
    std::printf("00f27780_TEST PASS\n");
    return 0;
}