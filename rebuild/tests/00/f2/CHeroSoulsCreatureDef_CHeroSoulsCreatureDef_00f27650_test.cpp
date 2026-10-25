// Behaviour test for 00f27650: CHeroSoulsCreatureDef::CHeroSoulsCreatureDef() -- base ctor first, then CreatureType and Location in order; NumCreatures left untouched
#include <cstdio>
#include <cstring>
#include <new>
static char g_seq[8] = ""; static void* g_at[2]; static int g_n = 0;
struct Str { void* PData; Str() : PData(0) { std::strcat(g_seq, "s"); g_at[g_n++] = this; } };
struct DefBase { virtual ~DefBase() { } unsigned char body[0x24]; DefBase() { std::strcat(g_seq, "B"); } };
struct CreatureDef : DefBase { Str CreatureType; Str Location; long NumCreatures; CreatureDef() { } };
int main()
{
    unsigned char raw[sizeof(CreatureDef)]; std::memset(raw, 0x5A, sizeof raw);
    CreatureDef* d = new (raw) CreatureDef;
    int ok = sizeof(CreatureDef) == 0x34 && std::strcmp(g_seq, "Bss") == 0 && g_at[0] == &d->CreatureType && g_at[1] == &d->Location
          && d->NumCreatures == 0x5A5A5A5A && d->CreatureType.PData == 0;
    std::printf(ok ? "00f27650_TEST PASS\n" : "00f27650_TEST FAIL\n");
    return ok ? 0 : 1;
}
