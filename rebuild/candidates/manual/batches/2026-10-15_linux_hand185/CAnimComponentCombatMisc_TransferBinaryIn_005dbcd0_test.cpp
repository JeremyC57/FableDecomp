// Behaviour test for 005dbcd0: CAnimComponentCombatMisc::TransferBinaryIn -- two bytes read as bools, overflow path.
#include <cstdio>
#include <cstring>
struct TStream { unsigned long position; const unsigned char* data; long remaining; int overflows;
    void ReadOverflow(unsigned char* buf, long size) { ++overflows; std::memset(buf, 0, size); position += size; }
    void Read(void* buf, long size) {
        long long np = (long long)(position + size);
        if (!(np >= 0 && np <= 0x7fffffff)) return;
        if (remaining >= size) { std::memcpy(buf, data, size); data += size; remaining -= size; position += size; }
        else ReadOverflow((unsigned char*)buf, size); } };
static void TransferBoolIn(TStream* s, bool* out) { unsigned char v = 0; s->Read(&v, 1); *out = v != 0; }
struct TMisc { bool MeleeFlourish, MeleeKnockdown;
    void TransferBinaryIn(TStream* s) { TransferBoolIn(s, &MeleeFlourish); TransferBoolIn(s, &MeleeKnockdown); } };
int main() {
    unsigned char buf[2]; buf[0] = 5; buf[1] = 0;
    TStream s; s.position = 0; s.data = buf; s.remaining = 2; s.overflows = 0;
    TMisc m; m.TransferBinaryIn(&s);
    if (!(m.MeleeFlourish && !m.MeleeKnockdown && s.position == 2)) { std::printf("005dbcd0_TEST FAIL\n"); return 1; }
    buf[0] = 0; buf[1] = 1; s.data = buf; s.remaining = 1; s.position = 0; m.TransferBinaryIn(&s);
    if (!(!m.MeleeFlourish && !m.MeleeKnockdown && s.overflows == 1)) { std::printf("005dbcd0_TEST FAIL\n"); return 1; }
    std::printf("005dbcd0_TEST PASS\n"); return 0; }
