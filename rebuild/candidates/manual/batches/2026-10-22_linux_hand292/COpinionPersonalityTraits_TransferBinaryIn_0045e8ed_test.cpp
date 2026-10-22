// Behaviour test for COpinionPersonalityTraits::TransferBinaryIn (0x0045E8ED): nine 4-byte reads from a chunked
// memory stream, the in-chunk path copying directly and the short-chunk path going through the overflow reader.
#include <cstdio>
#include <cstring>
struct Stream {
    unsigned int position; int size; const unsigned char* data; int chunkPos; int remaining;
    int overflowCalls; const unsigned char* overflowData;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, int n)
    {
        overflowCalls++;
        memcpy(buffer, overflowData, n);
        overflowData += n;
        position += n;
    }
    void Read(void* buffer, int n)
    {
        long long np = (long long)(position + n);
        if (np < 0 || np > 0x7fffffff) return;
        if (remaining >= n) { memcpy(buffer, data, n); data += n; remaining -= n; position += n; }
        else ReadWithSrcChunkOverflow((unsigned char*)buffer, n);
    }
};
template <class T> void TransferIn(Stream& s, T& out) { T v; s.Read(&v, sizeof(v)); out = v; }
struct Traits {
    float f[9];
    void TransferBinaryIn(Stream& s) { for (int i = 0; i < 9; i++) TransferIn(s, f[i]); }
};
int main()
{
    int fails = 0;
    float src[9]; float over[9];
    for (int i = 0; i < 9; i++) { src[i] = 1.5f * (float)i; over[i] = -2.0f * (float)i; }
    Stream s; s.position = 0; s.size = 64; s.data = (const unsigned char*)src; s.chunkPos = 0; s.remaining = 20;
    s.overflowCalls = 0; s.overflowData = (const unsigned char*)(over + 5);
    Traits t; memset(&t, 0, sizeof(t));
    t.TransferBinaryIn(s);
    for (int i = 0; i < 5; i++) if (t.f[i] != src[i]) fails++;
    for (int i = 5; i < 9; i++) if (t.f[i] != over[i]) fails++;
    if (s.overflowCalls != 4) fails++;
    if (s.position != 36) fails++;
    Stream bad = s; bad.position = 0x7ffffffe; bad.remaining = 100; bad.overflowCalls = 0;
    Traits u; memset(&u, 0, sizeof(u));
    u.TransferBinaryIn(bad);
    if (bad.remaining != 100 || bad.overflowCalls != 0 || bad.position != 0x7ffffffe) fails++;   // out of range: nothing consumed
    if (fails == 0) printf("0045e8ed_TEST PASS\n"); else printf("0045e8ed_TEST FAIL %d\n", fails);
    return fails;
}
