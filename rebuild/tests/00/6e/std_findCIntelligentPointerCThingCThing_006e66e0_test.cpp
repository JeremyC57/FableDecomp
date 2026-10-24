// Behaviour test for 0x006E66E0 (STLport __find over intelligent pointers: unrolled by four, then the 0..3 leftovers).
#include <cstdio>

struct Obj { int id; };
struct Ptr {
    Obj* PItem; void* PNext; mutable int calls;
    Obj* GetPItem() const { ++calls; return PItem; }
};
inline bool operator==(const Ptr& p, Obj* o) { return p.GetPItem() == o; }

static Ptr* Find(Ptr* first, Ptr* last, Obj* const& val)
{
    int trip = (int)((last - first) >> 2);
    for (; trip > 0; --trip) {
        if (*first == val) return first; ++first;
        if (*first == val) return first; ++first;
        if (*first == val) return first; ++first;
        if (*first == val) return first; ++first;
    }
    switch (last - first) {
    case 3: if (*first == val) return first; ++first;
    case 2: if (*first == val) return first; ++first;
    case 1: if (*first == val) return first;
    case 0:
    default: return last;
    }
}
#define FAIL { std::printf("006e66e0_TEST FAIL\n"); return 1; }
int main()
{
    Obj objs[11]; Ptr ptrs[11];
    for (int i = 0; i < 11; ++i) { objs[i].id = i; ptrs[i].PItem = &objs[i]; ptrs[i].PNext = 0; ptrs[i].calls = 0; }
    for (int n = 0; n <= 11; ++n)
        for (int k = 0; k < 11; ++k) {
            Ptr* r = Find(ptrs, ptrs + n, &objs[k]);
            if (k < n ? r != ptrs + k : r != ptrs + n) FAIL;
        }
    Obj outside; outside.id = 99;
    for (int i = 0; i < 11; ++i) ptrs[i].calls = 0;
    if (Find(ptrs, ptrs + 11, &outside) != ptrs + 11) FAIL;
    for (int i = 0; i < 11; ++i) if (ptrs[i].calls != 1) FAIL;
    if (Find(ptrs, ptrs, &objs[0]) != ptrs) FAIL;
    std::printf("006e66e0_TEST PASS\n");
    return 0;
}