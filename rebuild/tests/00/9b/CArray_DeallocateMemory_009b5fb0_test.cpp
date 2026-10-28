// Test: CArray<pair<unsigned long, unsigned long> >::DeallocateMemory -- non-empty capacity: clear, swap out and free; empty: untouched
#include <cstdio>
#include <cstdlib>
struct Pair { unsigned int first, second; };
struct CArray {
    Pair* start; Pair* finish; Pair* end_of_storage; int frees;
    unsigned int capacity() const { return (unsigned int)(end_of_storage - start); }
    void DeallocateMemory()
    {
        if (capacity() > 0) {
            finish = start;
            Pair* old = start;
            start = finish = end_of_storage = 0;
            if (old) { std::free(old); ++frees; }
        }
    }
};
int main()
{
    CArray a; a.start = (Pair*)std::malloc(4 * sizeof(Pair)); a.finish = a.start + 2; a.end_of_storage = a.start + 4; a.frees = 0;
    a.DeallocateMemory();
    CArray e; e.start = e.finish = e.end_of_storage = 0; e.frees = 0;
    e.DeallocateMemory();
    if (a.start || a.finish || a.end_of_storage || a.frees != 1 || e.frees != 0) { std::printf("009b5fb0_TEST FAIL\n"); return 1; }
    std::printf("009b5fb0_TEST PASS\n");
    return 0;
}