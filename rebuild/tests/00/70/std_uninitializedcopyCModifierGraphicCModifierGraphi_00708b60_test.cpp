// Behaviour test for 0x00708B60 std::__uninitialized_copy<CModifierGraphic*> (counted def pointer copy).
#include <cstdio>
#include <new>

struct CDefCountedBase { unsigned int RefCount; };
struct CDefPointer {
    CDefCountedBase* Object;
    CDefPointer(CDefCountedBase* o) : Object(o) {}
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
};
struct CModifierGraphic { CDefPointer PTextureReplacement; long BankIndex; float RenderSizeX; };

static CModifierGraphic* UninitializedCopy(CModifierGraphic* first, CModifierGraphic* last, CModifierGraphic* result)
{
    CModifierGraphic* cur = result;
    for ( ; first != last; ++first, ++cur)
        new (cur) CModifierGraphic(*first);
    return cur;
}

int main()
{
    CDefCountedBase a = { 1 }, b = { 5 };
    CModifierGraphic src[3] = { { CDefPointer(&a), 10, 1.5f }, { CDefPointer(0), 20, 2.5f }, { CDefPointer(&b), 30, 3.5f } };
    static unsigned char raw[sizeof(CModifierGraphic) * 3];
    CModifierGraphic* dst = (CModifierGraphic*)raw;
    CModifierGraphic* end = UninitializedCopy(src, src + 3, dst);
    bool ok = end == dst + 3 && a.RefCount == 2 && b.RefCount == 6
           && dst[0].PTextureReplacement.Object == &a && dst[1].PTextureReplacement.Object == 0 && dst[2].PTextureReplacement.Object == &b
           && dst[0].BankIndex == 10 && dst[1].BankIndex == 20 && dst[2].BankIndex == 30 && dst[2].RenderSizeX == 3.5f;
    ok = ok && UninitializedCopy(src, src, dst) == dst && a.RefCount == 2;
    if (!ok) { std::printf("00708b60_TEST FAIL\n"); return 1; }
    std::printf("00708b60_TEST PASS\n");
    return 0;
}