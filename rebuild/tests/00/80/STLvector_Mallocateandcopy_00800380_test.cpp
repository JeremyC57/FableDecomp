// Behaviour test for 0x00800380 vector<pair<C2DVector, unsigned char> >::_M_allocate_and_copy: n-element block, [first, last) copied in.
#include <cstdio>
#include <cstdlib>
#include <new>
class C2DVector { public: float X, Y; C2DVector(float x, float y) : X(x), Y(y) { } C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { } };
#pragma pack(push, 1)
struct SPoint { C2DVector first; unsigned char second; SPoint(const C2DVector& a, unsigned char b) : first(a), second(b) { } SPoint(const SPoint& o) : first(o.first), second(o.second) { } };
#pragma pack(pop)
static SPoint* AllocateAndCopy(unsigned long n, const SPoint* first, const SPoint* last)
{
    SPoint* result = n != 0 ? (SPoint*)std::malloc(n * sizeof(SPoint)) : 0;
    SPoint* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) SPoint(*first);
    return result;
}
int main()
{
    bool ok = sizeof(SPoint) == 9;
    SPoint src[3] = { SPoint(C2DVector(1.0f, 2.0f), 7), SPoint(C2DVector(3.0f, 4.0f), 0), SPoint(C2DVector(-5.0f, 6.5f), 255) };
    SPoint* block = AllocateAndCopy(5, src, src + 3);
    ok = ok && block != 0;
    for (int i = 0; ok && i < 3; ++i)
        ok = block[i].first.X == src[i].first.X && block[i].first.Y == src[i].first.Y && block[i].second == src[i].second;
    std::free(block);
    ok = ok && AllocateAndCopy(0, src, src) == 0;            // n == 0: no allocation, nothing copied
    SPoint* empty = AllocateAndCopy(2, src, src);            // empty range still allocates n
    ok = ok && empty != 0;
    std::free(empty);
    if (!ok) { std::printf("00800380_TEST FAIL\n"); return 1; }
    std::printf("00800380_TEST PASS\n");
    return 0;
}