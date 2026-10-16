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

struct Entry { unsigned int first, second; };
int main() {
    unsigned char b[20]; put(b, 0, 2); put(b, 4, 1); put(b, 8, 10); put(b, 12, 2); put(b, 16, 20);
    Stream s(b, 20); unsigned int count = s.ReadULong(); Entry e[4];
    s.Read(e, (int)(count * sizeof(Entry)));
    Stream z(b, 4); unsigned int zc = z.ReadULong(); z.Read(e, 0);
    if (count != 2 || e[1].second != 20 || e[0].first != 1 || zc != 2 || z.overflowCalls != 0) { std::printf("00680950_TEST FAIL\n"); return 1; }
    std::printf("00680950_TEST PASS\n"); return 0; }