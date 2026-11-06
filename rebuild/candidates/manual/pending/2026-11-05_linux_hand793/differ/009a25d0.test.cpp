// Behaviour test for 009a25d0 (CStreamingFontBank::Render): a standalone copy of the shared quad index list (0,1,2, 2,3,0 per glyph,
// 64 glyphs), the glyph cache cell to texture UV mapping and the rounded advance.
#include <cstdio>
static unsigned short indices[64 * 6];
static void BuildIndices()
{
    unsigned short* p = indices;
    for (unsigned long i = 0; i < 64; ++i) {
        unsigned short b = (unsigned short)(i * 4);
        *p++ = b; *p++ = b + 1; *p++ = b + 2; *p++ = b + 2; *p++ = b + 3; *p++ = b;
    }
}
static signed char Round(float v) { return (signed char)(long)(v < 0.0f ? v - 0.5f : v + 0.5f); }
int main()
{
    int fails = 0;
    BuildIndices();
    if (indices[6] != 4 || indices[10] != 7 || indices[11] != 4 || indices[383] != 252) ++fails;
    unsigned long cell = 37, per_row = 16, rows = 8;
    float u = (float)(cell % per_row) / (float)per_row, v = (float)(cell / per_row) / (float)rows;
    if (u != 5.0f / 16.0f || v != 2.0f / 8.0f) ++fails;
    if (Round(2.5f) != 3 || Round(-2.5f) != -3 || Round(2.4f) != 2) ++fails;
    if (fails) { std::printf("009a25d0_TEST FAIL %d\n", fails); return 1; }
    std::printf("009a25d0_TEST PASS\n");
    return 0;
}
