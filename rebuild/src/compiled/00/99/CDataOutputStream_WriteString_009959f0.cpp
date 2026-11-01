// CDataOutputStream::WriteString(const CCharString&) -- write the string's characters (no length prefix, no terminator) through the virtual Write
// Leaf: ?WriteString@CDataOutputStream@@QAEXABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009959F0, 46 bytes, thiscall, `ret 4` (two tails).
// Identity (proven): Ego_r.pdb ?WriteString@CDataOutputStream@@QAEXABVCCharString@@@Z; the Ego_r donor order WriteString, WriteStringNewLine,
//   WritePresizedString, WriteNullTerminatedString equals the retail order 0x009959F0, 0x00995A20, 0x00995A80, 0x00995AE0, and
//   WriteStringNewLine 0x00995A20 opens with this same body (then writes the NewLine member); 12 callers.
// Callees: CDataOutputStream virtual slot 4 Write(const void*, long) (+0x10; pure here, CMemoryDataOutputStream::Write 0x00996CC0 in
//   vtable 0x0129A788 slot 4 = Ego_r ?Write@CMemoryDataOutputStream@@UAEXPBXJ@Z). Data: the empty string literal 0x0129AAF4.
// Layout: engine/CDataOutputStream.h (vptr, NewLine +0x04); CCharString (Ego_r.pdb): PStringData -> CCharStringData { CBasicString<char>
//   Data { PData +0, StringLength +4, .. }, NoRefs }. The header flattens the vptr, so the class is a local polymorphic model.
// Virtual slots (from the CMemoryDataOutputStream overrides, vtable 0x0129A788): 0 dtor, 1 SetPosition 0x00994B10, 2 GetPosition const
//   0x009945B0, 3 Close 0x009945C0 (CDataStream, pure), 4 Write (CDataOutputStream, pure).
// Shape: the inline PDB accessors GetAsCharArray() / GetLength() (each `PStringData == 0 ? .. : ..`); VC7.1 threads both tests into one.
//   The data pointer is a named local read before the call: written inline as the first argument it is evaluated after the length
//   (right-to-left), which puts the length test first and splits the tails differently (52v46).
template <class T> class CBasicString {
public:
    T* PData;                                              // +0x00
    unsigned long StringLength;                            // +0x04
    unsigned long DataLength : 31;                         // +0x08
    bool UseFastExtend : 1;
};
#include "engine/CBaseClassNonCopyable.h"
class CCharStringData {
public:
    CBasicString<char> Data;                               // +0x00
    long NoRefs;
};
class CCharString {
public:
    const char* GetAsCharArray() const { return PStringData == 0 ? "" : PStringData->Data.PData; }
    long GetLength() const { return PStringData == 0 ? 0 : PStringData->Data.StringLength; }
private:
    CCharStringData* PStringData;                          // +0x00
};
class CDataStream : public CBaseClassNonCopyable {
public:
    virtual void SetPosition(unsigned long position) = 0;  // slot 1
    virtual unsigned long GetPosition() const = 0;         // slot 2
    virtual void Close() = 0;                              // slot 3
};
class CDataOutputStream : public CDataStream {
public:
    virtual void Write(const void* data, long length) = 0; // slot 4 (+0x10)
    void WriteString(const CCharString& text);
    CCharString NewLine;                                   // +0x04
};

void CDataOutputStream::WriteString(const CCharString& text)
{
    const char* data = text.GetAsCharArray();
    Write(data, text.GetLength());
}