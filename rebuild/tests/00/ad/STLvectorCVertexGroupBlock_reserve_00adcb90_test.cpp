// Behaviour test for 0x00ADCB90 vector<CVertexGroupBlock>::reserve (copy into new storage, destroy + free the old run).
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_copies = 0, g_destroys = 0, g_frees = 0;
struct SBlock { int Id; unsigned long* Words; };
struct SVector {
    SBlock* Start; SBlock* Finish; SBlock* End;
    unsigned int size() const { return (unsigned int)(Finish - Start); }
    unsigned int capacity() const { return (unsigned int)(End - Start); }
    static SBlock* Allocate(unsigned int n) { return n ? (SBlock*)std::malloc(n * sizeof(SBlock)) : 0; }
    // reconstructed body
    void reserve(unsigned int n)
    {
        if (capacity() < n) {
            const unsigned int oldSize = size();
            SBlock* tmp;
            if (Start) {
                tmp = Allocate(n);
                SBlock* cur = tmp;                                            // __uninitialized_copy 0x00ADC330
                for (SBlock* p = Start; p != Finish; ++p, ++cur) { new (cur) SBlock(*p); ++g_copies; }
                for (SBlock* p = Start; p != Finish; ++p) ++g_destroys;      // __destroy_aux 0x00ADC190
                if (Start) { std::free(Start); ++g_frees; }
            } else
                tmp = Allocate(n);
            Start = tmp; Finish = tmp + oldSize; End = tmp + n;
        }
    }
};

int main()
{
    SVector v; v.Start = v.Finish = v.End = 0;
    v.reserve(0);                                                              // capacity 0 < 0 is false: nothing
    if (v.Start != 0 || g_copies != 0) { std::printf("00adcb90_TEST FAIL\n"); return 1; }
    v.reserve(2);                                                              // empty vector: plain allocate
    if (v.capacity() != 2 || v.size() != 0 || g_copies != 0 || g_frees != 0) { std::printf("00adcb90_TEST FAIL\n"); return 1; }
    v.Start[0].Id = 10; v.Start[1].Id = 11; v.Finish = v.Start + 2;
    v.reserve(2);                                                              // already big enough
    if (g_copies != 0) { std::printf("00adcb90_TEST FAIL\n"); return 1; }
    SBlock* old = v.Start;
    v.reserve(5);
    if (v.capacity() != 5 || v.size() != 2 || g_copies != 2 || g_destroys != 2 || g_frees != 1 || v.Start[1].Id != 11 || v.Start == old) { std::printf("00adcb90_TEST FAIL\n"); return 1; }
    std::free(v.Start);
    std::printf("00adcb90_TEST PASS\n");
    return 0;
}