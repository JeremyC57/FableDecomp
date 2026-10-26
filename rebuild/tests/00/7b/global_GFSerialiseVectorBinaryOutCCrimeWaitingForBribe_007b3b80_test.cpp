// Behaviour test for 0x007B3B80 GFSerialiseVectorBinaryOut<CCrimeWaitingForBribe>: count then (handle, type) pairs.
#include <cstdio>
struct Stream { int vals[16]; int n; void WriteLong(long v) { vals[n++] = (int)v; } };
class CCrimeWaitingForBribe { public: int DeedHandle; int Type; };
struct Vec { CCrimeWaitingForBribe* s; CCrimeWaitingForBribe* f; unsigned int size() const { return (unsigned int)(f - s); } CCrimeWaitingForBribe& operator[](unsigned int i) { return *(s + i); } };
static void Out(Stream& stream, Vec& value)
{
    long count = value.size();
    stream.WriteLong(count);
    for (long i = 0; i < count; ++i) { const CCrimeWaitingForBribe* v = &value[i]; stream.WriteLong(v->DeedHandle); stream.WriteLong(v->Type); }
}
int main()
{
    CCrimeWaitingForBribe r[2] = { { 11, 3 }, { 22, 7 } };
    Vec v; v.s = r; v.f = r + 2;
    Stream s; s.n = 0;
    Out(s, v);
    if (s.n != 5 || s.vals[0] != 2 || s.vals[1] != 11 || s.vals[2] != 3 || s.vals[3] != 22 || s.vals[4] != 7) { std::printf("007b3b80_TEST FAIL\n"); return 1; }
    v.f = r; s.n = 0; Out(s, v);
    if (s.n != 1 || s.vals[0] != 0) { std::printf("007b3b80_TEST FAIL\n"); return 1; }
    std::printf("007b3b80_TEST PASS\n");
    return 0;
}