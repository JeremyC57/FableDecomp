#include <cstdio>
struct Obj { int x; };
struct IP { Obj* item; void* next; Obj* GetPItem() const { return item; } };
inline bool operator==(const IP& p, Obj* o) { return p.GetPItem() == o; }
IP* Find(IP* first, IP* last, Obj* const& value)
{
    long count;
    for (count = (last - first) >> 2; 0 < count; --count) {
        if (*first == value) return first; ++first;
        if (*first == value) return first; ++first;
        if (*first == value) return first; ++first;
        if (*first == value) return first; ++first;
    }
    switch (last - first) {
    case 3: if (*first == value) return first; ++first;
    case 2: if (*first == value) return first; ++first;
    case 1: if (*first == value) return first;
    default: return last;
    }
}
int main()
{
    Obj o[7];
    IP a[7];
    for (int i = 0; i < 7; ++i) { a[i].item = &o[i]; a[i].next = 0; }
    Obj other;
    Obj* p;
    bool ok = true;
    for (int i = 0; i < 7; ++i) { p = &o[i]; if (Find(a, a + 7, p) != a + i) ok = false; }
    p = &other; if (Find(a, a + 7, p) != a + 7) ok = false;
    p = &o[6]; if (Find(a, a + 6, p) != a + 6) ok = false;
    if (!ok) { std::printf("006e67a0_TEST FAIL\n"); return 1; }
    std::printf("006e67a0_TEST PASS\n");
    return 0;
}