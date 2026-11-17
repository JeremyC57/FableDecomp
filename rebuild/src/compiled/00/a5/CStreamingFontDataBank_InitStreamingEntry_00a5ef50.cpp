// CStreamingFontDataBank::InitStreamingEntry(CSymbolMap& symbols) -- look up the "GlyphData" entry of the font bank and keep its asynchronous bank entry for streaming the glyphs
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A5EF50, 178 bytes (+ int3 padding), `ret 4` (PDB
//   `?InitStreamingEntry@CStreamingFontDataBank@@QAEXAAVCSymbolMap@@@Z`; the symbol map is unused in the release build).
// Leaf: InitStreamingEntry
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("GlyphData" @ 0x0129D5A4), ~CCharString @ 0x0099EAE0,
//   CBankFile::FindIndexBySymbol(const CCharString&) const @ 0x009CCDC0 (landed), CBankFileAsync::GetAsyncEntry(unsigned long)
//   @ 0x009D58D0 (PDB name; CCountedPointer<CBankFileAsyncEntry> by hidden pointer; landed as "CreateEntry"),
//   operator delete @ 0x00BFE9BC (count blocks).
// Shape: the "GlyphData" conversion temporary dies right after the lookup, so the hidden result of GetAsyncEntry reuses its
//   frame word. The counted-pointer assignment is inline: both source words are read into locals first (ebx/edi), the
//   member block is compared with the new one, the old block released (the member re-read after the decrement), the
//   words stored and the new block add-referenced; then the temporary's destructor releases its own reference (its
//   address escaped into the call, so the block is re-read after the decrement).
// Retail layout: the CBankFileAsync base is 0x164 bytes (engine/CStreamingFontDataBank.h carries the PDB +0x1D8..), so
//   streamingEntry sits at +0x188 (Data) / +0x18C (Info), 0x74 below the header.
// NF72 (round 7, q10881): GetAsyncEntry is CBankFileAsync's (Ego_r.pdb 0x00B29950 / FableWin.pdb 0x02FF69A0
//   ?GetAsyncEntry@CBankFileAsync@@QAE?AV?$CCountedPointer@VCBankFileAsyncEntry@@@@K@Z), not the font bank's: the local CCharString / CBankFile /
//   CBankFileAsync models are replaced by fable_string.h and engine/CBankFile.h / CBankFileAsync.h (opt-ins STATICBLOCKS_TEXTURE_HELPERS,
//   CBANKFILEASYNC_DATA, CBANKFILEASYNC_GETASYNCENTRY, CBANKFILE_ENTRIES for FindIndexBySymbol). CStreamingFontDataBank keeps the row's
//   earlier local model (engine/CStreamingFontDataBank.h is the flattened layout without the retail base), now over the shared CBankFileAsync.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CBANKFILEASYNC_DATA
#define FABLE_RETAIL_CBANKFILEASYNC_GETASYNCENTRY
#define FABLE_RETAIL_CBANKFILE_ENTRIES
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CBankFile.h"
#include "engine/CBankFileAsync.h"

class CSymbolMap;
class CBankFileAsyncEntry;

class CStreamingFontDataBank : public CBankFileAsync {
public:
    unsigned char                        _pad_0x164[0x24]; // StateBlock +0x164, Fonts +0x178, Filename +0x184
    CCountedPointer<CBankFileAsyncEntry> streamingEntry;   // +0x188
    void InitStreamingEntry(CSymbolMap& symbols);
};

void CStreamingFontDataBank::InitStreamingEntry(CSymbolMap& /*symbols*/)
{
    unsigned long index = FindIndexBySymbol("GlyphData");
    streamingEntry = GetAsyncEntry(index);
}