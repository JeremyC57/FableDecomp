// Behaviour test for 0x00AB8E10 CStaticFontBank::CStaticFontBank: a standalone copy of the reconstructed parse over a
// bounds-checked memory stream (the inline Read: a read past the end leaves the destination untouched).
#include <cstdio>
#include <cstring>
#include <vector>
#include <string>
struct Stream {
    const unsigned char* p; long remaining; unsigned int pos;
    void Read(void* out, long size) { if (size <= remaining) { std::memcpy(out, p, size); p += size; remaining -= size; pos += size; } }
    unsigned int ULong() { unsigned int v = 0xCDCDCDCD; Read(&v, 4); return v; }
    bool EBool() { unsigned char v = 0; Read(&v, 1); return v != 0; }
    std::string Str() { std::string s; while (remaining > 0) { char c = *p++; --remaining; ++pos; if (!c) break; s += c; } return s; }
};
struct Glyph { float l, t, r, b; short off, w, adv; void Read(Stream& s) { s.Read(&l, 16); s.Read(&off, 6); } };
struct Bank { unsigned short first, count; Glyph* glyphs; };
struct Font {
    std::string name; unsigned int height, maxHeight; int weight, texX, texY, minChar, maxChar; bool italics;
    Bank banks[1024]; std::vector<unsigned char> pixels;
    Font(const std::vector<unsigned char>& entry) {
        std::memset(banks, 0, sizeof(banks));
        Stream s = { entry.empty() ? 0 : &entry[0], (long)entry.size(), 0 };
        name = s.Str(); height = s.ULong(); weight = (int)s.ULong(); italics = s.EBool(); maxHeight = s.ULong();
        texX = (int)s.ULong(); texY = (int)s.ULong(); minChar = (int)s.ULong(); maxChar = (int)s.ULong();
        unsigned int n = s.ULong();
        for (unsigned int i = 0; i < n; ++i) {
            Bank& b = banks[s.ULong()];
            b.first = (unsigned short)s.ULong(); b.count = (unsigned short)s.ULong();
            b.glyphs = new Glyph[b.count];
            for (unsigned int j = 0; j < b.count; ++j) b.glyphs[j].Read(s);
        }
        unsigned int imageSize = s.ULong();
        unsigned char header[18]; s.Read(header, 18);
        pixels.assign(imageSize, 0);
        long bytes = (long)imageSize - 18;
        if (bytes > 0) s.Read(&pixels[0], bytes);
    }
};
static void PutB(std::vector<unsigned char>& v, const void* p, int n) { for (int i = 0; i < n; ++i) v.push_back(((const unsigned char*)p)[i]); }
static void PutU(std::vector<unsigned char>& v, unsigned int x) { for (int i = 0; i < 4; ++i) v.push_back((unsigned char)(x >> (8 * i))); }
int main() {
    std::vector<unsigned char> e;
    const char* nm = "Arial"; PutB(e, nm, 6);
    PutU(e, 12); PutU(e, 400); e.push_back(1); PutU(e, 14); PutU(e, 256); PutU(e, 128); PutU(e, 32); PutU(e, 255);
    PutU(e, 1); PutU(e, 3); PutU(e, 0x300); PutU(e, 2);
    for (int g = 0; g < 2; ++g) { float f[4] = { 0.1f * g, 0.2f, 0.3f, 0.4f }; PutB(e, f, 16); short sh[3] = { (short)g, 7, 8 }; PutB(e, sh, 6); }
    PutU(e, 18 + 4); for (int i = 0; i < 18; ++i) e.push_back(0); e.push_back(1); e.push_back(2); e.push_back(3); e.push_back(4);
    Font f(e);
    int fails = 0;
    if (f.name != "Arial" || f.height != 12 || f.weight != 400 || !f.italics || f.maxHeight != 14) ++fails;
    if (f.texX != 256 || f.texY != 128 || f.minChar != 32 || f.maxChar != 255) ++fails;
    if (f.banks[3].first != 0x300 || f.banks[3].count != 2 || !f.banks[3].glyphs || f.banks[3].glyphs[1].off != 1 || f.banks[0].glyphs) ++fails;
    if (f.pixels.size() != 22 || f.pixels[0] != 1 || f.pixels[3] != 4 || f.pixels[4] != 0) ++fails;   // image copy fills the first size-18 bytes
    if (fails) { std::printf("00ab8e10_TEST FAIL %d\n", fails); return 1; }
    std::printf("00ab8e10_TEST PASS\n");
    return 0;
}