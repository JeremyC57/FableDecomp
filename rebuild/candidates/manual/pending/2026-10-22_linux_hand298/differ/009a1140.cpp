// Grade: DIFFER(871v918) -- retail keeps ch in its param slot, this in ebx spilled to the frame, BigChar tested via shr/js forms; 47 bytes larger
// CStreamingFontBank::GlyphLookup(unsigned short ch) -- find (or stream in) the cached lookup record of a character: fall back to U+25A1 for missing glyphs, search the character's 8-way hash row, else take a free slot, a slot whose texture-cache entry was recycled, or the least recently used one, and fill it from the (decompressed) glyph bank
// Leaf: GlyphLookup
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A1140, 918 bytes, `ret 4` (thiscall; the landed GetStringExtents 0x009A14E0 calls it).
// Callees: GFGetSystemManager 0x009A4EC0 (__fastcall; render manager +0x60, frame counter +0x25c), CStreamingFontBank glyph
//   index lookup 0x009A4160 (thiscall, character), the glyph-bank decompressor 0x00C08170 (__cdecl, 5 arguments).
// Globals: lookup cache 0x013BDA70 (256 rows x 8 CStreamingGlyphLookup), the big/small glyph texture caches 0x013C7AC8 /
//   0x013C5A98 (entry array at +0x18, 16-byte entries: last-used frame +4, CStreamingGlyph +8), glyphs per bank 0x0138E068,
//   resident bank 0x0138E06C, bank offsets 0x013BDA34, packed banks 0x013BDA38, decompressed bank 0x013BDA3C.
// Layout: engine/CStreamingFontBank.h (GlyphFlags +0x102c, fontIndex +0x302c); PDB CStreamingGlyphLookup (font:4 /
//   cachePos:12, ch +2, indices +4, data +8) as the landed GetStringExtents.
#include "engine/CStreamingFontBank.h"

struct CStreamingGlyph {
    unsigned long GlyphIndex : 24;
    unsigned long AdjustmentIndex : 8;
};
struct CStreamingGlyphData {
    signed char   OffsetX, OffsetY, Width, Height;
    unsigned long MemOffset : 23;
    unsigned long Advance : 8;
    unsigned long BigChar : 1;
};
struct CStreamingGlyphLookup {
    unsigned short      font : 4;
    unsigned short      cachePos : 12;
    unsigned short      ch;
    CStreamingGlyph     indices;
    CStreamingGlyphData data;
};
struct CGlyphCacheEntry {
    unsigned long   Pad00;
    unsigned long   LastUsedFrame;     // +4
    CStreamingGlyph Glyph;             // +8
    unsigned long   Pad0C;
};
struct CGlyphTextureCache {
    unsigned char     _pad[0x18];
    CGlyphCacheEntry* Entries;         // +0x18
};
extern CGlyphTextureCache g_BigGlyphCache;                       // 0x013C7AC8
extern CGlyphTextureCache g_SmallGlyphCache;                     // 0x013C5A98
extern CStreamingGlyphLookup g_GlyphLookupCache[256][8];         // 0x013BDA70
extern unsigned long g_GlyphsPerBank;                            // 0x0138E068
extern unsigned long g_ResidentGlyphBank;                        // 0x0138E06C
extern unsigned long* g_GlyphBankOffsets;                        // 0x013BDA34
extern unsigned char* g_GlyphBankData;                           // 0x013BDA38
extern CStreamingGlyphData* g_GlyphBankBuffer;                   // 0x013BDA3C
void __cdecl GFDecompressGlyphBank(const void* src, unsigned long srcSize, void* dst, unsigned long* dstSize, int flags);   // 0x00C08170
struct CRenderManagerFrame { unsigned char _pad[0x25c]; unsigned long FrameCount; };
struct CSystemManager { unsigned char _pad[0x60]; CRenderManagerFrame* RenderManager; };
CSystemManager* __fastcall GFGetSystemManager();                 // 0x009A4EC0

inline CGlyphTextureCache* GetGlyphCache(const CStreamingGlyphLookup& e)
{
    return e.data.BigChar ? &g_BigGlyphCache : &g_SmallGlyphCache;
}

struct CStreamingFontBankX : CStreamingFontBank {
    bool HasGlyph(unsigned short c) const { return (GlyphFlags[c >> 5] & (1 << (c & 31))) != 0; }
    const CStreamingGlyph* GetGlyph(unsigned short c) const;     // 0x009A4160
    CStreamingGlyphLookup* GlyphLookup(unsigned short ch) const;
};

CStreamingGlyphLookup* CStreamingFontBankX::GlyphLookup(unsigned short ch) const
{
    while (!HasGlyph(ch)) {
        if (ch == 0x25a1)
            return 0;
        ch = 0x25a1;
    }
    unsigned long hash = ch & 0xff;
    CStreamingGlyphLookup* row = g_GlyphLookupCache[hash];
    unsigned long slot;
    for (slot = 0; slot < 8; ++slot)
        if (row[slot].ch == ch && row[slot].font == fontIndex)
            break;
    if (slot == 8) {
        for (slot = 0; slot < 8; ++slot)
            if (row[slot].ch == 0)
                break;
        if (slot == 8) {
            for (slot = 0; slot < 8; ++slot)
                if (GetGlyphCache(row[slot])->Entries[row[slot].cachePos].Glyph.GlyphIndex != row[slot].indices.GlyphIndex)
                    break;
            if (slot == 8) {
                slot = 0;
                unsigned long now = GFGetSystemManager()->RenderManager->FrameCount;
                unsigned long oldest = 0;
                for (unsigned long i = 0; i < 8; ++i) {
                    unsigned long age = now - GetGlyphCache(row[i])->Entries[row[i].cachePos].LastUsedFrame;
                    if (oldest <= age) {
                        slot = i;
                        oldest = age;
                    }
                }
                if (slot == 8)
                    return 0;
            }
        }
        const CStreamingGlyph* glyph = GetGlyph(ch);
        unsigned long index = glyph->GlyphIndex - 1;
        unsigned long bank = index / g_GlyphsPerBank;
        unsigned long within = index % g_GlyphsPerBank;
        if (g_ResidentGlyphBank != bank) {
            unsigned long size = g_GlyphsPerBank * sizeof(CStreamingGlyphData);
            GFDecompressGlyphBank(g_GlyphBankData + g_GlyphBankOffsets[bank], g_GlyphBankOffsets[bank + 1] - g_GlyphBankOffsets[bank],
                g_GlyphBankBuffer, &size, 0);
            g_ResidentGlyphBank = bank;
        }
        CStreamingGlyphLookup& e = g_GlyphLookupCache[hash][slot];
        e.ch = ch;
        e.font = fontIndex;
        e.cachePos = 0;
        e.indices = *glyph;
        e.data = g_GlyphBankBuffer[within];
    }
    return &g_GlyphLookupCache[hash][slot];
}
