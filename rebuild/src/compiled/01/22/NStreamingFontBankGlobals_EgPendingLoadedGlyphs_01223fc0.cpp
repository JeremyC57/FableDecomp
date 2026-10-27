// NStreamingFontBankGlobals::$E_g_PendingLoadedGlyphs() -- dynamic initializer of the 512 pending-glyph records: clear each record's counted glyph pointer, then register the array destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01223FC0, 39 bytes, in `.text$yc`, reached from the .CRT$XCU table, plain `ret`.
// Identity (proven): Ego_r.pdb / FableWin.pdb `NStreamingFontBankGlobals::dynamic initializer for 'g_PendingLoadedGlyphs'`;
//   it registers the landed atexit destructor 0x0122A610 (NStreamingFontBankGlobals::DestroyPendingLoadedGlyphs, the
//   `dynamic atexit destructor for 'g_PendingLoadedGlyphs'`) over the same 512 x 16-byte array at 0x013C82F8.
// Label: the manifest's "sub_01223fc0" is a gap-scan placeholder.
// Callee: _atexit 0x004012BC.
// Layout: as the landed 0x0122A610: 512 records of 16 bytes, CCountedPointer {Data +0, Info +4}, bool Loaded +8, GlyphIndex +0xc.
// Shape: the compiler-generated array constructor is an inline counted loop (pointer in eax, 512 in ecx, zero in edx)
//   storing only the counted pointer's two words; the plain members stay uninitialised.
class CGlyphBlock;

struct SCountInfo {
    long  Count;                                   // +0x00
    void (__fastcall* DeleteData)(void* object);   // +0x04
    void* Object;                                  // +0x08
};

template <class T> class CCountedPointer {
public:
    T*          Data;                              // +0x00
    SCountInfo* Info;                              // +0x04
    CCountedPointer() : Data(0), Info(0) {}
    ~CCountedPointer() { ReleaseData(); }
    void ReleaseData()
    {
        if (Info) {
            --Info->Count;
            if (Info->Count == 0) {
                Info->DeleteData(Info->Object);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};

namespace NStreamingFontBankGlobals {
struct SPendingLoadedGlyph {                       // 16 bytes
    CCountedPointer<CGlyphBlock> Block;            // +0x00
    bool          Loaded;                          // +0x08
    unsigned long GlyphIndex;                      // +0x0c
};

SPendingLoadedGlyph g_PendingLoadedGlyphs[512];    // 0x013C82F8
}