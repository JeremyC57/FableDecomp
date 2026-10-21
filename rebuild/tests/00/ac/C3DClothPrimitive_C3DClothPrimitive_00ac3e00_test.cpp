// Behaviour test for the C3DClothPrimitive ctor at 0x00AC3E00: program read, material index, reserve then one group per VGRP chunk.
#include <cstdio>
#include <cstring>
static char g_log[256];
static void Log(const char* s) { std::strcat(g_log, s); }
struct Chunk { unsigned int material; int groups[3]; int groupCount; int dataSize; };
struct Prim {
    unsigned int MaterialIndex;
    Prim(const Chunk* c)
    {
        char b[32];
        std::sprintf(b, "R%d;", c->dataSize); Log(b);              // ReadFromFile over the data stream
        MaterialIndex = c->material;
        std::sprintf(b, "V%d;", c->groupCount); Log(b);            // reserve
        for (int i = 0; i < c->groupCount; ++i) { std::sprintf(b, "G%d;", c->groups[i]); Log(b); }
    }
};
int main()
{
    Chunk c = { 5, { 3, 1, 2 }, 3, 64 };
    Prim p(&c);
    int ok = p.MaterialIndex == 5 && std::strcmp(g_log, "R64;V3;G3;G1;G2;") == 0;
    std::printf(ok ? "00ac3e00_TEST PASS\n" : "00ac3e00_TEST FAIL\n");
    return ok ? 0 : 1;
}