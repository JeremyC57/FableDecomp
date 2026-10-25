// Behaviour test for 0x006860F0: STLport vector<bool>::_M_fill_assign (rebuild, overwrite+grow, overwrite+erase)
#include <cstdio>
#include <cstdlib>
#include <cstring>

struct BVec {
    bool* start; bool* finish; bool* eos;
    unsigned int capacity() const { return (unsigned int)(eos - start); }
    unsigned int size() const { return (unsigned int)(finish - start); }
    void fill_assign(unsigned int n, const bool& val)
    {
        if (n > capacity()) {
            bool* s = n ? (bool*)std::malloc(n) : 0;
            for (unsigned int i = 0; i < n; ++i) s[i] = val;
            if (start) std::free(start);
            start = s; finish = s + n; eos = s + n;
        } else if (n > size()) {
            for (bool* p = start; p != finish; ++p) *p = val;
            unsigned int k = n - size();
            bool* f = finish;
            for (; k > 0; --k, ++f) *f = val;
            finish = f;
        } else {
            bool* f = start;
            for (unsigned int k = n; k > 0; --k, ++f) *f = val;
            if (finish != f) std::memmove(f, finish, 0);
            finish = f;
        }
    }
};

int main()
{
    BVec v; v.start = v.finish = v.eos = 0;
    v.fill_assign(0x5c, true);
    if (v.size() != 0x5c || v.capacity() != 0x5c) { std::printf("006860f0_TEST FAIL\n"); return 1; }
    for (unsigned int i = 0; i < v.size(); ++i) if (!v.start[i]) { std::printf("006860f0_TEST FAIL\n"); return 1; }
    v.fill_assign(10, false);
    if (v.size() != 10 || v.capacity() != 0x5c || v.start[0] || v.start[9]) { std::printf("006860f0_TEST FAIL\n"); return 1; }
    v.fill_assign(40, true);
    if (v.size() != 40 || v.capacity() != 0x5c) { std::printf("006860f0_TEST FAIL\n"); return 1; }
    for (unsigned int i = 0; i < 40; ++i) if (!v.start[i]) { std::printf("006860f0_TEST FAIL\n"); return 1; }
    v.fill_assign(100, false);
    if (v.size() != 100 || v.capacity() != 100 || v.start[99]) { std::printf("006860f0_TEST FAIL\n"); return 1; }
    std::printf("006860f0_TEST PASS\n");
    return 0;
}