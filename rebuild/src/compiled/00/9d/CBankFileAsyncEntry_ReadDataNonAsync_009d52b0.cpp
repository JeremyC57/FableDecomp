// CBankFileAsyncEntry::ReadDataNonAsync(unsigned long offset, void* buffer, unsigned long size) -- blocking read of this entry's data through its bank file
// Leaf: ?ReadDataNonAsync@CBankFileAsyncEntry@@QAEXKPAXK@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x009D52B0, 30 bytes (`ret 0Ch`): BankFile (+4) as `this`, then CBankFile::ReadEntryData 0x009CC2A0 with
//   (Index (+8), offset, buffer, size).
// Identity (two sources): Ego_r.pdb (0x00B29320) and FableWin.pdb ?ReadDataNonAsync@CBankFileAsyncEntry@@QAEXKPAXK@Z; Ego_r donor order
//   ctor / ReadDataNonAsync / ... = retail 0x009D5260 (landed ??0CBankFileAsyncEntry@@AAE@AAVCBankFileAsync@@K@Z) / 0x009D52B0; the body
//   forwards to ?ReadEntryData@CBankFile@@QAEXKKPAXK@Z (Ego_r.pdb + FableWin.pdb) on the Ego_r members BankFile +4 / Index +8; its callers
//   CStreamingFontDataBank::ReadData 0x00A5EF30 / StreamGlyph 0x00A5F010 call it on streamingEntry (CCountedPointer<CBankFileAsyncEntry>),
//   CEngineWorldMap::LoadWaterData 0x00B41FA0 on its async entry. The catalog's "CEditTransactionBase::ActionSetEngineTheme" was propagated.
// NF79 (round 7, q10958): re-authored (was a free __fastcall stand-in ?CEditTransactionBase_ActionSetEngineTheme@@YIX..@Z calling a stand-in
//   Target::Apply). CBankFileAsyncEntry has no shared header: declared here with its Ego_r.pdb members (as the landed 0x00B41FA0 does);
//   CBankFile / CBankFileAsync are the shared headers (macro block of the landed InitStreamingEntry 0x00A5EF50) with the header's
//   CBANKFILE_READENTRYDATA opt-in (chunk Q26).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CBANKFILEASYNC_DATA
#define FABLE_RETAIL_CBANKFILE_ENTRIES
#define FABLE_RETAIL_CBANKFILE_READENTRYDATA
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CBankFile.h"
#include "engine/CBankFileAsync.h"

class CBankFileAsyncEntry {                       // no shared header (Ego_r.pdb: vptr, BankFile +4, Index +8, PaddedSize +0xC)
public:
    virtual ~CBankFileAsyncEntry();
    void ReadDataNonAsync(unsigned long offset, void* buffer, unsigned long size);   // 0x009D52B0
private:
    CBankFileAsync* BankFile;                     // +0x04
    unsigned long Index;                          // +0x08
    unsigned long PaddedSize;                     // +0x0C
};

void CBankFileAsyncEntry::ReadDataNonAsync(unsigned long offset, void* buffer, unsigned long size)
{
    BankFile->ReadEntryData(Index, offset, buffer, size);
}
