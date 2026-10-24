// Behaviour test for 0x00738720 remove_copy over CDefPointer<const CMeleeCombatAbilityDef>.
#include <stdio.h>
struct CDefPointeeBase {
    virtual void v00() {}
    virtual void Release() { ++Released; }
    long RefCount;
    int Released;
    CDefPointeeBase() : RefCount(0), Released(0) {}
    void RemoveRef()
    {
        if (--RefCount == 0)
            Release();
    }
};
struct CMeleeCombatAbilityDef;
template <class T> struct CDefPointer {
    CDefPointeeBase* Object;
    CDefPointer& operator=(const CDefPointer& o)
    {
        if (Object != o.Object) {
            if (Object) {
                Object->RemoveRef();
                Object = 0;
            }
            Object = o.Object;
            if (Object)
                ++Object->RefCount;
        }
        return *this;
    }
    bool operator==(const CDefPointer& o) const { return Object == o.Object; }
};
typedef CDefPointer<const CMeleeCombatAbilityDef> TAbilityPtr;

TAbilityPtr* remove_copy(TAbilityPtr* first, TAbilityPtr* last, TAbilityPtr* result, const TAbilityPtr& val)
{
    for (; first != last; ++first)
        if (!(*first == val)) {
            *result = *first;
            ++result;
        }
    return result;
}

int main()
{
    CDefPointeeBase a, b, c;
    TAbilityPtr v[5];
    v[0].Object = &a; v[1].Object = &b; v[2].Object = &c; v[3].Object = &b; v[4].Object = &a;
    a.RefCount = 2; b.RefCount = 2; c.RefCount = 1;
    TAbilityPtr val; val.Object = &b;
    int fail = 0;
    // in-place removal of b: a, c, a remain
    TAbilityPtr* end = remove_copy(v, v + 5, v, val);
    if (end != v + 3) fail |= 1;
    if (v[0].Object != &a || v[1].Object != &c || v[2].Object != &a) fail |= 2;
    // v[1] was b (count 2 -> 1), now c (count 1 -> 2); v[2] was c (2 -> 1) now a (2 -> 3) ...
    if (b.RefCount != 1 || b.Released != 0) fail |= 4;
    if (c.RefCount != 1) fail |= 8;
    if (a.RefCount != 3) fail |= 16;
    // empty range returns result unchanged
    if (remove_copy(v, v, v + 2, val) != v + 2) fail |= 32;
    // a release reaching zero calls Release once
    CDefPointeeBase d; d.RefCount = 1;
    TAbilityPtr dst[1]; dst[0].Object = &d;
    TAbilityPtr src[1]; src[0].Object = &a;
    remove_copy(src, src + 1, dst, val);
    if (d.Released != 1 || d.RefCount != 0 || dst[0].Object != &a || a.RefCount != 4) fail |= 64;
    if (fail) { printf("00738720_TEST FAIL %d\n", fail); return 1; }
    printf("00738720_TEST PASS\n");
    return 0;
}