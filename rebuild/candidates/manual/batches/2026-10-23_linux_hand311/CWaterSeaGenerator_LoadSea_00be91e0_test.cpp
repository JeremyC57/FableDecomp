// Behaviour test for CWaterSeaGenerator::LoadSea (0x00BE91E0): counts read through the out-pointers, each buffer created and
// decompressed only when its count is non-zero, compressed blocks skipped in the stream.
#include <cstdio>
#include <cstring>

struct Stream {
    const unsigned char* data; unsigned long pos;
    long ReadLong() { long v = 0; std::memcpy(&v, data + pos, 4); pos += 4; return v; }
    const unsigned char* GetDataAndSkip(long n) { const unsigned char* p = data + pos; pos += n; return p; }
};
struct Buffer { int inits, unlocks; long count, stride, extra; unsigned char mem[64];
    void Uninitialise() { count = -1; } };
static long g_decompressCalls = 0, g_lastCount = 0, g_lastStride = 0;
static void Decompress(const unsigned char* src, long count, long stride, unsigned char* dst) { ++g_decompressCalls; g_lastCount = count; g_lastStride = stride; dst[0] = src[0]; }

static void LoadSea(Stream* stream, Buffer* vb, long* vertexCount, Buffer* ib, long* polyCount)
{
    vb->Uninitialise(); ib->Uninitialise();
    *vertexCount = stream->ReadLong();
    *polyCount = stream->ReadLong();
    if (*vertexCount) {
        ++vb->inits; vb->count = *vertexCount; vb->stride = 0xc;
        long size = stream->ReadLong();
        if (size > 0) Decompress(stream->GetDataAndSkip(size), *vertexCount, 0xc, vb->mem);
        ++vb->unlocks;
    }
    if (*polyCount) {
        ++ib->inits; ib->count = *polyCount * 2; ib->extra = 0x65;
        long size = stream->ReadLong();
        if (size > 0) Decompress(stream->GetDataAndSkip(size), *polyCount, 2, ib->mem);
        ++ib->unlocks;
    }
}

int main()
{
    unsigned char bytes[64]; std::memset(bytes, 0, sizeof(bytes));
    int words[] = { 3, 4, 2 }; std::memcpy(bytes, words, 12); bytes[12] = 0x5a; bytes[13] = 0x5b;
    int w2 = 1; std::memcpy(bytes + 14, &w2, 4); bytes[18] = 0x77;
    Stream s = { bytes, 0 };
    Buffer vb, ib; std::memset(&vb, 0, sizeof(vb)); std::memset(&ib, 0, sizeof(ib));
    long vc = 0, pc = 0;
    LoadSea(&s, &vb, &vc, &ib, &pc);
    int ok = vc == 3 && pc == 4 && vb.inits == 1 && vb.unlocks == 1 && vb.mem[0] == 0x5a && ib.count == 8 && ib.mem[0] == 0x77;
    ok = ok && g_decompressCalls == 2 && g_lastCount == 4 && g_lastStride == 2 && s.pos == 19;
    int zero[2] = { 0, 0 }; Stream z = { (const unsigned char*)zero, 0 };
    Buffer a, b; std::memset(&a, 0, sizeof(a)); std::memset(&b, 0, sizeof(b));
    LoadSea(&z, &a, &vc, &b, &pc);
    ok = ok && vc == 0 && pc == 0 && a.inits == 0 && b.inits == 0 && a.count == -1 && g_decompressCalls == 2;
    if (!ok) { std::printf("00be91e0_TEST FAIL\n"); return 1; }
    std::printf("00be91e0_TEST PASS\n");
    return 0;
}
