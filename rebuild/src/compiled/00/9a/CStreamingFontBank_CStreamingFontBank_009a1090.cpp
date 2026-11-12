// CStreamingFontBank::~CStreamingFontBank() -- streaming font bank destructor: unregister the bank from its font slot, drop its entries from the global glyph cache, free the glyph-bank memory, the bank cache and the texture, then ~CFontBank (Name) and ~CBaseClassNonCopyable
// Leaf: ??1CStreamingFontBank@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A1090, 170 bytes, tail jump to ~CBaseClassNonCopyable 0x0099A300.
// Identity (proven): sole caller 0x009A4E40, slot 0 (scalar deleting dtor) of vtable 0x0129AB54, RTTI `.?AVCStreamingFontBank@@`
//   (CStreamingFontBank : CFontBank : CBaseClassNonCopyable : CBaseClass); the body stores that vtable first (user dtor) and,
//   after the body, CFontBank's vtable 0x0129AB38 (??_7CFontBank@@6B@ in the bindings) with CFontBank::Name's ~CCharString:
//   ~CFontBank is a user-written inline dtor. Ego_r.pdb / FableWin.pdb ??1CStreamingFontBank@@UAE@XZ, ??1CFontBank@@UAE@XZ.
//   The manifest label (CTextureScreen_dtor) is propagated and wrong.
// Members: Ego_r.pdb CStreamingFontBank (0x3034; retail uses the same offsets: glyphBankCache +0x14, GlyphBankMemory +0x28,
//   fontIndex +0x302c, PTexture +0x3030) over CFontBank {FontHeight +0x04, Name +0x08, PManager +0x0c}.
// Callees: operator delete[] 0x00BFEB1C (GlyphBankMemory), operator delete 0x00BFE9BC (glyphBankCache; PTexture after
//   CTexture's inline dtor = Uninitialise 0x009F9F70, ?Uninitialise@CTexture@@QAEXXZ in both PDBs), ~CCharString 0x0099EAE0,
//   ~CBaseClassNonCopyable 0x0099A300.
// Globals: 0x013BDA50 = streamingFontBankList (Anniversary publics, anonymous namespace; FAM13), the 8 font-slot bank pointers indexed by fontIndex,
//   0x013BDA70 = the 256 x 8 table of 16-byte glyph-cache entries whose first ushort carries the owning font slot in its low
//   4 bits (cleared together with the second ushort). Ego_r's StreamingFontCacheEntry has a different layout (newer build),
//   so the entry is modelled descriptively.
// Shape: the cache walk is one entry pointer advanced inside two counted loops (256, 8): an indexed [i][j] form is folded into
//   a single 0x8000-byte loop and keeps fontIndex in a register (probe 1); the pointer form reloads fontIndex after the
//   ushort stores (they may alias) as retail does. `entry->Glyph = 0` precedes `entry->FontIndex = 0` (retail store order).
//   `delete PTexture; PTexture = 0;` (the zero store sits after the null-tested delete).
// Layout: engine/CStreamingFontBank.h, CFontBank.h and CTexture.h are flattened Ego_r layouts (no vtable chain, no member
//   functions), so the three classes are local models with the headers' member names and offsets (NOTES.md "Header debt").
void __cdecl operator delete(void* block);       // 0x00BFE9BC
void __cdecl operator delete[](void* block);     // 0x00BFEB1C
#include "engine/CBaseClassNonCopyable.h"
class CCharStringData;
class CCharString { public:
    ~CCharString();                                                      // 0x0099EAE0
    CCharStringData* PStringData;
};
struct IDirect3DTexture9;
class CTexture { public:
    ~CTexture() { Uninitialise(); }
    void Uninitialise();                                                 // 0x009F9F70
    IDirect3DTexture9* PD3DTexture;                                      // +0x00
    unsigned long      ByteLength : 28;                                  // +0x04
    unsigned long      AllocationSource : 4;
};
class CFontManager;
class CFontBank : public CBaseClassNonCopyable {                         // vtable 0x0129AB38
public:
    virtual ~CFontBank() { }
    unsigned long FontHeight;                                            // +0x04
    CCharString   Name;                                                  // +0x08
    CFontManager* PManager;                                              // +0x0c
};
class CStreamingFontDataBank;
struct GlyphBank;
class CStreamingFontBank;
struct SStreamingGlyphCacheEntry_013bda70 {                                       // 16 bytes (descriptive)
    unsigned short FontIndex : 4;
    unsigned short Flags : 12;
    unsigned short Glyph;
    unsigned long  Data[3];
};
namespace { extern CStreamingFontBank* streamingFontBankList[8]; }   // 0x013BDA50 (FAM13) Anniversary publics ?streamingFontBankList@?A0x..@@3PAPAVCStreamingFontBank@@A
                                                                      // (the only CStreamingFontBank* array global, anonymous namespace of the font-bank TU)
extern SStreamingGlyphCacheEntry_013bda70 g_StreamingGlyphCache_013bda70[256][8];          // 0x013BDA70
class CStreamingFontBank : public CFontBank {                            // vtable 0x0129AB54
public:
    virtual ~CStreamingFontBank();
    CStreamingFontDataBank* data_bank;                                   // +0x10
    GlyphBank*              glyphBankCache;                              // +0x14
    unsigned long           glyphBankCacheIndex;                         // +0x18
    unsigned long           MaxHeight;                                   // +0x1c
    long                    Weight;                                      // +0x20
    bool                    Italics;                                     // +0x24
    void*                   GlyphBankMemory;                             // +0x28
    void*                   GlyphBanks[1024];                            // +0x2c
    unsigned long           GlyphFlags[2048];                            // +0x102c
    unsigned long           fontIndex;                                   // +0x302c
    CTexture*               PTexture;                                    // +0x3030
};
CStreamingFontBank::~CStreamingFontBank()
{
    if (streamingFontBankList[fontIndex] == this)
        streamingFontBankList[fontIndex] = 0;
    SStreamingGlyphCacheEntry_013bda70* entry = &g_StreamingGlyphCache_013bda70[0][0];
    for (long i = 0; i < 256; ++i) {
        for (long j = 0; j < 8; ++j, ++entry) {
            if (entry->FontIndex == fontIndex) {
                entry->Glyph = 0;
                entry->FontIndex = 0;
            }
        }
    }
    delete [] GlyphBankMemory;
    delete glyphBankCache;
    delete PTexture;
    PTexture = 0;
}