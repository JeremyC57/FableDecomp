#include <cstdio>
#include <cstdlib>
// Behaviour test for CVectorMap<unsigned long, long>::DeallocateMemory (0x00A39900).
struct Pair { unsigned long first; long second; };
struct Map {
    Pair* start; Pair* finish; Pair* eos; char compare; bool Dirty;
    void DeallocateMemory()
    {
        finish = start;
        Pair* old = start;
        start = 0; finish = 0; eos = 0;
        Dirty = false;
        if (old) free(old);
    }
};
int main()
{
    Map m; m.start = (Pair*)malloc(3 * sizeof(Pair)); m.finish = m.start + 3; m.eos = m.finish; m.Dirty = true;
    m.DeallocateMemory();
    bool ok = m.start == 0 && m.finish == 0 && m.eos == 0 && !m.Dirty;
    m.Dirty = true; m.DeallocateMemory();
    ok = ok && !m.Dirty && m.start == 0;
    std::printf(ok ? "00a39900_TEST PASS\n" : "00a39900_TEST FAIL\n");
    return ok ? 0 : 1;
}