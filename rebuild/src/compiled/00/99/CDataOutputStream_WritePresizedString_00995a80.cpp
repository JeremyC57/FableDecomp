// CDataOutputStream::WritePresizedString(const CCharString& text) -- write the string's length as a 4-byte prefix, then its characters
// Leaf: ?WritePresizedString@CDataOutputStream@@QAEXABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00995A80, 86 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity (probable): Ego_r.pdb ?WritePresizedString@CDataOutputStream@@QAEXABVCCharString@@@Z -- the one CDataOutputStream
//   string writer that prefixes the length; it sits between WriteStringNewLine 0x00995A20 and the landed
//   WriteNullTerminatedString 0x00995AE0.
// Callees: virtual slot 4 Write(const void*, long) twice; "" (0x0129AAF4, the empty literal of the null data block).
// Shape: the text into a named local first (the WriteStringNewLine form), the length into an unsigned long local handed to the
//   inline WriteULONG (its by-value parameter is homed in the dead `text` slot and stored after the `push 4`), then the
//   characters with a re-read length (second null test, the call duplicated in its arms). A plain `long` local whose address
//   is taken (or an inline WriteSLONG) sinks the store into both null-test arms.
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
    void WriteULONG(unsigned long value) { Write(&value, sizeof(value)); }   // inline here (out-of-line copy 0x00993EA0)
};

void CDataOutputStream::WritePresizedString(const CCharString& text)
{
    const char* characters = text.GetAsCharArray();
    unsigned long length = text.GetLength();
    WriteULONG(length);
    Write(characters, text.GetLength());
}