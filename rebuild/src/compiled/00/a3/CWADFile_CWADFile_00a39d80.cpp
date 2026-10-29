// CWADFile::CWADFile(CBankFileAsync* bank, const CWideString& name, FileAccessType access, unsigned long flags) -- a WAD stream over a bank, opened on the named entry at construction
// Leaf: ??0CWADFile@@QAE@PAVCBankFileAsync@@ABVCWideString@@W4FileAccessType@@K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A39D80, 57 bytes (+7 int3 padding), thiscall, `ret 10h`.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0, CWADFile::Open(const CWideString&, FileAccessType, unsigned long) @ 0x00A39CC0
//   (landed; the vtable 0x0129CF8C slot 1 reaches the same body through the `jmp` at 0x00A39DC0).
// Layout: engine/CWADFile.h (Bank +4, Index +8, FilePos +0xc) flattens the vptr into a void* member, so the constructor
//   needs a real polymorphic base: the same local model as the landed CWADFile::CWADFile(CBankFileAsync*) 0x00A39B40.
// Shape: the one-argument constructor's initialisers (Bank, the xor-zeroed Index/FilePos) are scheduled before the
//   derived vtable store, which lands right before the direct (constructor-time) Open call.
class CBankFileAsync;
class CWideString;
enum FileAccessType { FILE_ACCESS_READ };

class CBaseClassNonCopyable {
protected:                                // the engine's root polymorphic base
    virtual ~CBaseClassNonCopyable();
    CBaseClassNonCopyable();                                // 0x0099A2F0
};

class CWADFile : public CBaseClassNonCopyable {             // vtable 0x0129CF8C (rtti-vtables.tsv); sizeof 0x10
public:
    CBankFileAsync* Bank;                   // +0x04
    unsigned long   Index;                  // +0x08
    long            FilePos;                // +0x0c
    CWADFile(CBankFileAsync* bank, const CWideString& name, FileAccessType access, unsigned long flags);
    void Open(const CWideString& name, FileAccessType access, unsigned long flags);   // 0x00A39CC0
    virtual CWADFile* Clone();              // slot 14
};

CWADFile::CWADFile(CBankFileAsync* bank, const CWideString& name, FileAccessType access, unsigned long flags)
    : Bank(bank), Index(0), FilePos(0)
{
    Open(name, access, flags);
}