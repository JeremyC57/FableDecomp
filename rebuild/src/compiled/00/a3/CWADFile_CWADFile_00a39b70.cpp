// CWADFile::CWADFile(const CWADFile& other) -- copy constructor: default base, then bank, index and position copied
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A39B70, length 42 bytes (+ int3 padding).
// Leaf: ??0CWADFile@@QAE@ABV0@@Z
// Callee: CBase::CBase() @ 0x0099A2F0 (the DEFAULT base constructor -- an implicit copy constructor would copy the base inline).
// Shape: the vtable store comes right after the argument load and before the member copies (each copy through its own
// register); a user copy constructor with an initialiser list. Local polymorphic model (see 0x00A39B40).
class CBankFileAsync;

struct CBase {                              // the engine's root polymorphic base
    virtual ~CBase();
    CBase();                                // 0x0099A2F0
};

class CWADFile : public CBase { public:                   // vtable 0x0129CF8C (rtti-vtables.tsv); sizeof 0x10
    CBankFileAsync* Bank;                   // +0x04
    unsigned long   Index;                  // +0x08
    long            FilePos;                // +0x0c
    CWADFile(CBankFileAsync* bank);
    CWADFile(const CWADFile& other);
    virtual CWADFile* Clone();              // slot 14
};

CWADFile::CWADFile(const CWADFile& other) : Bank(other.Bank), Index(other.Index), FilePos(other.FilePos)
{
}