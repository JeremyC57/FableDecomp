// CStreamingFontBank::CStreamingFontBank(CStreamingFontDataBank*, unsigned long) -- load font metadata and glyph banks, initialize glyph presence and caches.
// Grade: relocation-masked exact1298bytes; original FableWin bankIndex/bankSize/bank/c1/ch scopes and original stream Read scope.
// Leaf: ??0CStreamingFontBank@@QAE@PAVCStreamingFontDataBank@@K@Z
// Retail: Fable.exe 0x009A3C20..0x009A4132,1298bytes,thiscall ret8.
// Layout: original nested GlyphBank260 and CStreamingGlyph4, original private cache types/globals and shared font base.
// Shape: original BankOffsets unsigned-long table and CArray<char> data_buffer; real STLport char fill overload.
// Layout: data bank header is flattened; RTTI proves CBankFile at zero, so its explicit base cast preserves the native access.
#define FABLE_RETAIL_PASS22_STREAMING_FONT_CTOR
#define FABLE_RETAIL_STLP_FILL_CHAR
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CDATASTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM_ORIGINAL_READ_SCOPE
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
#ifdef _MSC_VER
#pragma intrinsic(memset)
#endif
extern "C" int __cdecl lzo1x_decompress_safe(const unsigned char* src, unsigned int srcLen, unsigned char* dst, unsigned int* dstLen, void* wrk);   // 0x00C08170

#include "engine/StreamingFontCache.h"

CStreamingFontBank::CStreamingFontBank(CStreamingFontDataBank* data_bank_, unsigned long index)
{
    data_bank = data_bank_;
    fontIndex = 0;
    while (fontIndex < 8 && streamingFontBankList[fontIndex] != this)
        ++fontIndex;
    if (fontIndex == 8) {
        for (fontIndex = 0; fontIndex < 8; ++fontIndex)
            if (streamingFontBankList[fontIndex] == 0)
                break;
        streamingFontBankList[fontIndex] = this;
    }
    InitializeStreamingFontTextureCache(data_bank);
    InitializeStreamingFontCacheEntries(g_StreamingFontCacheBufferData);
    InitializeStreamingFontCacheEntries(g_StreamingFontCacheBufferData2);
    memset(GlyphBanks, 0, sizeof(GlyphBanks));
    memset(GlyphFlags, 0, sizeof(GlyphFlags));
    // CStreamingFontDataBank : CBankFile at offset 0 (RTTI); the shared header is the flattened Ego_r layout.
    CBankFile* file = (CBankFile*)data_bank;
    unsigned long data_size = file->GetEntryDataSize(index);
    CArray<char> data_buffer(data_size);
    ((CBankFile*)data_bank)->ReadEntryData(index, 0, data_buffer.GetAsCArray(), data_size);
    CMemoryDataInputStream stream(data_buffer.GetAsCArray(), data_size);
    glyphBankCache = new GlyphBank;
    glyphBankCacheIndex = 0xffffffff;
    Name = stream.ReadNullTerminatedString();
    FontHeight = stream.ReadULONG();
    Weight = stream.ReadSLONG();
    Italics = stream.ReadEBOOL();
    MaxHeight = stream.ReadULONG();
    unsigned long BankOffsets[0x401];
    stream.Read(BankOffsets, sizeof(BankOffsets));
    GlyphBankMemory = operator new[](BankOffsets[0x400]);
    stream.Read(GlyphBankMemory, BankOffsets[0x400]);
    for (unsigned long bankIndex = 0; bankIndex < 0x400; ++bankIndex) {
        const unsigned long bankSize = BankOffsets[bankIndex + 1] - BankOffsets[bankIndex];
        if (bankSize != 0) {
            GlyphBanks[bankIndex] = (const unsigned char*)GlyphBankMemory + BankOffsets[bankIndex];
            const GlyphBank* bank = (const GlyphBank*)((const unsigned char*)GlyphBanks[bankIndex] + 2);
            if (*(const unsigned short*)GlyphBanks[bankIndex] & 0x8000) {
                unsigned int output_size = sizeof(GlyphBank);
                lzo1x_decompress_safe((const unsigned char*)GlyphBanks[bankIndex] + 2, bankSize - 2, (unsigned char*)glyphBankCache, &output_size, 0);
                bank = glyphBankCache;
                glyphBankCacheIndex = bankIndex;
            }
            for (unsigned long c1 = 0; c1 < bank->NoGlyphs; ++c1) {
                if (bank->Glyphs[c1].GlyphIndex != 0) {
                    const unsigned short ch = (unsigned short)((bankIndex << 6) + c1 + bank->FirstGlyph);
                    GlyphFlags[ch >> 5] |= 1 << (ch & 31);
                }
            }
        }
    }
    PTexture = new CTexture;
}