// Behaviour test for 0x006C4210 std::remove_copy_if(CIntelligentPointer<CThing>*, .., CIsThingDead).
#include <cstdio>
struct CThing { unsigned char StateBits; bool IsDead() const { return (StateBits & 0x01) != 0; } };
static int g_gets = 0, g_sets = 0;
struct IPtr {
    CThing* item;
    CThing* GetPItem() const { ++g_gets; return item; }
    void SetPItem(CThing* p) { ++g_sets; item = p; }
    IPtr& operator=(const IPtr& o) { SetPItem(o.GetPItem()); return *this; }
};
struct CIsThingDead {
    bool operator()(const IPtr& t) const { return t.GetPItem() == 0 || t.GetPItem()->IsDead(); }
};
static IPtr* RemoveCopyIf(IPtr* first, IPtr* last, IPtr* result, CIsThingDead pred) {
    for (; first != last; ++first)
        if (!pred(*first)) { *result = *first; ++result; }
    return result;
}
int main() {
    int ok = 1;
    CThing alive1 = { 0x04 }, dead = { 0x05 }, alive2 = { 0x08 }, alive3 = { 0x00 };
    IPtr v[6] = { { &alive1 }, { &dead }, { 0 }, { &alive2 }, { &dead }, { &alive3 } };
    // remove_if shape: the first dead at index 1, copy from index 2 onto it
    IPtr* end = RemoveCopyIf(v + 2, v + 6, v + 1, CIsThingDead());
    ok &= end == v + 3;
    ok &= v[0].item == &alive1 && v[1].item == &alive2 && v[2].item == &alive3;
    ok &= g_sets == 2;
    IPtr* same = RemoveCopyIf(v + 3, v + 3, v + 1, CIsThingDead());   // empty range returns result
    ok &= same == v + 1 && g_sets == 2;
    IPtr w[2] = { { &dead }, { 0 } };
    ok &= RemoveCopyIf(w, w + 2, w, CIsThingDead()) == w;            // nothing live
    if (!ok) { std::printf("006c4210_TEST FAIL\n"); return 1; }
    std::printf("006c4210_TEST PASS\n");
    return 0;
}