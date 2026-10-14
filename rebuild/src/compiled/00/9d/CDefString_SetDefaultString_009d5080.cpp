// CDefString::SetDefaultString(const CCharString&) -- register the text in the global def string table unless the table is locked
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D5080, length 33 bytes.
// Callees: CDefStringTable::AddString(const CCharString&) @ 0x009D4F90 on the global table @ 0x013CA828 (its Locked
// byte is read directly @ 0x013CA85C = table + 0x34).
// PDB CDefStringTable (64 bytes): StringMap +0, StringStream +16, StringCount +44, SharedCount +48, RandomID +52,
// TableFilename +56, bool Locked +60; retail map (12) and stream (24) are 8 bytes smaller in all, so Locked is +0x34.
struct CCharString;

struct CDefStringTable {
    unsigned char _pad[0x34];                      // +0x00 StringMap, StringStream, counts, random id, filename
    bool          Locked;                          // +0x34
    unsigned char ReadOnly;                        // +0x35
    long AddString(const CCharString& text);       // 0x009D4F90
};

extern CDefStringTable g_DefStringTable;           // 0x013CA828

struct CDefString {
    long TablePos;                                 // +0
    void SetDefaultString(const CCharString& text);
};

void CDefString::SetDefaultString(const CCharString& text)
{
    if (!g_DefStringTable.Locked)
        TablePos = g_DefStringTable.AddString(text);
}