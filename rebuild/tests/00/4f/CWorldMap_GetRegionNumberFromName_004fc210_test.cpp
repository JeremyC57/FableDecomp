#include <cstdio>
#include <cstring>
struct Data { const char* text; unsigned long length; };
static bool Eq(const Data* a, const Data* b) { if (a == b) return true; if (!a || !b) return false; if (a->length != b->length) return false; return std::strcmp(a->text, b->text) == 0; }
static long Find(Data* const* names, unsigned long n, const Data* name) { for (unsigned long i = 1; i < n; ++i) if (Eq(names[i], name)) return (long)i; return 0; }
static bool Run()
{
    Data a = { "Oakvale", 7 }, b = { "Oakvale", 7 }, c = { "Bower", 5 };
    Data* names[4] = { &a, 0, &c, &b };
    return Find(names, 4, &a) == 3 && Find(names, 4, 0) == 1 && Find(names, 4, &c) == 2 && Find(names, 2, &c) == 0;
}
int main()
{
    if (!Run()) { std::printf("004fc210_TEST FAIL\n"); return 1; }
    std::printf("004fc210_TEST PASS\n");
    return 0;
}