// Behaviour test for 005dba80: CAnimComponentCombatHandedness::TransferBinaryIn -- two 4-byte enums from the chunk, overflow path.
#include <cstdio>
#include <cstring>
struct TStream { unsigned long position; const unsigned char* data; long remaining; int overflows;
    void ReadOverflow(unsigned char* buf, long size) { ++overflows; std::memset(buf, 0x11, size); position += size; }
    void Read(void* buf, long size) {
        long long np = (long long)(position + size);
        if (!(np >= 0 && np <= 0x7fffffff)) return;
        if (remaining >= size) { std::memcpy(buf, data, size); data += size; remaining -= size; position += size; }
        else ReadOverflow((unsigned char*)buf, size); } };
static void TransferLongIn(TStream* s, int* out) { int v = -7; s->Read(&v, 4); *out = v; }
struct THand { int StartHandedness, EndHandedness;
    void TransferBinaryIn(TStream* s) { TransferLongIn(s, &StartHandedness); TransferLongIn(s, &EndHandedness); } };
int main() {
    int buf[2]; buf[0] = 1; buf[1] = 2;
    TStream s; s.position = 0; s.data = (const unsigned char*)buf; s.remaining = 8; s.overflows = 0;
    THand h; h.TransferBinaryIn(&s);
    if (!(h.StartHandedness == 1 && h.EndHandedness == 2 && s.position == 8 && s.overflows == 0)) { std::printf("005dba80_TEST FAIL\n"); return 1; }
    s.data = (const unsigned char*)buf; s.remaining = 4; s.position = 0; h.TransferBinaryIn(&s);
    if (!(h.StartHandedness == 1 && h.EndHandedness == 0x11111111 && s.overflows == 1)) { std::printf("005dba80_TEST FAIL\n"); return 1; }
    std::printf("005dba80_TEST PASS\n"); return 0; }
