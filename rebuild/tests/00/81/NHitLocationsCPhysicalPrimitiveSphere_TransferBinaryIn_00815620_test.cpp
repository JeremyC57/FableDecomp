#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
typedef __int64 i64;
#else
typedef long long i64;
#endif
struct Stream {
    unsigned int position; const unsigned char* data; int remaining; int overflow;
    void ReadOverflow(unsigned char* buf, int size) { ++overflow; std::memset(buf, 0, size); }
    static bool Valid(i64 p) { return p >= 0 && p <= 0x7fffffff; }
    void Read(void* buf, int size)
    {
        i64 np = (i64)(position + size);
        if (!Valid(np)) return;
        if (remaining >= size) { std::memcpy(buf, data, size); data += size; remaining -= size; position += size; }
        else ReadOverflow((unsigned char*)buf, size);
    }
};
struct Sphere { int nameReads; float Radius;
    void TransferBinaryIn(Stream* s) { ++nameReads; float r; s->Read(&r, sizeof(r)); Radius = r; } };
int main()
{
    float v = 2.5f; unsigned char buf[4]; std::memcpy(buf, &v, 4);
    Stream s; s.position = 8; s.data = buf; s.remaining = 4; s.overflow = 0;
    Sphere sp; sp.nameReads = 0; sp.Radius = 0.0f;
    sp.TransferBinaryIn(&s);
    int ok = sp.Radius == 2.5f && sp.nameReads == 1 && s.position == 12 && s.remaining == 0;
    sp.TransferBinaryIn(&s);
    ok = ok && s.overflow == 1 && sp.Radius == 0.0f;
    if (!ok) { std::printf("00815620_TEST FAIL\n"); return 1; }
    std::printf("00815620_TEST PASS\n");
    return 0;
}