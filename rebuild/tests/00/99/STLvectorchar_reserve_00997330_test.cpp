// Behaviour test for 0x00997330 std::vector<char>::reserve: a smaller or equal request changes nothing; a larger one
// moves the bytes to a new block of exactly n, keeping the size; an empty (null) vector just allocates.
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct VecChar {
    char* s; char* f; char* e;
    void reserve(unsigned int n)
    {
        if ((unsigned int)(e - s) < n) {
            unsigned int old = (unsigned int)(f - s);
            char* tmp;
            if (s) {
                tmp = n ? (char*)malloc(n) : 0;
                if (f != s) memmove(tmp, s, f - s);
                if (s) free(s);
            }
            else
                tmp = n ? (char*)malloc(n) : 0;
            s = tmp; f = tmp + old; e = tmp + n;
        }
    }
};
int main()
{
    int fails = 0;
    VecChar v; v.s = v.f = v.e = 0;
    v.reserve(0);
    if (v.s || v.f || v.e) ++fails;
    v.reserve(8);
    if (!v.s || v.f != v.s || v.e != v.s + 8) ++fails;
    std::memcpy(v.s, "abcde", 5); v.f = v.s + 5;
    char* before = v.s;
    v.reserve(4);
    if (v.s != before || v.e != v.s + 8) ++fails;
    v.reserve(32);
    if (v.f != v.s + 5 || v.e != v.s + 32 || std::memcmp(v.s, "abcde", 5) != 0) ++fails;
    free(v.s);
    std::printf(fails ? "00997330_TEST FAIL\n" : "00997330_TEST PASS\n");
    return fails ? 1 : 0;
}