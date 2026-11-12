// CBankFile::AddEntryMultipleFiles(symbol, type, filenames, stateBlock) -- add or refresh a bank entry built from several source
//   files. A symbol already in OldSymbolIndices must not exist yet in this update pass (else throws CBankCreatorException
//   "Duplicate symbol: <symbol>"); it is marked existing and flagged for update when its type, filenames or state-block CRC
//   changed, then InitEntry(index, symbol, first filename or "") and return. A new symbol takes the first index >= 1 whose entry
//   does not exist (inline EntryExists), growing RuntimeData / UpdateData / Symbols / Checksums by one when none is free, gets a
//   fresh update record, its symbol, type, filenames and CRC, and the same InitEntry tail.
// Leaf: ?AddEntryMultipleFiles@CBankFile@@UAEXABVCCharString@@KABV?$CArray@VCCharString@@@@V?$CCountedPointer@VCBankStateBlock@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009CE5E0, 741 bytes, thiscall ret 0x14 (public virtual, CBankFile vtable 0x0129B54C slot 6;
//   Ego_r 0x00B1C000 / FableWin 0x017A3523). Default flags (no /EHsc: no SEH frame, the throw temporary is never destroyed).
// Callees: OldSymbolIndices _M_find 0x009D2DD0 (node in eax), operator+(const char*, const CCharString&) 0x0099F690 (__fastcall),
//   CCharString() 0x0099E4B0 / (const CCharString&) 0x0099EC30 / (const char*, long) 0x0099EBF0 / ~ 0x0099EAE0 / operator=
//   0x0099EFB0, _CxxThrowException 0x00BFEB84 (ThrowInfo 0x013693B4 CBankCreatorException), CBankFileEntryUpdateData::Equals
//   0x009CC750 / SetFilenames 0x009CC840, CBankStateBlock::CreateCRC (vtable slot 2), CPackedUIntArray::operator[] 0x00A629F0,
//   vector<CBankFile::CRuntimeData>::resize(n, x) 0x009D4010, vector<CBankFileEntryUpdateData*>::resize(n, x) 0x009D3DF0 (FableWin),
//   vector<CCharString>::resize(n) 0x0049B760, vector<unsigned long>::resize(n, x) 0x00464931 (Ego_r), operator new 0x00BFEA1A,
//   CBankFile::InitEntry 0x009CE050, operator delete 0x00BFE9BC (the counted pointer's info block).
// Shape: the hit arm returns early -- its filename is a block local and the miss path's filename a function-scope one, so they
//   take different frame slots and VC7.1 cross-jumps only the ~CCharString call (two InitEntry calls stay). EntryExists 0x009CC190
//   and the CBankFileEntryUpdateData ctor 0x009CC6A0 are expanded inline (same TU, bodies visible); STLport resize(n, x) bodies are
//   visible but called out of line (FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE), vector<CCharString>::resize(n) is declared only.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_TREE_MFIND_DECL
#define FABLE_RETAIL_CBANKFILE_FILENAMEINDICES
#define FABLE_RETAIL_CBANKFILE_ADDENTRYMULTIPLEFILES
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CBANKFILEENTRYUPDATEDATA_RETAIL
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/CArray.h"
// vector<CCharString>::resize(n) is called out of line (0x0049B760).
template<> void std::vector<CCharString, std::allocator<CCharString> >::resize(unsigned int n);
#include "engine/CPackedUIntArray.h"
#include "engine/CCountedPointer.h"
#include "engine/CBankFileEntryUpdateData.h"
#include "engine/CBankFile.h"

class CBankStateBlock {                            // no shared header; vtable 0x0129C82C (FableWin.pdb names, CWADBankStateBlock slots)
public:
    virtual ~CBankStateBlock();                                  // slot 0
    virtual void GetDefault(CBankStateBlock& block);             // slot 1
    virtual unsigned long CreateCRC();                           // slot 2 (+0x08)
};
class CBankCreatorException {                      // 8 bytes (Ego_r.pdb); no shared header
public:
    CCharString EntrySymbol;                       // +0x00
    CCharString ErrorText;                         // +0x04
    // ??0CBankCreatorException@@QAE@ABVCCharString@@@Z (FableWin.pdb 0x01817946). Retail has no out-of-line copy and no landed row
    // to take the body from: this row expands it in place at 0x009CE62B..0x009CE63E (CCharString default ctor 0x0099E4B0 on +0x00,
    // copy ctor 0x0099EC30 on +0x04), so the inline body is visible to this TU.
    CBankCreatorException(const CCharString& errorText) : ErrorText(errorText) { }
    CBankCreatorException(const CBankCreatorException& other);   // 0x009D47E0
    ~CBankCreatorException();                                    // 0x009D47C0 (ThrowInfo 0x013693B4 pmfnUnwind)
};

void CBankFile::AddEntryMultipleFiles(const CCharString& symbol, unsigned long type, const CArray<CCharString>& filenames, const CCountedPointer<CBankStateBlock> stateBlock)
{
    unsigned long index;
    std::map<CCharString, unsigned long>::iterator it = OldSymbolIndices.find(symbol);
    if (it != OldSymbolIndices.end()) {
        index = (*it).second;
        CBankFileEntryUpdateData* update = UpdateData[index];
        if (update->Exists)
            throw CBankCreatorException("Duplicate symbol: " + symbol);
        update->Exists = true;
        CRuntimeData& data = RuntimeData[index];
        if (data.DataType != type || !update->Equals(filenames, stateBlock)) {
            data.DataType = (unsigned char)type;
            update->SetFilenames(filenames);
            update->StateBlockCRC = stateBlock->CreateCRC();
            update->RequiresUpdate = true;
        }
        CCharString filename(filenames.empty() ? CCharString("") : filenames[0]);
        InitEntry(index, symbol, filename);
        return;
    }
    for (index = 1; index < Size; ++index)
        if (!EntryExists(index))
            break;
    if (index >= Size) {
        CRuntimeData blank;
        blank.DataOffset = 0;
        blank.DataSize = 0;
        blank.DataType = 0;
        blank.Valid = false;
        RuntimeData.resize(index + 1, blank);
        UpdateData.resize(index + 1, 0);
        Symbols.resize(index + 1);
        Checksums.resize(index + 1, 0);
        Size = RuntimeData.size();
    }
    CRuntimeData& data = RuntimeData[index];
    CBankFileEntryUpdateData* update = new CBankFileEntryUpdateData;
    UpdateData[index] = update;
    Symbols[index] = symbol;
    data.Valid = true;
    data.DataType = (unsigned char)type;
    update->SetFilenames(filenames);
    update->StateBlockCRC = stateBlock->CreateCRC();
    update->Exists = true;
    update->RequiresUpdate = true;
    CCharString filename(filenames.empty() ? CCharString("") : filenames[0]);
    InitEntry(index, symbol, filename);
}