// Behaviour test for 0x00515FA0 CReplaceSerialise<COldFileFormatMapHeader5,COldFileFormatMapHeader4>::GetMaxSize(version,
// size): the scratch size is the largest of the record size and every header size from the record's version up to
// version 5 (headers 1-3: 5 bytes, 4: 0x840E, 5: 0x8416); sizes compare unsigned.
#include <cstdio>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

static long level1(long, long size) { return MAX((unsigned long)size, 5u); }
static long level2(long v, long size) { if (v == 2) return MAX((unsigned long)size, 5u); return MAX((unsigned long)level1(v, size), 5u); }
static long level3(long v, long size) { if (v == 3) return MAX((unsigned long)size, 5u); return MAX((unsigned long)level2(v, size), 5u); }
static long level4(long v, long size) { if (v == 4) return MAX((unsigned long)size, 0x840eu); return MAX((unsigned long)level3(v, size), 0x840eu); }
static long level5(long v, long size) { if (v == 5) return MAX((unsigned long)size, 0x8416u); return MAX((unsigned long)level4(v, size), 0x8416u); }

int main()
{
    struct Case { long v, size, want; };
    static const Case cases[] = {
        { 5, 0, 0x8416 }, { 5, 0x9000, 0x9000 }, { 4, 0, 0x8416 }, { 4, 0x8410, 0x8416 }, { 4, 0x9000, 0x9000 },
        { 3, 0, 0x8416 }, { 2, 7, 0x8416 }, { 1, 0x9000, 0x9000 }, { 9, 3, 0x8416 }, { 3, -1, -1 },
    };
    for (unsigned i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i)
        if (level5(cases[i].v, cases[i].size) != cases[i].want) { std::printf("00515fa0_TEST FAIL\n"); return 1; }
    std::printf("00515fa0_TEST PASS\n");
    return 0;
}