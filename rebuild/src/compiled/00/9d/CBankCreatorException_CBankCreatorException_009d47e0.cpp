// CBankCreatorException::CBankCreatorException(const CBankCreatorException&) -- implicit copy constructor (the exception's throw copy)
// Retail: Fable.exe 0x009D47E0, 33 bytes: copies EntrySymbol (+0) and ErrorText (+4) with the CCharString copy
// constructor 0x0099EC30, returns `this`, `ret 4`; no EH frame.
// Identity: its only reference is the CatchableType at 0x01369390 (copyFunction field), whose TypeDescriptor 0x013963A0
// names .?AVCBankCreatorException@@ (size 8); the matching unwind function 0x009D47C0 destroys +4 then +0. The bank-file
// writer throws the exception (landed ctor 0x009CB8F0 notes ThrowInfo 0x013693B4). Ego_r.pdb / FableWin.pdb:
// ??0CBankCreatorException@@QAE@ABV0@@Z; layout EntrySymbol +0, ErrorText +4.
// Shape: the copy constructor is compiler-generated; it is emitted here the way retail emitted it, from a `throw` of the
// class (the TU builds without /EHsc, so the memberwise copy has no unwind state).
// Leaf: ??0CBankCreatorException@@QAE@ABV0@@Z
class CCharString {
public:
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    void* PStringData;
};

class CBankCreatorException {
public:
    CCharString EntrySymbol;                            // +0x00
    CCharString ErrorText;                              // +0x04
};

void ThrowBankCreatorException_009d47e0(const CBankCreatorException& exception)
{
    throw exception;
}