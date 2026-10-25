// behaviour test for 0x00BC5E90 std::vector<CEnginePrimitiveHandle>::reserve(size_type n)
#include <cstdio>
#include <cstdlib>
#include <new>

struct Counted { int RefCount; };

struct Handle {
    Counted* P;
    Handle(Counted* p) : P(p) { if (P) ++P->RefCount; }
    Handle(const Handle& o) : P(o.P) { if (P) ++P->RefCount; }
    ~Handle() { if (P) --P->RefCount; }
};

struct HandleVector {
    Handle* Start;
    Handle* Finish;
    Handle* End;
    HandleVector() : Start(0), Finish(0), End(0) { }
    unsigned int size() const { return (unsigned int)(Finish - Start); }
    unsigned int capacity() const { return (unsigned int)(End - Start); }
    Handle* allocate(unsigned int n) { return n != 0 ? (Handle*)std::malloc(n * sizeof(Handle)) : 0; }
    void clear()
    {
        for (Handle* p = Start; p != Finish; ++p)
            p->~Handle();
        if (Start) std::free(Start);
    }
    void reserve(unsigned int n)
    {
        if (capacity() < n) {
            unsigned int oldSize = size();
            Handle* tmp;
            if (Start) {
                tmp = allocate(n);
                Handle* cur = tmp;
                for (Handle* p = Start; p != Finish; ++p, ++cur)
                    new (cur) Handle(*p);
                clear();
            } else
                tmp = allocate(n);
            Start = tmp;
            Finish = tmp + oldSize;
            End = tmp + n;
        }
    }
};

int main()
{
    Counted a; a.RefCount = 0;
    Counted b; b.RefCount = 0;
    HandleVector v;
    v.reserve(2);
    if (v.capacity() != 2 || v.size() != 0) { std::printf("00bc5e90_TEST FAIL\n"); return 1; }
    new (v.Finish) Handle(&a); ++v.Finish;
    new (v.Finish) Handle(&b); ++v.Finish;
    if (a.RefCount != 1 || b.RefCount != 1) { std::printf("00bc5e90_TEST FAIL\n"); return 1; }
    v.reserve(1);
    if (v.capacity() != 2) { std::printf("00bc5e90_TEST FAIL\n"); return 1; }
    v.reserve(8);
    if (v.capacity() != 8 || v.size() != 2 || v.Start[0].P != &a || v.Start[1].P != &b) { std::printf("00bc5e90_TEST FAIL\n"); return 1; }
    if (a.RefCount != 1 || b.RefCount != 1) { std::printf("00bc5e90_TEST FAIL\n"); return 1; }
    v.clear();
    if (a.RefCount != 0 || b.RefCount != 0) { std::printf("00bc5e90_TEST FAIL\n"); return 1; }
    std::printf("00bc5e90_TEST PASS\n");
    return 0;
}