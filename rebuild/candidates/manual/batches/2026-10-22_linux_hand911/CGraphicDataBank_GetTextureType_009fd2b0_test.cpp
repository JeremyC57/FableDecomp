// Behaviour test for 009fd2b0 CGraphicDataBank::GetTextureType: picks the texture-type row from the pixel format
// (signed -> 10s; param && uncompressed -> 8/9; param && compressed -> 4/5; uncompressed -> 6/7; else 0,1,2,3,1) and
// the column from the transparency type (1..4, anything else column 0).
#include <cstdio>
struct CPixelFormat {
    bool compressed, isSigned;
    bool IsCompressed() const { return compressed; }
    bool IsSigned() const { return isSigned; }
};
struct CGraphicHeader { CPixelFormat PixelFormat; unsigned char TransparencyType; };
static long GetTextureType(const CGraphicHeader& header, bool param)
{
    long transparency = header.TransparencyType;
    bool uncompressed = !header.PixelFormat.IsCompressed();
    long type1, type2, type3, type4, type0;
    if (header.PixelFormat.IsSigned()) {
        type0 = 10; type1 = 10; type2 = 10; type4 = 10; type3 = 10;
    } else if (param) {
        if (uncompressed) { type0 = 8; type1 = 9; type2 = 9; type4 = 9; type3 = 9; }
        else { type0 = 4; type1 = 5; type2 = 5; type4 = 5; type3 = 5; }
    } else if (uncompressed) {
        type0 = 6; type1 = 7; type2 = 7; type4 = 7; type3 = 7;
    } else {
        type0 = 0; type1 = 1; type2 = 2; type4 = 1; type3 = 3;
    }
    switch (transparency) {
    case 1: return type1;
    case 2: return type2;
    case 3: return type3;
    case 4: return type4;
    default: return type0;
    }
}
int main()
{
    /* rows: [signed][param][compressed] -> types for transparency 0..5 */
    static const long plain[6] = { 0, 1, 2, 3, 1, 0 };
    int ok = 1;
    for (int t = 0; t < 6; ++t) {
        CGraphicHeader h; h.TransparencyType = (unsigned char)t;
        h.PixelFormat.isSigned = true; h.PixelFormat.compressed = false;
        if (GetTextureType(h, true) != 10 || GetTextureType(h, false) != 10) ok = 0;
        h.PixelFormat.isSigned = false;
        long base = (t >= 1 && t <= 4) ? 1 : 0;
        if (GetTextureType(h, true) != 8 + base) ok = 0;
        if (GetTextureType(h, false) != 6 + base) ok = 0;
        h.PixelFormat.compressed = true;
        if (GetTextureType(h, true) != 4 + base) ok = 0;
        if (GetTextureType(h, false) != plain[t]) ok = 0;
    }
    if (!ok) { std::printf("009fd2b0_TEST FAIL\n"); return 1; }
    std::printf("009fd2b0_TEST PASS\n");
    return 0;
}
