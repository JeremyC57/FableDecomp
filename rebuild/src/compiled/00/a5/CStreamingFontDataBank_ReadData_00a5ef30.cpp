// CStreamingFontDataBank::ReadData(void* buffer, unsigned long offset, unsigned long size) -- blocking read of glyph data through the streaming entry
// Leaf: ?ReadData@CStreamingFontDataBank@@QAEXPAXKK@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A5EF30, 29 bytes (`ret 0Ch`): streamingEntry's Data (+0x188) as `this`, the arguments pushed as
//   (offset, buffer, size), then CBankFileAsyncEntry::ReadDataNonAsync 0x009D52B0.
// Identity (two sources): Ego_r.pdb + FableWin.pdb ?ReadData@CStreamingFontDataBank@@QAEXPAXKK@Z, in Ego_r donor order before InitStreamingEntry (landed
//   0x00A5EF50) and StreamGlyph (0x00A5F010) as retail places it; the shared engine/CStreamingFontDataBank.h declares it for this
//   address, and the landed caller `anonymous namespace'::InitializeStreamingFontTextureCache 0x009A3640 already binds this spelling at
//   0x00A5EF30 (three calls). Its callee is ReadDataNonAsync (Ego_r.pdb + FableWin.pdb), and the argument order (buffer, offset, size)
//   matches the Ego_r signature. The catalog's "std::vector<CCharString>::_Ucopy<...>" was a propagated label.
// NF79 (round 7, q10958): re-authored on the shared header (FABLE_RETAIL_CSTREAMINGFONTDATABANK, flattened streamingEntry_Data);
//   CBankFileAsyncEntry has no shared header (local declaration of the one member called). Was an Outer / Inner stand-in
//   (?Ucopy_forward@Outer@@ binding ?Ucopy@Inner@@ at 0x009D52B0).
#define FABLE_RETAIL_CSTREAMINGFONTDATABANK
#include "engine/CCountedPointer.h"
#include "engine/CStreamingFontDataBank.h"
class CBankFileAsyncEntry { public:                // no shared header (Ego_r.pdb: vptr, BankFile +4, Index +8, PaddedSize +0xC)
    void ReadDataNonAsync(unsigned long offset, void* buffer, unsigned long size);   // 0x009D52B0
};

void CStreamingFontDataBank::ReadData(void* buffer, unsigned long offset, unsigned long size)
{
    streamingEntry_Data->ReadDataNonAsync(offset, buffer, size);
}
