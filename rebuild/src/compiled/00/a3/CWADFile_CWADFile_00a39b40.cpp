// CWADFile::CWADFile(CBankFileAsync* bank) -- a WAD stream over a bank: keep the bank, entry index and position zero
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A39B40, length 35 bytes (+ int3 padding).
// Leaf: ??0CWADFile@@QAE@PAUCBankFileAsync@@@Z
// Callee: CBase::CBase @ 0x0099A2F0. Vtable 0x0129CF8C = CWADFile.
// Shape: base constructor, then the three initialisers (argument, xor-zeroed pair) and the vtable store LAST: plain
// initialiser-list stores are scheduled before the derived vtable store. Local polymorphic model: the engine header
// CWADFile.h flattens the vptr into a void* member, so the constructor needs a real virtual base (same members, same offsets).
struct CBankFileAsync;

struct CBase {                              // the engine's root polymorphic base
    virtual ~CBase();
    CBase();                                // 0x0099A2F0
};

struct CWADFile : CBase {                   // vtable 0x0129CF8C (rtti-vtables.tsv); sizeof 0x10
    CBankFileAsync* Bank;                   // +0x04
    unsigned long   Index;                  // +0x08
    long            FilePos;                // +0x0c
    CWADFile(CBankFileAsync* bank);
    CWADFile(const CWADFile& other);
    virtual CWADFile* Clone();              // slot 14
};

CWADFile::CWADFile(CBankFileAsync* bank) : Bank(bank), Index(0), FilePos(0)
{
}