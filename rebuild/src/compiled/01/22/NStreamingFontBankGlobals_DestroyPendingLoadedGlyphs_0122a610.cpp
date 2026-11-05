// NStreamingFontBankGlobals::DestroyPendingLoadedGlyphs() -- the `dynamic atexit destructor for 'g_PendingLoadedGlyphs'': destroy the 512 pending-glyph records last to first, releasing each record's counted glyph pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0122A610, 71 bytes (row padded to 80) in `.text$yd`; registered with atexit by
// the array's dynamic initializer @ 0x01223FC0 (which zeroes each record's counted pointer, then `push 0x0122A610 ; call atexit`).
// Callees: the count block's static `__fastcall` deleter (`mov ecx,[block+8] ; call [block+4]`), operator delete @ 0x00BFE9BC.
// Layout: g_PendingLoadedGlyphs @ 0x013C82F8, 512 records of 16 bytes: CCountedPointer {Data +0, Info +4} then the record's
// plain members (a bool at +8 is read by CStreamingFontBank::Preload 0x009A2000); the count block {count, deleter, object}.
// Shape: the compiler-generated array destructor is an inline counted loop over the records from the end (`esi` stepping
// back 16 bytes, `ebx` = 512); the counted pointer's inline destructor re-reads Info after the decrement (`--Info->Count;
// if (Info->Count == 0)`), calls the deleter on the object, frees the block, then clears Data and Info.
// Leaf: _$E2
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local SCountInfo (= CCPPointerInfo) members Count->RefCount, DeleteData->DeleteFunc, Object->Data are the header CCPPointerInfo's (same offsets).
#include "engine/CCountedPointer.h"


class CGlyphBlock;

namespace NStreamingFontBankGlobals {
struct SPendingLoadedGlyph {                       // 16 bytes
    CCountedPointer<CGlyphBlock> Block;            // +0x00
    bool          Loaded;                          // +0x08
    unsigned long GlyphIndex;                      // +0x0c
};

SPendingLoadedGlyph g_PendingLoadedGlyphs[512];    // 0x013C82F8
}