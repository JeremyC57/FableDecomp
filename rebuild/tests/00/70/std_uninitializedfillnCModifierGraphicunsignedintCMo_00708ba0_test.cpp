// Behaviour test for 0x00708BA0 std::__uninitialized_fill_n<CModifierGraphic*> (counted def pointer copy).
#include <cstdio>
#include <new>

struct CDefCountedBase { unsigned int RefCount; };
struct CDefPointer {
    CDefCountedBase* Object;
    CDefPointer(CDefCountedBase* o) : Object(o) {}
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
};
struct CModifierGraphic { CDefPointer PTextureReplacement; long BankIndex; float RenderSizeX; };

static CModifierGraphic* UninitializedFillN(CModifierGraphic* first, unsigned int n, const CModifierGraphic& x)
{
    CModifierGraphic* cur = first;
    for ( ; n > 0; --n, ++cur)
        new (cur) CModifierGraphic(x);
    return cur;
}

int main()
{
    CDefCountedBase a = { 1 };
    CModifierGraphic x = { CDefPointer(&a), 42, 0.25f };
    static unsigned char raw[sizeof(CModifierGraphic) * 4];
    CModifierGraphic* dst = (CModifierGraphic*)raw;
    CModifierGraphic* end = UninitializedFillN(dst, 4, x);
    bool ok = end == dst + 4 && a.RefCount == 5;
    for (int i = 0; i < 4; ++i)
        ok = ok && dst[i].PTextureReplacement.Object == &a && dst[i].BankIndex == 42 && dst[i].RenderSizeX == 0.25f;
    ok = ok && UninitializedFillN(dst, 0, x) == dst && a.RefCount == 5;
    if (!ok) { std::printf("00708ba0_TEST FAIL\n"); return 1; }
    std::printf("00708ba0_TEST PASS\n");
    return 0;
}