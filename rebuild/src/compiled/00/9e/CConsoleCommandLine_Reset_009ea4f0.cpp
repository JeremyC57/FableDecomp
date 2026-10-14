// CConsoleCommandLine::Reset() -- clear the typed entry, the user-entry flag and the cursor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009EA4F0, length 26 bytes.
// Callees: CCharString::operator=(const char*) @ 0x0099EFE0 with the empty literal @ 0x0122D70E.
// Layout from the PDB (CConsoleCommandLine, 16 bytes): vptr, CCharString CurrentEntry +0x04,
// bool UserEntry +0x08, long CursorPosition +0x0c. The flag and the cursor are both cleared from
// the same zero register (xor eax ; mov [+8],al ; mov [+0xc],eax).
struct CCharString {
    void* PStringData;
    const CCharString& operator=(const char* text);   // 0x0099EFE0
};

struct CConsoleCommandLine {
    virtual void v00();
    CCharString CurrentEntry;       // +0x04
    bool        UserEntry;          // +0x08
    long        CursorPosition;     // +0x0c
    void Reset();
};

void CConsoleCommandLine::Reset()
{
    CurrentEntry = "";
    UserEntry = false;
    CursorPosition = 0;
}