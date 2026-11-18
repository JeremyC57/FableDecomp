// CBankFileAsyncEntry::CBankFileAsyncEntry(CBankFileAsync*, unsigned long) -- bind to one bank entry, cache its aligned size
// Leaf: ??0CBankFileAsyncEntry@@AAE@AAVCBankFileAsync@@K@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D5260, oracle length 52 bytes.
// Callees: CBase::CBase @ 0x0099A2F0 (the root with vtable 0x0129A7C4), CBankFile::GetEntryDataSize @ 0x009CC240,
//          CBankFile::Align @ 0x009CBB90.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CBase::CBase ; mov eax,[esp+0Ch] ; mov ecx,[esp+8] ; push eax ; mov dword ptr [esi],offset CBankFileAsyncEntry::`vftable' ;
//                mov [esi+4],ecx ; mov [esi+8],eax ; call GetEntryDataSize ; mov ecx,[esi+4] ; push eax ; call Align ; mov [esi+0Ch],eax ; mov eax,esi ; pop esi ; ret 8
// The out-of-line root constructor runs first, then this class's vtable (0x0129B880 per
// rtti-vtables.tsv), the bank pointer and the entry index; the body computes
// `m_pBank->Align(m_pBank->GetEntryDataSize(index))`: the inner call's `this` is the just-stored
// bank still in ecx, the outer re-reads the member after the call.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CBANKFILE_ENTRYDATASIZE
#define FABLE_RETAIL_CBANKFILE_ALIGN
#include "fable_string.h"
#include "engine/CBankFile.h"
#include "engine/CBankFileAsync.h"

// NF75 (round 7, land1117fu q10932): Ego_r.pdb ??0CBankFileAsyncEntry@@AAE@AAVCBankFileAsync@@K@Z (private, the bank by reference) and the
//   Ego_r layout (BankFile +4, Index +8, PaddedSize +0xC; ??1CBankFileAsyncEntry@@UAE@XZ virtual); no engine/ header exists for the class.
//   Its base is the shared engine/CBaseClassNonCopyable.h (ctor 0x0099A2F0; was a four-slot CBase stand-in).
#include "engine/CBaseClassNonCopyable.h"
class CBankFileAsyncEntry : public CBaseClassNonCopyable {
public:
    virtual ~CBankFileAsyncEntry();
private:
    CBankFileAsyncEntry(CBankFileAsync& bank_file, unsigned long index);   // 0x009D5260
    CBankFileAsync* BankFile;        // +0x04
    unsigned long   Index;           // +0x08
    unsigned long   PaddedSize;      // +0x0c
};

CBankFileAsyncEntry::CBankFileAsyncEntry(CBankFileAsync& bank_file, unsigned long index)
    : BankFile(&bank_file), Index(index)
{
    PaddedSize = BankFile->Align(BankFile->GetEntryDataSize(index));
}
