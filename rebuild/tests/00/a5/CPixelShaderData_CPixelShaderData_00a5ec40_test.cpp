#include <cstdio>
#include <cstring>
struct Vec { long* s; long* f; long* e; long size() const { return f - s; } };
inline long* CopyTrivial(const long* first, const long* last, long* result) { if (last != first) std::memmove(result, first, (const char*)last - (const char*)first); return result; }
struct PSD {
    void* PixelShader; unsigned long ReferenceCount; unsigned long ConstCount; long* ConstIndices;
    PSD(void* sh, const Vec& v, long) : PixelShader(sh), ReferenceCount(1), ConstCount(v.size()), ConstIndices(0)
    { if (ConstCount) { ConstIndices = new long[ConstCount]; CopyTrivial(v.s, v.f, ConstIndices); } }
};
int main()
{
    long data[3] = { 7, 8, 9 }; Vec v; v.s = data; v.f = data + 3; v.e = data + 3;
    PSD a(&v, v, 0);
    int ok = a.ReferenceCount == 1 && a.ConstCount == 3 && a.ConstIndices && a.ConstIndices[2] == 9 && a.PixelShader == &v;
    Vec e; e.s = e.f = e.e = 0; PSD b(0, e, 0);
    ok = ok && b.ConstCount == 0 && b.ConstIndices == 0;
    delete[] a.ConstIndices;
    if (!ok) { std::printf("00a5ec40_TEST FAIL\n"); return 1; }
    std::printf("00a5ec40_TEST PASS\n");
    return 0;
}