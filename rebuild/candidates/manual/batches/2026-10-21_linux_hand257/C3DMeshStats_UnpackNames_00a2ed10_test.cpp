// Behaviour test for C3DMeshStats::UnpackNames (0x00A2ED10): packed helper/dummy name lists, prefix filter,
// one copy per stored object (counted by CRC key) or one copy each when `once` is set.
#include <cstdio>
#include <cstring>
static unsigned int Crc(const char* t) { unsigned int h = 7; while (*t) h = h * 31 + (unsigned char)*t++; return h; }
struct CNames { char items[16][16]; int n; void push_back(const char* s) { std::strcpy(items[n++], s); } };
struct CMap { unsigned int keys[8]; int n; int Count(unsigned int k) const { int c = 0; for (int i = 0; i < n; ++i) if (keys[i] == k) ++c; return c; } };
struct CStats {
    CMap HelperPoints, DummyObjects; char* PackedNames;
    void UnpackNames(CNames& names, bool dummyObjects, const char* prefix, bool once) const
    {
        if (!PackedNames) return;
        const char* text = PackedNames;
        unsigned int dummyNamesOffset = *(const unsigned short*)text;
        text += sizeof(unsigned short);
        if (dummyObjects) text = PackedNames + dummyNamesOffset;
        int prefixLength = (int)std::strlen(prefix);
        unsigned int length;
        while ((length = (unsigned int)std::strlen(text)) != 0) {
            if (prefixLength == 0 || std::strncmp(text, prefix, prefixLength) == 0) {
                int count = 1;
                if (!once) count = dummyObjects ? DummyObjects.Count(Crc(text)) : HelperPoints.Count(Crc(text));
                for (int i = 0; i < count; ++i) names.push_back(text);
            }
            text += length + 1;
        }
    }
};
int main()
{
    char packed[64]; std::memset(packed, 0, sizeof(packed));
    // helper names at +2: "hp_a", "hp_b", "foot"; dummy names at +18: "dm_x"
    std::memcpy(packed + 2, "hp_a\0hp_b\0foot\0", 15);
    unsigned short off = 18; std::memcpy(packed, &off, 2);
    std::memcpy(packed + 18, "dm_x\0", 5);
    CStats s; s.PackedNames = packed;
    s.HelperPoints.n = 3; s.HelperPoints.keys[0] = Crc("hp_a"); s.HelperPoints.keys[1] = Crc("hp_a"); s.HelperPoints.keys[2] = Crc("foot");
    s.DummyObjects.n = 1; s.DummyObjects.keys[0] = Crc("dm_x");
    CNames a; a.n = 0; s.UnpackNames(a, false, "hp", false);      // hp_a twice, hp_b none
    CNames b; b.n = 0; s.UnpackNames(b, false, "", true);         // all three once
    CNames c; c.n = 0; s.UnpackNames(c, true, "", false);         // dm_x once
    CStats e; e.PackedNames = 0; CNames d; d.n = 0; e.UnpackNames(d, false, "", true);
    if (a.n != 2 || std::strcmp(a.items[1], "hp_a") != 0 || b.n != 3 || std::strcmp(b.items[2], "foot") != 0 ||
        c.n != 1 || std::strcmp(c.items[0], "dm_x") != 0 || d.n != 0) {
        std::printf("00a2ed10_TEST FAIL\n"); return 1;
    }
    std::printf("00a2ed10_TEST PASS\n");
    return 0;
}
