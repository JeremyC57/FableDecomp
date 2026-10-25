// Behaviour test for 0x00841FC0: vector<string>::erase(first, last) (assign tail down, free the dead strings, return first)
#include <cstdio>
#include <cstdlib>
#include <cstring>

static int g_frees = 0;

struct Str {
    char* start;
    char* finish;
    char* end_of_storage;
    void set(const char* s)
    {
        unsigned int n = (unsigned int)std::strlen(s);
        start = (char*)std::malloc(n + 1);
        std::memcpy(start, s, n + 1);
        finish = start + n; end_of_storage = finish + 1;
    }
    Str& operator=(const Str& o)
    {
        if (&o != this) {
            unsigned int n = (unsigned int)(o.finish - o.start);
            std::free(start);
            start = (char*)std::malloc(n + 1);
            std::memcpy(start, o.start, n + 1);
            finish = start + n; end_of_storage = finish + 1;
        }
        return *this;
    }
    void destroy() { if (start != 0) { std::free(start); ++g_frees; } }
};

struct Vec {
    Str* start;
    Str* finish;
    Str* end_of_storage;
    Str* erase(Str* first, Str* last)
    {
        Str* r = first;
        for (int n = (int)(finish - last); n > 0; --n) { *r = *last; ++last; ++r; }
        for (Str* p = r; p != finish; ++p) p->destroy();
        finish = r;
        return first;
    }
};

int main()
{
    Str data[6];
    const char* words[6] = { "zero", "one", "two", "three", "four", "five" };
    for (int i = 0; i < 6; ++i) data[i].set(words[i]);
    Vec v; v.start = data; v.finish = data + 6; v.end_of_storage = data + 6;
    if (v.erase(data + 1, data + 3) != data + 1) { std::printf("00841fc0_TEST FAIL\n"); return 1; }
    const char* want[4] = { "zero", "three", "four", "five" };
    if (v.finish != data + 4 || g_frees != 2) { std::printf("00841fc0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 4; ++i)
        if (std::strcmp(data[i].start, want[i]) != 0) { std::printf("00841fc0_TEST FAIL\n"); return 1; }
    if (v.erase(v.start, v.finish) != data || v.finish != data || g_frees != 6) { std::printf("00841fc0_TEST FAIL\n"); return 1; }
    if (v.erase(v.start, v.start) != data || v.finish != data) { std::printf("00841fc0_TEST FAIL\n"); return 1; }
    std::printf("00841fc0_TEST PASS\n");
    return 0;
}