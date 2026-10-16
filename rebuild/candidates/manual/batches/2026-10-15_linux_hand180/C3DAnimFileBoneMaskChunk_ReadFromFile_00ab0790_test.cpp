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
    unsigned char b[7]; put(b, 0, 3); b[4] = 0; b[5] = 5; b[6] = 1;
    Stream s(b, 7); unsigned int count = s.ReadULong(); bool mask[8];
    for (unsigned int i = 0; i < count; ++i) mask[i] = s.ReadBool();
    if (count != 3 || mask[0] || !mask[1] || !mask[2] || s.remaining != 0) { std::printf("00ab0790_TEST FAIL\n"); return 1; }
    std::printf("00ab0790_TEST PASS\n"); return 0; }
