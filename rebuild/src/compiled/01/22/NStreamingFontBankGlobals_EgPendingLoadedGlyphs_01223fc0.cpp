// NStreamingFontBankGlobals::$E_g_PendingLoadedGlyphs() -- dynamic initializer of the 512 pending-glyph records: clear each record's counted glyph pointer, then register the array destructor with atexit
// Leaf: _$E1
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local SCountInfo (= CCPPointerInfo) members Count->RefCount, DeleteData->DeleteFunc, Object->Data are the header CCPPointerInfo's (same offsets).
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

#include "engine/CCountedPointer.h"


namespace NStreamingFontBankGlobals {
struct SPendingLoadedGlyph {                       // 16 bytes
    CCountedPointer<CGlyphBlock> Block;            // +0x00
    bool          Loaded;                          // +0x08
    unsigned long GlyphIndex;                      // +0x0c
};

SPendingLoadedGlyph g_PendingLoadedGlyphs[512];    // 0x013C82F8
}