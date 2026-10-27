// behaviour test for 0x004AA270 CPersistTraits<ESaveType>::TransferBinaryIn (model: a chunk read and the overflow path)
#include <cstdio>
#include <cstring>
static int g_overflow = 0;
struct Stream {
    unsigned int pos; const unsigned char* data; int remaining;
    void Overflow(unsigned char* b, int n) { ++g_overflow; std::memset(b, 0, n); }
    void Read(void* buf, int size)
    {
        long long np = (long long)(pos + size);
        if (!(np >= 0 && np <= 0x7fffffff)) return;
        if (remaining >= size) { std::memcpy(buf, data, size); data += size; remaining -= size; pos += size; }
        else Overflow((unsigned char*)buf, size);
    }
};
int main()
{
    unsigned char bytes[4] = { 0x78, 0x56, 0x34, 0x12 };
    Stream s = { 0, bytes, 4 };
    unsigned int v = 0, n;
    s.Read(&n, 4); v = n;
    int ok = v == 0x12345678u && s.pos == 4 && s.remaining == 0;
    s.Read(&n, 4);
    ok = ok && g_overflow == 1;
    if (!ok) { std::printf("004aa270_TEST FAIL\n"); return 1; }
    std::printf("004aa270_TEST PASS\n");
    return 0;
}