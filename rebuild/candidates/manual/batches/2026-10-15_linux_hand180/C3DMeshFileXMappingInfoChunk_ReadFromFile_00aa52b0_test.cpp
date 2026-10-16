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

int main() {
    unsigned char b[8]; put(b, 0, 1); float f = 2.5f; std::memcpy(b + 4, &f, 4);
    Stream s(b, 8); s.ReadULong(); float stretch = s.ReadFloat();
    Stream t(b, 6); t.ReadULong(); t.ReadFloat();
    if (stretch != 2.5f || s.pos != 8 || s.overflowCalls != 0 || t.overflowCalls != 1) { std::printf("00aa52b0_TEST FAIL\n"); return 1; }
    std::printf("00aa52b0_TEST PASS\n"); return 0; }
