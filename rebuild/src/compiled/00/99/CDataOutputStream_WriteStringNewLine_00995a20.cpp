// CDataOutputStream::WriteStringNewLine(const CCharString& text) -- write the string's characters, then the stream's NewLine string
// Leaf: ?WriteStringNewLine@CDataOutputStream@@QAEXABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00995A20, 88 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity (probable): Ego_r.pdb ?WriteStringNewLine@CDataOutputStream@@QAEXABVCCharString@@@Z, the one CDataOutputStream string
//   writer that appends the NewLine member (+0x04); it sits beside WritePresizedString 0x00995A80 and the landed
//   WriteNullTerminatedString 0x00995AE0.
// Callees: virtual slot 4 Write(const void*, long) twice; "" (0x0129AAF4, the empty literal of the null data block).
// Shape: each string's text is read into a named local before the call that also takes its length (both inline accessors):
//   for the argument VC7.1 merges the two null tests into one and moves the null arm (`xor eax,eax`) out of line; for the
//   member NewLine it tests twice and duplicates the call in the length arms. Without the named locals the length is
//   computed first (right-to-left) and the blocks fall in another order (the 2026-10-14 "null arm out of line" park).
// ---- CDataOutputStream model: vptr +0, NewLine (CCharString) +4 -- engine/CDataOutputStream.h is the flat layout (vptr as data) ----
// Vtable (RTTI .?AVCMemoryDataOutputStream@@ 0x...): slot 0 dtor, 1 SetPosition, 2 GetPosition, 3 Close, 4 Write(const void*, long).
template <class T> class CBasicString {
public:
    T*            PData;                                // +0x00
    unsigned long StringLength;                         // +0x04
    unsigned long DataLength : 31;                      // +0x08
    unsigned long UseFastExtend : 1;
};
class CCharStringData {
public:
    CBasicString<char> Data;                            // +0x00
    long               NoRefs;
};
class CCharString {
public:
    CCharStringData* PStringData;                       // +0x00
    const char* GetAsCharArray() const { return PStringData == 0 ? "" : PStringData->Data.PData; }
    long GetLength() const { return PStringData == 0 ? 0 : PStringData->Data.StringLength; }
};

class CDataOutputStream {
public:
    virtual ~CDataOutputStream();                       // slot 0
    virtual void SetPosition(unsigned long position) = 0;   // slot 1
    virtual unsigned long GetPosition() const = 0;      // slot 2
    virtual void Close() = 0;                           // slot 3
    virtual void Write(const void* data, long length) = 0;  // slot 4
    CCharString NewLine;                                // +0x04
    void WriteStringNewLine(const CCharString& text);
    void WritePresizedString(const CCharString& text);
};

void CDataOutputStream::WriteStringNewLine(const CCharString& text)
{
    const char* characters = text.GetAsCharArray();
    Write(characters, text.GetLength());
    const char* newLine = NewLine.GetAsCharArray();
    Write(newLine, NewLine.GetLength());
}