// CStreamingFontBank::CStreamingFontBank(CStreamingFontDataBank*, unsigned long) -- register the bank in the global font-bank table, prime the streaming caches, read the font entry into a byte buffer and parse it through a memory stream: name, heights/weight/italics, the 1025-entry glyph-bank offset table and the glyph-bank memory; then mark every present glyph in GlyphFlags (decompressing LZO-packed banks into the cache) and allocate the texture holder
// Grade: DIFFER(1298v1298) -- same length; (1) bank-table search: retail hoists mov ecx,8 above the first loop and aligns the second loop, ours compares the first loop against an immediate and aligns it; (2) byte vector: retail zeroes its three words with eax and takes the null-size path as a branch (xor edi,edi), ours uses edi as the zero register; (3) mov edi,4 (read size) is set before the first Read in retail; (4) glyph loop: retail keeps offsets[i] in eax / length in ecx and the strength-reduced GlyphBanks pointer in [esp+0xc] with the glyph-entry pointer in [esp+0x40]; ours uses ecx/edi and swaps the two slots
// Leaf: ??0CStreamingFontBank
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A3C20, 1296 bytes, `ret 8`; 8-aligned frame with a 0x1044-byte __chkstk frame (the offset table is a local).
// Callees: CBaseClassNonCopyable ctor 0x0099A2F0, CCharString() 0x0099E4B0 / operator= 0x0099EFB0 / dtor 0x0099EAE0,
//   InitializeStreamingFontTextureCache 0x009A3640 (__fastcall, the data bank), InitializeStreamingFontCacheEntries 0x009A0F00 (__fastcall,
//   globals 0x013C5A98 / 0x013C7AC8), CBankFile::GetEntryDataSize 0x009CC240 / ReadEntryData 0x009CC2A0, malloc 0x00BFEA0E / free 0x00BFEA14
//   (the byte vector), CMemoryDataInputStream ctor 0x00994930 / ReadNullTerminatedString 0x00995D30 / dtor 0x00993B10,
//   ReadWithSrcChunkOverflow 0x00993CA0 (the inlined CDataInputStream::Read), operator new 0x00BFEA1A, operator new[] 0x00BFEB22,
//   lzo1x_decompress_safe 0x00C08170; global font-bank table 0x013BDA50[8].
// Model: local (engine/CStreamingFontBank.h is a flattened data header and cannot carry the CBase -> CFontBank -> CStreamingFontBank
//   vtable chain); member names from that header.
extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);
extern "C" void* __cdecl memset(void*, int, unsigned int);
#pragma intrinsic(memcpy, memset)
extern "C" void* __cdecl malloc(unsigned int);         // 0x00BFEA0E
extern "C" void __cdecl free(void*);                   // 0x00BFEA14
void* __cdecl operator new(unsigned int);              // 0x00BFEA1A
void* __cdecl operator new[](unsigned int);            // 0x00BFEB22
extern "C" int __cdecl lzo1x_decompress_safe(const unsigned char* src, unsigned long srcLen, unsigned char* dst, unsigned long* dstLen, void* wrk);   // 0x00C08170

struct CCharString {
    void* PStringData;
    CCharString();                                     // 0x0099E4B0
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
};
struct CBaseClassNonCopyable {
    virtual ~CBaseClassNonCopyable();
    CBaseClassNonCopyable();                           // 0x0099A2F0
};
struct CDataInputStream : CBaseClassNonCopyable {
    unsigned long StreamPos;                            // +0x04
    long StreamSize;                                    // +0x08
    const unsigned char* CurrentSourcePtr;              // +0x0c
    unsigned long SourceChunkStreamPos;                 // +0x10
    long SourceChunkBytesRemaining;                     // +0x14
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 p) { return p >= 0 && p <= 0x7fffffff; }
    __forceinline void Read(void* dst, long size) {
        __int64 np = (__int64)(StreamPos + size);
        if (!IsValidPosition(np)) return;
        if (SourceChunkBytesRemaining >= size) {
            memcpy(dst, CurrentSourcePtr, size); CurrentSourcePtr += size; SourceChunkBytesRemaining -= size; StreamPos += size;
        } else ReadWithSrcChunkOverflow((unsigned char*)dst, size);
    }
};
struct CMemoryDataInputStream : CDataInputStream {
    __forceinline void ReadBytes(void* dst, long size) {
        if (size <= 0) return;
        __int64 np = (__int64)(StreamPos + size);
        if (!IsValidPosition(np)) return;
        if (size <= SourceChunkBytesRemaining) {
            memcpy(dst, CurrentSourcePtr, size); CurrentSourcePtr += size; SourceChunkBytesRemaining -= size; StreamPos += size;
        } else ReadWithSrcChunkOverflow((unsigned char*)dst, size);
    }
    const unsigned char* m_pDataCopy;                   // +0x18
    CMemoryDataInputStream(const unsigned char* data, long size);   // 0x00994930
    ~CMemoryDataInputStream();                          // 0x00993B10
    CCharString ReadNullTerminatedString();             // 0x00995D30
};
struct CBankFile {
    unsigned long GetEntryDataSize(unsigned long entry);   // 0x009CC240
    void ReadEntryData(unsigned long entry, unsigned long offset, void* buffer, unsigned long size);   // 0x009CC2A0
};
struct CStreamingFontDataBank : CBankFile { };
struct StreamingFontCacheBufferData;
void __fastcall InitializeStreamingFontTextureCache(CStreamingFontDataBank* bank);   // 0x009A3640
void __fastcall InitializeStreamingFontCacheEntries(StreamingFontCacheBufferData* cache);   // 0x009A0F00
extern StreamingFontCacheBufferData g_StreamingFontCacheA;   // 0x013C5A98
extern StreamingFontCacheBufferData g_StreamingFontCacheB;   // 0x013C7AC8

struct CByteVector {                                    // STLport vector<unsigned char> over a malloc allocator
    unsigned char* First; unsigned char* Last; unsigned char* End;
    static unsigned char* Allocate(unsigned long n) { return n != 0 ? (unsigned char*)malloc(n) : 0; }
    static unsigned char* FillZero(unsigned char* p, unsigned long n) { memset(p, 0, n); return p + n; }
    explicit CByteVector(unsigned long n) : First(0), Last(0), End(0) {
        First = Allocate(n);
        End = First + n;
        Last = FillZero(First, n);
    }
    ~CByteVector() { if (First) free(First); }
    unsigned char* Data() { return Last - First ? First : (unsigned char*)this; }
};
struct GlyphBank {
    unsigned short FirstGlyph;
    unsigned short Count;
    unsigned long Glyphs[1];
};
struct CTexture;
struct CFontTextureHolder {
    CTexture* P0; CTexture* P1;
    CFontTextureHolder() : P0(0), P1(0) { }
};
struct CFontBank : CBaseClassNonCopyable {
    long m_maxHeight;                                   // +0x04
    CCharString Name;                                   // +0x08
    long _0c;                                           // +0x0c
    CFontBank() { }
    virtual long GetFontMaxHeight();
};
struct CStreamingFontBank;
extern CStreamingFontBank* g_StreamingFontBanks[8];     // 0x013BDA50
struct CStreamingFontBank : CFontBank {
    CStreamingFontDataBank* data_bank;                  // +0x10
    GlyphBank* glyphBankCache;                          // +0x14
    unsigned long glyphBankCacheIndex;                  // +0x18
    unsigned long MaxHeight;                            // +0x1c
    long Weight;                                        // +0x20
    bool Italics;                                       // +0x24
    void* GlyphBankMemory;                              // +0x28
    void* GlyphBanks[1024];                             // +0x2c
    unsigned long GlyphFlags[2048];                     // +0x102c
    unsigned long fontIndex;                            // +0x302c
    CFontTextureHolder* PTexture;                       // +0x3030
    CStreamingFontBank(CStreamingFontDataBank* bank, unsigned long entry);
    virtual long GetFontMaxHeight();
};

CStreamingFontBank::CStreamingFontBank(CStreamingFontDataBank* bank, unsigned long entry)
{
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
    InitializeStreamingFontCacheEntries(&g_StreamingFontCacheA);
    InitializeStreamingFontCacheEntries(&g_StreamingFontCacheB);
    memset(GlyphBanks, 0, sizeof(GlyphBanks));
    memset(GlyphFlags, 0, sizeof(GlyphFlags));
    unsigned long size = data_bank->GetEntryDataSize(entry);
    CByteVector buffer(size);
    data_bank->ReadEntryData(entry, 0, buffer.Data(), size);
    CMemoryDataInputStream stream(buffer.Data(), size);
    glyphBankCache = (GlyphBank*)operator new(0x104);
    glyphBankCacheIndex = 0xffffffff;
    Name = stream.ReadNullTerminatedString();
    {
    long value;
    stream.Read(&value, 4);
    m_maxHeight = value;
    stream.Read(&value, 4);
    Weight = value;
    char italics;
    stream.Read(&italics, 1);
    Italics = italics != 0;
    stream.Read(&value, 4);
    MaxHeight = value;
    }
    long offsets[0x401];
    stream.Read(offsets, sizeof(offsets));
    GlyphBankMemory = operator new[](offsets[0x400]);
    stream.ReadBytes(GlyphBankMemory, offsets[0x400]);
    for (unsigned long i = 0; i < 0x400; ++i) {
        unsigned long length = offsets[i + 1] - offsets[i];
        if (length == 0)
            continue;
        unsigned char* data = (unsigned char*)GlyphBankMemory + offsets[i];
        GlyphBanks[i] = data;
        GlyphBank* header = (GlyphBank*)(data + 2);
        if (*(unsigned short*)data & 0x8000) {
            unsigned long outLength = 0x104;
            lzo1x_decompress_safe(data + 2, length - 2, (unsigned char*)glyphBankCache, &outLength, 0);
            header = glyphBankCache;
            glyphBankCacheIndex = i;
        }
        for (unsigned long j = 0; j < header->Count; ++j) {
            if (header->Glyphs[j] & 0xffffff) {
                unsigned short glyph = (unsigned short)((i << 6) + j + header->FirstGlyph);
                GlyphFlags[glyph >> 5] |= 1 << (glyph & 31);
            }
        }
    }
    PTexture = new CFontTextureHolder;
}
