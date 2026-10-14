// CDefStringTable::GetString(long) -- the string stored `index` bytes into the table's memory stream (after its 4-byte length), or an empty string for -1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D49B0, length 64 bytes.
// Callees: CCharString::CCharString() @ 0x0099E4B0, CMemoryDataOutputStream::PeekData() @ 0x00995E70 on the stream at +0xc,
// CCharString::CCharString(const char*, long) @ 0x0099EBF0 (length -1 = strlen).
// Shape: both returns construct straight into the hidden result (`ret 8`: result pointer + index); the index is read
// into esi before the test and the result pointer only inside each branch; the text pointer is a named local so the
// PeekData call precedes the `push -1` of the constructor (a bare argument expression pushes the -1 first). PDB CDefStringTable: StringStream at
// +16 (retail +0xc, the map before it is 12 bytes).
struct CCharString {
    void* PStringData;                             // +0
    CCharString();                                 // 0x0099E4B0
    CCharString(const char* text, long length);    // 0x0099EBF0
};

struct CMemoryDataOutputStream {
    void* PeekData();                              // 0x00995E70
};

struct CDefStringTable {
    unsigned char           StringMap[0xc];        // +0x00 map<CCharString, long>
    CMemoryDataOutputStream StringStream;          // +0x0c
    CCharString GetString(long index);
};

CCharString CDefStringTable::GetString(long index)
{
    if (index == -1)
        return CCharString();
    const char* text = (const char*)StringStream.PeekData() + index + 4;
    return CCharString(text, -1);
}
