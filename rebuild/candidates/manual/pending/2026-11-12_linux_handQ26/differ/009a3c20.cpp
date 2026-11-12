// CStreamingFontBank::CStreamingFontBank(CStreamingFontDataBank*, unsigned long) -- register the bank in the global font-bank table, prime the streaming caches, read the font entry into a byte buffer and parse it through a memory stream: name, heights/weight/italics, the 1025-entry glyph-bank offset table and the glyph-bank memory; then mark every present glyph in GlyphFlags (decompressing LZO-packed banks into the cache) and allocate the texture holder
// Grade: DIFFER(1298v1298) -- ported onto the shared headers (CStreamingFontBank/CFontBank retail branches, CArray, CMemoryDataInputStream, CTexture; overlay ctor + CBankFile::ReadEntryData opt-ins), which fixed the draft's bank-table, byte-vector and read-size residues; only the glyph loop differs: retail keeps i in esi / j in edi (offsets[i] in eax, length in ecx, [ebx+14h] pushed via eax), ours i in edi / j in esi. Tried: start local, offset-first add, integer add, j or i declared outside, Ego_r GlyphBank/CStreamingGlyph types, no locals, header first (residue unchanged 7 grades)
// Leaf: ??0CStreamingFontBank
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A3C20, 1296 bytes, `ret 8`; 8-aligned frame with a 0x1044-byte __chkstk frame (the offset table is a local).
// Callees: CBaseClassNonCopyable ctor 0x0099A2F0, CCharString() 0x0099E4B0 / operator= 0x0099EFB0 / dtor 0x0099EAE0,
//   InitializeStreamingFontTextureCache 0x009A3640 (__fastcall, the data bank), InitializeStreamingFontCacheEntries 0x009A0F00 (__fastcall,
//   globals 0x013C5A98 / 0x013C7AC8), CBankFile::GetEntryDataSize 0x009CC240 / ReadEntryData 0x009CC2A0, malloc 0x00BFEA0E / free 0x00BFEA14
//   (the byte vector), CMemoryDataInputStream ctor 0x00994930 / ReadNullTerminatedString 0x00995D30 / dtor 0x00993B10,
//   ReadWithSrcChunkOverflow 0x00993CA0 (the inlined CDataInputStream::Read), operator new 0x00BFEA1A, operator new[] 0x00BFEB22,
//   lzo1x_decompress_safe 0x00C08170; global font-bank table 0x013BDA50[8].
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CDATASTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CFONTBANK_BASE
#define FABLE_RETAIL_CSTREAMINGFONTBANK
#define FABLE_RETAIL_CSTREAMINGFONTBANK_CTOR
#define FABLE_RETAIL_CBANKFILE_READENTRYDATA
#define FABLE_RETAIL_CBANKFILE_ENTRYDATASIZE
#include "engine/CArray.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CTexture.h"
#include "engine/CBankFile.h"
#include "engine/CStreamingFontDataBank.h"
#include "engine/CStreamingFontBank.h"
void* __cdecl operator new[](unsigned int bytes);       // 0x00BFEB22
extern "C" void* __cdecl memset(void* dest, int value, unsigned int bytes);
#pragma intrinsic(memset)
extern "C" int __cdecl lzo1x_decompress_safe(const unsigned char* src, unsigned long srcLen, unsigned char* dst, unsigned long* dstLen, void* wrk);   // 0x00C08170

struct CStreamingGlyph {                                // Ego_r.pdb, 4 bytes
    unsigned long GlyphIndex : 24;
    unsigned long AdjustmentIndex : 8;
};
struct GlyphBank {                                      // Ego_r.pdb, 0x104 bytes (the streaming-font one)
    unsigned short FirstGlyph;
    unsigned short NoGlyphs;
    CStreamingGlyph Glyphs[64];
};
struct StreamingFontCacheBufferData;
void __fastcall InitializeStreamingFontTextureCache(CStreamingFontDataBank* bank);   // 0x009A3640
void __fastcall InitializeStreamingFontCacheEntries(StreamingFontCacheBufferData& data);   // 0x009A0F00
extern StreamingFontCacheBufferData g_StreamingFontCacheA;   // 0x013C5A98
extern StreamingFontCacheBufferData g_StreamingFontCacheB;   // 0x013C7AC8
extern CStreamingFontBank* g_StreamingFontBanks[8];     // 0x013BDA50

CStreamingFontBank::CStreamingFontBank(CStreamingFontDataBank* bank, unsigned long index)
{
    unsigned long i;
    data_bank = bank;
    fontIndex = 0;
    while (fontIndex < 8 && g_StreamingFontBanks[fontIndex] != this)
        ++fontIndex;
    if (fontIndex == 8) {
        for (fontIndex = 0; fontIndex < 8; ++fontIndex)
            if (g_StreamingFontBanks[fontIndex] == 0)
                break;
        g_StreamingFontBanks[fontIndex] = this;
    }
    InitializeStreamingFontTextureCache(data_bank);
    InitializeStreamingFontCacheEntries(g_StreamingFontCacheA);
    InitializeStreamingFontCacheEntries(g_StreamingFontCacheB);
    memset(GlyphBanks, 0, sizeof(GlyphBanks));
    memset(GlyphFlags, 0, sizeof(GlyphFlags));
    // CStreamingFontDataBank : CBankFile at offset 0 (RTTI); the shared header is the flattened Ego_r layout.
    CBankFile* file = (CBankFile*)data_bank;
    unsigned long size = file->GetEntryDataSize(index);
    CArray<unsigned char> buffer(size);
    ((CBankFile*)data_bank)->ReadEntryData(index, 0, buffer.GetAsCArray(), size);
    CMemoryDataInputStream stream(buffer.GetAsCArray(), size);
    glyphBankCache = new GlyphBank;
    glyphBankCacheIndex = 0xffffffff;
    Name = stream.ReadNullTerminatedString();
    FontHeight = stream.ReadULONG();
    Weight = stream.ReadSLONG();
    Italics = stream.ReadEBOOL();
    MaxHeight = stream.ReadULONG();
    long offsets[0x401];
    stream.Read(offsets, sizeof(offsets));
    GlyphBankMemory = operator new[](offsets[0x400]);
    stream.Read(GlyphBankMemory, offsets[0x400]);
    for (i = 0; i < 0x400; ++i) {
        GlyphBank* header;
        long start = offsets[i];
        unsigned long length = offsets[i + 1] - start;
        if (length == 0)
            continue;
        unsigned char* data = (unsigned char*)GlyphBankMemory + start;
        GlyphBanks[i] = data;
        header = (GlyphBank*)(data + 2);
        if (*(unsigned short*)data & 0x8000) {
            unsigned long outLength = sizeof(GlyphBank);
            lzo1x_decompress_safe(data + 2, length - 2, (unsigned char*)glyphBankCache, &outLength, 0);
            header = glyphBankCache;
            glyphBankCacheIndex = i;
        }
        for (unsigned long j = 0; j < header->NoGlyphs; ++j) {
            if (header->Glyphs[j].GlyphIndex != 0) {
                unsigned short glyph = (unsigned short)((i << 6) + j + header->FirstGlyph);
                GlyphFlags[glyph >> 5] |= 1 << (glyph & 31);
            }
        }
    }
    PTexture = new CTexture;
}
