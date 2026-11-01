// Behaviour test for 0x009A1090 CStreamingFontBank::~CStreamingFontBank: the font slot is cleared only when it holds this bank,
// every glyph-cache entry owned by the bank's font index loses its slot bits and glyph (other bits and entries untouched),
// the glyph memory / bank cache / texture are freed, PTexture is zeroed, then ~CFontBank (Name) runs.
#include <cstdio>
static int g_log[16]; static int g_n = 0;
static void Log(int v) { if (g_n < 16) g_log[g_n++] = v; }
class CTexture { public: int Id; void Uninitialise() { Log(Id); } ~CTexture() { Uninitialise(); } };
class CCharString { public: int Id; ~CCharString() { Log(Id); } };
class CFontBank { public: unsigned long FontHeight; CCharString Name; void* PManager; virtual ~CFontBank() { } };
struct GlyphBank { unsigned short FirstGlyph; unsigned short NoGlyphs; };
struct SEntry { unsigned short FontIndex : 4; unsigned short Flags : 12; unsigned short Glyph; unsigned long Data[3]; };
struct CStreamingFontBank;
static CStreamingFontBank* g_Banks[8];
static SEntry g_Cache[256][8];
struct CStreamingFontBank : public CFontBank {
    GlyphBank* glyphBankCache; void* GlyphBankMemory; unsigned long fontIndex; CTexture* PTexture;
    virtual ~CStreamingFontBank()
    {
        if (g_Banks[fontIndex] == this)
            g_Banks[fontIndex] = 0;
        SEntry* entry = &g_Cache[0][0];
        for (long i = 0; i < 256; ++i)
            for (long j = 0; j < 8; ++j, ++entry)
                if (entry->FontIndex == fontIndex) { entry->Glyph = 0; entry->FontIndex = 0; }
        delete [] (char*)GlyphBankMemory;
        delete glyphBankCache;
        delete PTexture;
        PTexture = 0;
    }
};
int main()
{
    for (int i = 0; i < 256; ++i)
        for (int j = 0; j < 8; ++j) { g_Cache[i][j].FontIndex = (i + j) & 0xf; g_Cache[i][j].Flags = 0x123; g_Cache[i][j].Glyph = (unsigned short)(i * 8 + j + 1); }
    {
        CStreamingFontBank b; b.Name.Id = 77; b.fontIndex = 3; b.glyphBankCache = new GlyphBank; b.GlyphBankMemory = new char[16];
        b.PTexture = new CTexture; b.PTexture->Id = 55;
        g_Banks[3] = &b;
    }
    if (g_Banks[3] != 0) { std::printf("009a1090_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 256; ++i)
        for (int j = 0; j < 8; ++j) {
            const SEntry& e = g_Cache[i][j];
            if (e.Flags != 0x123) { std::printf("009a1090_TEST FAIL\n"); return 1; }
            if (((i + j) & 0xf) == 3) { if (e.FontIndex != 0 || e.Glyph != 0) { std::printf("009a1090_TEST FAIL\n"); return 1; } }
            else if (e.FontIndex != ((i + j) & 0xf) || e.Glyph != (unsigned short)(i * 8 + j + 1)) { std::printf("009a1090_TEST FAIL\n"); return 1; }
        }
    {   // a bank that does not own its slot leaves the slot alone
        CStreamingFontBank b; b.Name.Id = 78; b.fontIndex = 5; b.glyphBankCache = 0; b.GlyphBankMemory = 0; b.PTexture = 0;
        CStreamingFontBank* marker = (CStreamingFontBank*)&g_Cache[0][0];
        g_Banks[5] = marker;
    }
    if (g_Banks[5] != (CStreamingFontBank*)&g_Cache[0][0]) { std::printf("009a1090_TEST FAIL\n"); return 1; }
    if (g_n != 3 || g_log[0] != 55 || g_log[1] != 77 || g_log[2] != 78) { std::printf("009a1090_TEST FAIL\n"); return 1; }
    std::printf("009a1090_TEST PASS\n");
    return 0;
}