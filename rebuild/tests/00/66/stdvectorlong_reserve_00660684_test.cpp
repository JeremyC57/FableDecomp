// Behaviour test for 0x00660684 vector<long>::reserve (model copy)
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct Vec {
    long* start; long* finish; long* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    unsigned int capacity() const { return (unsigned int)(eos - start); }
    void reserve(unsigned int n)
    {
        if (capacity() < n) {
            const unsigned int old_size = size();
            long* tmp;
            if (start) {
                tmp = n != 0 ? (long*)malloc(n * sizeof(long)) : 0;
                if (finish != start) memmove(tmp, start, (finish - start) * sizeof(long));
                if (start) free(start);
            } else
                tmp = n != 0 ? (long*)malloc(n * sizeof(long)) : 0;
            start = tmp; finish = tmp + old_size; eos = tmp + n;
        }
    }
};
int main()
{
    Vec v; v.start = v.finish = v.eos = 0;
    v.reserve(4);
    if (v.capacity() != 4 || v.size() != 0 || v.start == 0) { std::printf("00660684_TEST FAIL\n"); return 1; }
    v.start[0] = 11; v.start[1] = 22; v.finish = v.start + 2;
    v.reserve(3);                          // no shrink, no change
    if (v.capacity() != 4) { std::printf("00660684_TEST FAIL\n"); return 1; }
    v.reserve(10);
    if (v.capacity() != 10 || v.size() != 2 || v.start[0] != 11 || v.start[1] != 22) { std::printf("00660684_TEST FAIL\n"); return 1; }
    std::printf("00660684_TEST PASS\n");
    return 0;
}