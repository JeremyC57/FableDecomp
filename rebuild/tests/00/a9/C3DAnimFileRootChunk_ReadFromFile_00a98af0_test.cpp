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
    unsigned char b[5]; b[0] = 7; float f = 1.5f; std::memcpy(b + 1, &f, 4);
    Stream s(b, 5); unsigned int start = s.GetPosition(); bool loop = s.ReadBool(); float dur = s.ReadFloat();
    unsigned int rest = 20 - (s.GetPosition() - start);
    if (!loop || dur != 1.5f || rest != 15) { std::printf("00a98af0_TEST FAIL\n"); return 1; }
    std::printf("00a98af0_TEST PASS\n"); return 0; }