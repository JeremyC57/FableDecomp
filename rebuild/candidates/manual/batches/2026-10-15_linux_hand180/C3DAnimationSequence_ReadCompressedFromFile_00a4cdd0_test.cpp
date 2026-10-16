#include <cstdio>
#include <cstring>
// Behaviour model of the inlined retail stream Read: position range check, chunk fast path, overflow reader.
struct Stream {
    unsigned int pos; const unsigned char* data; int remaining; int overflowCalls; unsigned int start;
    Stream(const unsigned char* d, int n) : pos(0), data(d), remaining(n), overflowCalls(0), start(0) {}
    unsigned int GetPosition() { return pos; }
    void Overflow(void* buf, int size) { ++overflowCalls; std::memset(buf, 0, size); pos += size; }
    void Read(void* buf, int size) {
        if (size <= 0) return;
        if (remaining >= size) { std::memcpy(buf, data, size); data += size; remaining -= size; pos += size; }
        else Overflow(buf, size);
    }
    unsigned int ReadULong() { unsigned int v = 0; Read(&v, 4); return v; }
    float ReadFloat() { float v = 0; Read(&v, 4); return v; }
    bool ReadBool() { unsigned char v = 0; Read(&v, 1); return v != 0; }
};
static void put(unsigned char* b, int at, unsigned int v) { std::memcpy(b + at, &v, 4); }

struct Seq { void* Data; short ParentIndex; bool Shared; };
int main() {
    unsigned char b[8]; put(b, 0, 1); put(b, 4, 0x10005);
    Stream s(b, 8); Seq q; s.GetPosition(); s.ReadULong(); q.ParentIndex = (short)s.ReadULong(); int data = 0; q.Data = &data;
    if (q.ParentIndex != 5 || q.Data != &data) { std::printf("00a4cdd0_TEST FAIL\n"); return 1; }
    std::printf("00a4cdd0_TEST PASS\n"); return 0; }
