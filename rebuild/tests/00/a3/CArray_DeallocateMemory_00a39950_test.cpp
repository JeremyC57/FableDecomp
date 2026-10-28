#include <cstdio>
#include <cstdlib>
// Behaviour test for CArray<pair<unsigned long, long> >::DeallocateMemory (0x00A39950).
struct Pair { unsigned long first; long second; };
struct Arr {
    Pair* start; Pair* finish; Pair* eos;
    unsigned int capacity() const { return (unsigned int)(eos - start); }
    void DeallocateMemory()
    {
        if (capacity() > 0) {
            finish = start;                      // clear()
            Pair* old = start;
            start = 0; finish = 0; eos = 0;      // swap with an empty array
            if (old) free(old);                  // the empty array's destructor frees the old block
        }
    }
};
int main()
{
    Arr a; a.start = (Pair*)malloc(4 * sizeof(Pair)); a.finish = a.start + 2; a.eos = a.start + 4;
    a.DeallocateMemory();
    bool ok = a.start == 0 && a.finish == 0 && a.eos == 0;
    Arr e; e.start = e.finish = e.eos = 0; e.DeallocateMemory();
    ok = ok && e.start == 0 && e.capacity() == 0;
    std::printf(ok ? "00a39950_TEST PASS\n" : "00a39950_TEST FAIL\n");
    return ok ? 0 : 1;
}