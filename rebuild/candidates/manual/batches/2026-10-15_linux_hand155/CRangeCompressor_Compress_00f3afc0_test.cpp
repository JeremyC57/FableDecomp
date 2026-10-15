// Behaviour test for 0x00F3AFC0: Compress sizes the output to w*h+1, compresses, shrinks to the produced size; refuses huge bounds.
#include <cstdio>
struct Arr { unsigned int size; int resizes; void resize(unsigned int n, const unsigned char&) { size = n; ++resizes; } };
struct RC { int calls; long last; long CompressToBuffer(const void*, long w, long h, unsigned char*, long destSize) { ++calls; last = destSize; return w + h; }
  long Compress(const void* src, long w, long h, Arr& out) {
    long long bound = (long long)w * h + 1; if (bound < 0 || bound > 0x7fffffff) return 0;
    out.resize(w * h + 1, 0); unsigned char buf[1]; long p = CompressToBuffer(src, w, h, buf, w * h + 1); out.resize(p, 0); return p; } };
int main()
{
    RC rc = {0, 0}; Arr a = {0, 0};
    long r = rc.Compress(0, 4, 5, a);
    bool ok = r == 9 && rc.last == 21 && a.size == 9 && a.resizes == 2;
    Arr b = {0, 0}; ok = ok && rc.Compress(0, 0x10000, 0x10000, b) == 0 && b.resizes == 0 && rc.calls == 1;
    if (!ok) { std::printf("00f3afc0_TEST FAIL\n"); return 1; }
    std::printf("00f3afc0_TEST PASS\n");
    return 0;
}
