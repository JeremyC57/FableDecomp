// behaviour test for 0x00711A50: growing a vector of counted pointers add-refs every copied pointer once
#include <cstdio>
#include <vector>
struct Obj { long count; };
struct Ptr { Obj* p; Ptr(Obj* o) : p(o) { if (p) ++p->count; } Ptr(const Ptr& o) : p(o.p) { if (p) ++p->count; }
    Ptr& operator=(const Ptr& o) { if (o.p) ++o.p->count; if (p) --p->count; p = o.p; return *this; } ~Ptr() { if (p) --p->count; } };
int main()
{
    Obj a = { 0 }, b = { 0 };
    {
        std::vector<Ptr> v;
        for (int i = 0; i < 9; ++i) v.push_back(Ptr(i & 1 ? &a : &b));
        v.insert(v.begin() + 3, 4, Ptr(0));
        if (v.size() != 13 || a.count != 4 || b.count != 5 || v[3].p != 0 || v[7].p != &a) { std::printf("00711a50_TEST FAIL\n"); return 1; }
    }
    if (a.count != 0 || b.count != 0) { std::printf("00711a50_TEST FAIL\n"); return 1; }
    std::printf("00711a50_TEST PASS\n");
    return 0;
}