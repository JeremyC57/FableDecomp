// Behaviour test for 0x0053CB4F: STLport std::vector<bool>::push_back (room -> store + bump, else grow)
// vector<bool> is a one-byte-per-element STLport vector in retail; the body only moves bytes, so the test drives it through
// distinct byte patterns on an unsigned char view of the same storage.
#include <cstdio>
#include <cstdlib>
#include <cstring>

struct ByteVec {
    unsigned char* start;
    unsigned char* finish;
    unsigned char* eos;
    int overflowCalls;
    void insert_overflow(unsigned char* pos, const unsigned char& x, unsigned int n, bool atEnd)
    {
        ++overflowCalls;
        unsigned int old = (unsigned int)(finish - start);
        unsigned int len = old + (old < n ? n : old);
        unsigned char* s = (unsigned char*)std::malloc(len);
        unsigned int head = (unsigned int)(pos - start);
        if (head) std::memmove(s, start, head);
        unsigned char* f = s + head;
        for (; n > 0; --n, ++f) *f = x;
        if (!atEnd) { unsigned int tail = (unsigned int)(finish - pos); std::memmove(f, pos, tail); f += tail; }
        if (start) std::free(start);
        start = s; finish = f; eos = s + len;
    }
    void push_back(const unsigned char& x)
    {
        if (finish != eos) {
            if (finish) *finish = x;
            ++finish;
        } else
            insert_overflow(finish, x, 1, true);
    }
};

int main()
{
    ByteVec v; v.start = v.finish = v.eos = 0; v.overflowCalls = 0;
    for (int i = 0; i < 10; ++i) { unsigned char c = (unsigned char)(i * 3 + 1); v.push_back(c); }
    if (v.finish - v.start != 10) { std::printf("0053cb4f_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 10; ++i)
        if (v.start[i] != (unsigned char)(i * 3 + 1)) { std::printf("0053cb4f_TEST FAIL\n"); return 1; }
    // growth doubles: capacities 1,2,4,8,16 -> 5 overflow calls
    if (v.overflowCalls != 5 || v.eos - v.start != 16) { std::printf("0053cb4f_TEST FAIL\n"); return 1; }
    std::printf("0053cb4f_TEST PASS\n");
    return 0;
}