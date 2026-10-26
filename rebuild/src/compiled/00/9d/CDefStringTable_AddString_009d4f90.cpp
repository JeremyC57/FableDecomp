// CDefStringTable::AddString(const CCharString& text) -- return the stream offset of a string in the table, appending it (CRC word + NUL-terminated text) and recording it in the map when new; empty strings give -1
// Leaf: AddString
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D4F90, length 204 bytes (+ int3 padding), `ret 4`.
// Callees: CDefStringTable::Unlock @ 0x009D4F10, the STLport map<CCharString,long> find (node result) @ 0x0042B467,
// CMemoryDataOutputStream size getter @ 0x00994B40 (`last - first`, catalog "CCreatureActionBase::GetFramesRemaining", a
// shared COMDAT body), CDataOutputStream::WriteRepeatedByte(uchar, long) @ 0x00994230, the CRC routine
// Calc(ulong seed, const void*, ulong length) @ 0x004014A0 (__fastcall), virtual slot 4 Write on the stream,
// CDataOutputStream::WriteNullTerminatedString(const CCharString&) @ 0x00995AE0, map operator[] @ 0x00519230,
// CMemoryDataOutputStream::PeekData @ 0x00995E70; global 0x013CA824 (the table data pointer past the leading word);
// the empty string literal @ 0x0129AAF4.
// Layout: retail CDefStringTable = Ego_r.pdb with a 12-byte STLport map: StringMap +0 (header node +0), StringStream
// (CMemoryDataOutputStream, 0x18 bytes) +0xc, StringCount +0x24, SharedCount +0x28, RandomID +0x2c, TableFilename +0x30,
// Locked +0x34 (PDB offsets +4/+8 higher). Map nodes: key CCharString +0x10, value long +0x14.
// Shape: the empty test is `GetLength() == 0` (an explicit `PStringData == 0 || Length == 0` moves the -1 exit to
// the end); the CRC call's arguments come from two null-first CCharString accessors (length evaluated first, then the
// text with the "" fallback); the CRC is written through an inlined WriteULong whose parameter is homed in the dead
// `text` argument slot; the shared-string path returns the node's value straight from the find result.
class CCharStringData { public:
    char* Text;                                    // +0x00
    long Length;                                   // +0x04
};
extern const char g_emptyString[];                 // 0x0129AAF4 ""
class CCharString { public:
    CCharStringData* PStringData;
    long GetLength() const { return PStringData == 0 ? 0 : PStringData->Length; }
    const char* GetPString() const { return PStringData == 0 ? g_emptyString : PStringData->Text; }
};
class CCRC {
public:
    static unsigned long __fastcall Calc(unsigned long, const void*, unsigned long);   // 0x004014A0
};   // 0x004014A0 CRC

class CDataOutputStream
{
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void Write(const void* data, long length);   // slot 4 (+0x10)
public:
    void WriteNullTerminatedString(const CCharString&);   // 0x00995AE0
};

class CMemoryDataOutputStream : public CDataOutputStream { public:                   // 0x18 bytes, vtable 0x0129A788
    void* NewLine;                                 // +0x04
    unsigned char* First;                          // +0x08
    unsigned char* Last;                           // +0x0c
    unsigned char* End;                            // +0x10
    unsigned long Position;                        // +0x14
    long GetSize();                                // 0x00994B40
    void WriteRepeatedByte(unsigned char value, long count);   // 0x00994230
    void* PeekData();                              // 0x00995E70
    void WriteULong(unsigned long value) { Write(&value, 4); }
};

struct SStringNode {
    long Color;
    SStringNode* Parent;
    SStringNode* Left;
    SStringNode* Right;
    CCharString Key;                               // +0x10
    long Offset;                                   // +0x14
};
struct CStringMap {                                // STLport map<CCharString, long>
    SStringNode* Header;                           // +0x00
    unsigned long NodeCount;                       // +0x04
    unsigned long Compare;                         // +0x08
    SStringNode* find(const CCharString& key);     // 0x0042B467
    long& operator[](const CCharString& key);      // 0x00519230
};

extern unsigned char* g_pDefStringTableData;       // 0x013CA824

class CDefStringTable {
public:
    CStringMap StringMap;                          // +0x00
    CMemoryDataOutputStream StringStream;          // +0x0c
    unsigned long StringCount;                     // +0x24
    unsigned long SharedCount;                     // +0x28
    unsigned long RandomID;                        // +0x2c
    void* TableFilename;                           // +0x30 CWideString
    bool Locked;                                   // +0x34
    bool ReadOnly;                                 // +0x35
    void Unlock();                                 // 0x009D4F10
    long AddString(const CCharString& text);
};

long CDefStringTable::AddString(const CCharString& text)
{
    if (text.GetLength() == 0)
        return -1;
    if (Locked)
        Unlock();
    SStringNode* node = StringMap.find(text);
    if (node != StringMap.Header) {
        ++SharedCount;
        return node->Offset;
    }
    if (StringStream.GetSize() == 0)
        StringStream.WriteRepeatedByte(0, 4);
    long offset = StringStream.GetSize();
    StringStream.WriteULong(CCRC::Calc(0, text.GetPString(), text.GetLength()));
    StringStream.WriteNullTerminatedString(text);
    StringMap[text] = offset;
    ++StringCount;
    g_pDefStringTableData = (unsigned char*)StringStream.PeekData() + 4;
    return offset;
}