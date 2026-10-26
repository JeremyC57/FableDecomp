// CWADFile::Clone() -- allocate a copy of this WAD stream (0 when the allocation fails)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A39C80, length 61 bytes (+ int3 padding). CWADFile vtable 0x0129CF8C slot 14.
// Leaf: Clone
// Callees: operator new @ 0x00BFEA1A (0x10 bytes), CBase::CBase @ 0x0099A2F0.
// Shape: `new CWADFile(*this)` with the copy constructor (0x00A39B70) inlined: null test on the allocation, base
// constructor, vtable, three member copies, and the separate `xor eax,eax` failure exit. Local polymorphic model (see 0x00A39B40).
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

inline CWADFile::CWADFile(const CWADFile& other) : Bank(other.Bank), Index(other.Index), FilePos(other.FilePos)
{
}

CWADFile* CWADFile::Clone()
{
    return new CWADFile(*this);
}