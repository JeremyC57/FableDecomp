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
    unsigned char b[12]; put(b, 0, 2); float f1 = 0.5f, f2 = -1.0f; std::memcpy(b + 4, &f1, 4); std::memcpy(b + 8, &f2, 4);
    Stream s(b, 12); unsigned int count = s.ReadULong(); float v[4] = {9, 9, 9, 9};
    for (unsigned int i = 0; i < count; ++i) v[i] = 0.0f;
    s.Read(v, (int)(count * sizeof(float)));
    if (count != 2 || v[0] != 0.5f || v[1] != -1.0f || v[2] != 9) { std::printf("007020d0_TEST FAIL\n"); return 1; }
    std::printf("007020d0_TEST PASS\n"); return 0; }
