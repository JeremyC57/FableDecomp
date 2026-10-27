// behaviour test for 0x007BAB60: push_back copies the pointed-to item in place while capacity remains, else grows
#include <cstdio>
#include <cstdlib>
struct P { void* item; };
struct V { P* s; P* f; P* e; int grows;
    void grow(const P& x) { unsigned int n = (unsigned int)(f - s), c = n ? 2 * n : 1; P* p = (P*)std::malloc(c * sizeof(P)); for (unsigned int i = 0; i < n; ++i) p[i] = s[i]; p[n].item = x.item; std::free(s); s = p; f = p + n + 1; e = p + c; ++grows; }
    void push_back(const P& x) { if (f != e) { f->item = x.item; ++f; } else grow(x); } };
int main()
{
    int objs[5]; V v; v.s = v.f = v.e = 0; v.grows = 0;
    for (int i = 0; i < 5; ++i) { P x; x.item = &objs[i]; v.push_back(x); }
    int ok = v.f - v.s == 5 && v.grows == 4;
    for (int i = 0; i < 5; ++i) ok = ok && v.s[i].item == &objs[i];
    std::free(v.s);
    if (!ok) { std::printf("007bab60_TEST FAIL\n"); return 1; }
    std::printf("007bab60_TEST PASS\n");
    return 0;
}