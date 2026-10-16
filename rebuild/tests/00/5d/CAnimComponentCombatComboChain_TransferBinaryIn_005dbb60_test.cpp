// Behaviour test for 005dbb60 (see src/005dbb60.cpp): CAnimComponentCombatComboChain::TransferBinaryIn: two longs from the chunk, overflow path, range check; plain C++98, member access only.
#include <cstdio>
#include <cstring>
// shared test model: a thing with an interface table (sorted type -> component), a dead bit, and an
// intelligent pointer; mirrors the HasTC / GetInterface / FindEntry shapes of the sources.
struct TThing {
    unsigned long bits[9];
    long types[8];
    void* comps[8];
    int count;
    unsigned char flags91;
    unsigned long flags6c;
    TThing() : count(0), flags91(0), flags6c(0) { for (int i = 0; i < 9; ++i) bits[i] = 0; }
    void Add(long type, void* comp) { types[count] = type; comps[count] = comp; ++count; bits[type >> 5] |= 1UL << (type & 31); }
    unsigned long HasTC(long type) const { return bits[type >> 5] & (1UL << (type & 31)); }
    void* Find(long type) const { for (int i = 0; i < count; ++i) if (types[i] == type) return comps[i]; return 0; }
    template<class T> bool GetInterface(T** out, long type) const { if (!HasTC(type)) return false; *out = (T*)Find(type); return true; }
};
struct TPtr {
    TThing* item;
    TPtr() : item(0) { }
    TThing* GetPItem() const { return item; }
    void SetPItem(TThing* t) { item = t; }
};
inline bool Alive(const TPtr& p) { return p.GetPItem() != 0 && !(p.GetPItem()->flags91 & 1); }
#define CHECK(c, addr) do { if (!(c)) { std::printf("%s_TEST FAIL\n", addr); return 1; } } while (0)

struct TStream { unsigned long position; const unsigned char* data; long remaining; int overflows;
    void ReadOverflow(unsigned char* buf, long size) { ++overflows; std::memset(buf, 0x11, size); position += size; }
    void Read(void* buf, long size) {
        long long np = (long long)(position + size);
        if (!(np >= 0 && np <= 0x7fffffff)) return;
        if (remaining >= size) { std::memcpy(buf, data, size); data += size; remaining -= size; position += size; }
        else ReadOverflow((unsigned char*)buf, size); } };
static void TransferLongIn(TStream* s, int* out) { int v = -7; s->Read(&v, 4); *out = v; }
struct TChain { int ComboStage, ComboID; void TransferBinaryIn(TStream* s) { TransferLongIn(s, &ComboStage); TransferLongIn(s, &ComboID); } };
int main() {
    int buf[2]; buf[0] = 3; buf[1] = 9;
    TStream s; s.position = 0; s.data = (const unsigned char*)buf; s.remaining = 8; s.overflows = 0;
    TChain c; c.TransferBinaryIn(&s); CHECK(c.ComboStage == 3 && c.ComboID == 9 && s.position == 8 && s.overflows == 0, "005dbb60");
    s.data = (const unsigned char*)buf; s.remaining = 4; s.position = 0; c.TransferBinaryIn(&s);
    CHECK(c.ComboStage == 3 && c.ComboID == 0x11111111 && s.overflows == 1, "005dbb60");
    std::printf("005dbb60_TEST PASS\n"); return 0; }