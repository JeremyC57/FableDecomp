// Behaviour test for 0x00A8EA70: the reconstructed logic over a host model.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
#ifdef _MSC_VER
typedef __int64 i64;
#else
typedef long long i64;
#endif
// Host model: the memory input stream's inlined 4-byte read (range check, chunk fast path, overflow fallback).
struct Stream {
    unsigned int position; const unsigned char* data; int remaining; int overflowCalls;
    void ReadWithSrcChunkOverflow(unsigned char* buf, int size) { ++overflowCalls; for (int i = 0; i < size; ++i) buf[i] = 0x11; position += size; }
    static bool IsValidPosition(i64 p) { return p >= 0 && p <= 0x7fffffff; }
    void Read(void* buf, int size)
    {
        i64 np = (i64)(unsigned int)(position + size);
        if (!IsValidPosition(np)) return;
        if (remaining >= size) { std::memcpy(buf, data, size); data += size; remaining -= size; position += size; }
        else ReadWithSrcChunkOverflow((unsigned char*)buf, size);
    }
};
struct Block { int baseLoads; int MaterialIndex;
    void LoadBinary(Stream* s) { ++baseLoads; int value = 0; s->Read(&value, sizeof(value)); MaterialIndex = value; } };
static void Run()
{
    unsigned char bytes[8] = { 7, 0, 0, 0, 9, 0, 0, 0 };
    Stream s; s.position = 100; s.data = bytes; s.remaining = 8; s.overflowCalls = 0;
    Block b; b.baseLoads = 0; b.MaterialIndex = -1;
    b.LoadBinary(&s);
    CHECK(b.baseLoads == 1 && b.MaterialIndex == 7 && s.position == 104 && s.remaining == 4 && s.data == bytes + 4);
    s.remaining = 2; b.LoadBinary(&s);
    CHECK(s.overflowCalls == 1 && b.MaterialIndex == 0x11111111);
}

int main()
{
    Run();
    if (g_fail) { std::printf("00a8ea70_TEST FAIL\n"); return 1; }
    std::printf("00a8ea70_TEST PASS\n");
    return 0;
}
