// Behaviour test for 0x006E0E80 (CTCDParticleEmitter sound hook): with a sound component (interface 16 present in the
// bitset and found by the lower-bound lookup) the thing plays "CREATE_PARTICLE_" + the upper-cased live-emitter name, positional, with
// (0, 0, -1); without the component nothing is played.
#include <cstdio>
#include <cstring>
#include <cctype>

struct Sound { int plays; char last[64]; bool positional; long a, b, c;
    unsigned long PlaySound(const char* name, bool p, long x, long y, long z) { ++plays; std::strcpy(last, name); positional = p; a = x; b = y; c = z; return 1; } };
struct Entry { long type; Sound* component; };
struct Thing {
    unsigned int bits[9]; Entry entries[4]; int count; const char* emitterName;
    Entry* LowerBound(long t) { int i = 0; while (i < count && entries[i].type < t) ++i; return entries + i; }
    bool GetInterface(Sound** out, long t)
    {
        if (bits[t >> 5] & (1u << (t & 31))) {
            Entry* it = LowerBound(t); Entry* last = entries + count;
            if (it == last || t < it->type) it = last;
            *out = it->component; return true;
        }
        return false;
    }
};

static void SendSound(Thing& thing)
{
    const char* emitterName = thing.emitterName;
    Sound* sound;
    if (thing.GetInterface(&sound, 16)) {
        char name[64]; std::strcpy(name, "CREATE_PARTICLE_");
        size_t n = std::strlen(name);
        for (const char* p = emitterName; *p; ++p) name[n++] = (char)std::toupper((unsigned char)*p);
        name[n] = 0;
        sound->PlaySound(name, true, 0, 0, -1);
    }
}

int main()
{
    Sound s; s.plays = 0;
    Thing t; std::memset(t.bits, 0, sizeof(t.bits)); t.count = 2;
    t.entries[0].type = 3; t.entries[0].component = 0; t.entries[1].type = 16; t.entries[1].component = &s;
    t.entries[2].type = 0; t.entries[2].component = 0; t.emitterName = "smoke";
    SendSound(t);
    if (s.plays != 0) { std::printf("006e0e80_TEST FAIL\n"); return 1; }
    t.bits[0] = 0x10000;
    SendSound(t);
    if (s.plays != 1 || std::strcmp(s.last, "CREATE_PARTICLE_SMOKE") != 0 || !s.positional || s.a != 0 || s.b != 0 || s.c != -1) { std::printf("006e0e80_TEST FAIL\n"); return 1; }
    std::printf("006e0e80_TEST PASS\n");
    return 0;
}