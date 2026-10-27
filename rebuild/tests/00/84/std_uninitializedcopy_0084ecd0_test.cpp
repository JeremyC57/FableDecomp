// Behaviour test for 0x0084ECD0 std::__uninitialized_copy over CCountedPointer<NCameraModes::CSubComponentBase>: copy-
// constructs each pointer into raw storage (sharing Data/Info, bumping a non-null Info's count) and returns the end.
#include <cstdio>
#include <new>

struct PointerInfo { unsigned long RefCount; };
struct CountedPointer {
    void* Data;
    PointerInfo* Info;
    CountedPointer(void* d, PointerInfo* i) : Data(d), Info(i) { }
    CountedPointer(const CountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
};

static CountedPointer* UninitializedCopy(const CountedPointer* first, const CountedPointer* last, CountedPointer* result)
{
    CountedPointer* cur = result;
    for (; first != last; ++first, ++cur)
        new (static_cast<void*>(cur)) CountedPointer(*first);
    return cur;
}

int main()
{
    int a = 1, b = 2;
    PointerInfo ia = { 1 }, ib = { 3 };
    CountedPointer src[3] = { CountedPointer(&a, &ia), CountedPointer(0, 0), CountedPointer(&b, &ib) };
    union { double align; unsigned char raw[sizeof(CountedPointer) * 3]; } storage;
    CountedPointer* dst = reinterpret_cast<CountedPointer*>(storage.raw);
    CountedPointer* end = UninitializedCopy(src, src + 3, dst);
    if (end != dst + 3) { std::printf("0084ecd0_TEST FAIL\n"); return 1; }
    if (dst[0].Data != &a || dst[0].Info != &ia || ia.RefCount != 2) { std::printf("0084ecd0_TEST FAIL\n"); return 1; }
    if (dst[1].Data != 0 || dst[1].Info != 0) { std::printf("0084ecd0_TEST FAIL\n"); return 1; }
    if (dst[2].Data != &b || ib.RefCount != 4) { std::printf("0084ecd0_TEST FAIL\n"); return 1; }
    if (UninitializedCopy(src, src, dst) != dst) { std::printf("0084ecd0_TEST FAIL\n"); return 1; }
    std::printf("0084ecd0_TEST PASS\n");
    return 0;
}