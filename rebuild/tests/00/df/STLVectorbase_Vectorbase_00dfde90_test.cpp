#include <cstdio>
#include <cstdlib>
// Behaviour test for 00dfde90: the STLport _Vector_base(n, alloc) ctor allocates n elements (none for n == 0) and sets
// start == finish == the block, end == start + n.
struct Elem { unsigned char body[12]; };
struct Alloc { Elem* allocate(unsigned int n) { return n != 0 ? (Elem*)std::malloc(n * sizeof(Elem)) : 0; } };
struct VectorBase {
    Elem* start; Elem* finish; Elem* end; Alloc a;
    VectorBase(unsigned int n, const Alloc&) : start(0), finish(0), end(0) { start = a.allocate(n); finish = start; end = start + n; }
};
int main()
{
    Alloc al;
    VectorBase v(5, al);
    VectorBase z(0, al);
    int ok = v.start != 0 && v.finish == v.start && v.end == v.start + 5 && (char*)v.end - (char*)v.start == 60
          && z.start == 0 && z.finish == 0 && z.end == 0;
    std::free(v.start);
    if (!ok) { std::printf("00dfde90_TEST FAIL\n"); return 1; }
    std::printf("00dfde90_TEST PASS\n");
    return 0;
}