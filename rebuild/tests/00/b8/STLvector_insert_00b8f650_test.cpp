// behaviour test for 0x00B8F650 std::vector<pair<unsigned long, unsigned long> >::insert(position, x)
#include <cstdio>
#include <cstdlib>
#include <new>

struct Pair { unsigned int first; unsigned int second; };

struct PairVector {
    Pair* Start;
    Pair* Finish;
    Pair* End;
    int Overflows;
    void InsertOverflow(Pair* pos, const Pair& x)
    {
        ++Overflows;
        unsigned int size = (unsigned int)(Finish - Start);
        unsigned int cap = size ? size * 2 : 1;
        Pair* mem = (Pair*)std::malloc(cap * sizeof(Pair));
        Pair* d = mem;
        for (Pair* p = Start; p != pos; ++p, ++d) *d = *p;
        *d++ = x;
        for (Pair* p = pos; p != Finish; ++p, ++d) *d = *p;
        if (Start) std::free(Start);
        Start = mem; Finish = d; End = mem + cap;
    }
    Pair* insert(Pair* pos, const Pair& x)
    {
        unsigned int n = (unsigned int)(pos - Start);
        if (Finish != End) {
            if (pos == Finish) {
                new (Finish) Pair(x);
                ++Finish;
            } else {
                new (Finish) Pair(*(Finish - 1));
                ++Finish;
                Pair copy = x;
                Pair* last = Finish - 2;
                Pair* result = Finish - 1;
                for (int k = (int)(last - pos); k > 0; --k) *--result = *--last;
                *pos = copy;
            }
        } else
            InsertOverflow(pos, x);
        return Start + n;
    }
};

static Pair P(unsigned int a, unsigned int b) { Pair p; p.first = a; p.second = b; return p; }

int main()
{
    PairVector v; v.Start = 0; v.Finish = 0; v.End = 0; v.Overflows = 0;
    Pair* it = v.insert(v.Finish, P(10, 1));
    if (it != v.Start || v.Overflows != 1) { std::printf("00b8f650_TEST FAIL\n"); return 1; }
    it = v.insert(v.Finish, P(30, 3));
    it = v.insert(v.Start + 1, P(20, 2));
    if (it != v.Start + 1) { std::printf("00b8f650_TEST FAIL\n"); return 1; }
    it = v.insert(v.Start, P(5, 0));
    if (it != v.Start || v.Finish - v.Start != 4 || v.Overflows != 3) { std::printf("00b8f650_TEST FAIL\n"); return 1; }
    unsigned int want[4] = { 5, 10, 20, 30 };
    for (int i = 0; i < 4; ++i)
        if (v.Start[i].first != want[i] || v.Start[i].second != (unsigned int)i) { std::printf("00b8f650_TEST FAIL\n"); return 1; }
    std::free(v.Start);
    std::printf("00b8f650_TEST PASS\n");
    return 0;
}