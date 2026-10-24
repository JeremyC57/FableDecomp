// Behaviour test for 0x009CA4C0 UninitCopy_CountedPointerLong_009ca4c0: copies {intrusive pointer, long} records, add-refs the pointee.
#include <cstdio>
#include <new>

struct __false_type { __false_type() { } };
struct CTextEntry { long RefCount; CTextEntry() : RefCount(1) { } };
struct CIVCountedPointer {
    CTextEntry* P;
    CIVCountedPointer() : P(0) { }
    CIVCountedPointer(const CIVCountedPointer& x) : P(x.P) { if (P) ++P->RefCount; }
};
struct CDataBankTextEntry { CIVCountedPointer PText; long NarratorIndex; };

static CDataBankTextEntry* UninitCopy(const CDataBankTextEntry* first, const CDataBankTextEntry* last, CDataBankTextEntry* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        new (result) CDataBankTextEntry(*first);
    return result;
}

int main()
{
    CTextEntry a, b;
    CDataBankTextEntry src[3];
    src[0].PText.P = &a; src[0].NarratorIndex = 5;
    src[1].PText.P = 0;  src[1].NarratorIndex = -1;
    src[2].PText.P = &a; src[2].NarratorIndex = 9;
    CDataBankTextEntry dst[3];
    __false_type tag;
    CDataBankTextEntry* r = UninitCopy(src, src + 3, dst, tag);
    bool ok = r == dst + 3 && a.RefCount == 3 && b.RefCount == 1;
    if (dst[0].PText.P != &a || dst[0].NarratorIndex != 5 || dst[1].PText.P != 0 || dst[1].NarratorIndex != -1
        || dst[2].PText.P != &a || dst[2].NarratorIndex != 9) ok = false;
    if (UninitCopy(src, src, dst, tag) != dst || a.RefCount != 3) ok = false;
    if (!ok) { std::printf("009ca4c0_TEST FAIL\n"); return 1; }
    std::printf("009ca4c0_TEST PASS\n");
    return 0;
}