// CBankCreatorException::CBankCreatorException(const CCharString& entry_symbol, const CCharString& error_text) -- build the bank-creator exception: copy the entry symbol and the error text
// Leaf: ??0CBankCreatorException@@QAE@ABVCCharString@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009CB8F0, 32 bytes, __fastcall `ret 8`, returns `this`.
// Identity (label correction): the catalog's "pair<CCharString,CTCBase::CRegisteredTCInfo>" is propagated. The only caller,
//   0x009CF33B (the bank-file writer, catalog CBankFile_WriteSymbolFile region), builds the object in a frame slot and
//   passes it to _CxxThrowException (0x00BFEB84) with ThrowInfo 0x013693B4, whose catchable type is
//   `.?AVCBankCreatorException@@` (size 8, copy constructor 0x009D47E0). FableWin.pdb:
//   ??0CBankCreatorException@@QAE@ABVCCharString@@0@Z; Ego_r.pdb layout: EntrySymbol +0, ErrorText +4 (CCharString).
// Callee: CCharString::CCharString(const CCharString&) @ 0x0099EC30 (twice).
// Shape: two member initialisers from the two references (byte-identical to the pair<CCharString,CCharString> value
//   constructor 0x0076DD40).
class CCharString {
public:
    void* PData;                                        // +0x00
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};

class CBankCreatorException {
public:
    CCharString EntrySymbol;                            // +0x00
    CCharString ErrorText;                              // +0x04
    CBankCreatorException(const CCharString& entry_symbol, const CCharString& error_text);
};

CBankCreatorException::CBankCreatorException(const CCharString& entry_symbol, const CCharString& error_text)
    : EntrySymbol(entry_symbol), ErrorText(error_text)
{
}