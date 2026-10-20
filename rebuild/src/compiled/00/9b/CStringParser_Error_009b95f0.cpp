// CStringParser::Error(const CCharString& message) -- format "<file>(<line>) : error : <message>" and hand it to the error callback, if one is set
// Leaf: Error
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B95F0, 170 bytes, `ret 4`.
// Identity: Ego_r.pdb `?Error@CStringParser@@IAEXABVCCharString@@@Z`; the landed ReadAsSymbol 0x009BA790 calls it.
// Callees: GFIntToCharString @ 0x0099F830, operator+(const CCharString&, const char*) @ 0x0099F600,
//   operator+(const CCharString&, const CCharString&) @ 0x0099F570 (all `__fastcall`, hidden result in ecx),
//   ~CCharString @ 0x0099EAE0.
// Layout: the retail 0x28-byte CStringParser of the landed siblings (Filename +0x0c, ErrorCallback +0x18, CursorPos.Line
//   +0x24; the line number is zero-based, the message one-based).
// Shape: one chained expression: VC pushes each right operand before evaluating the left chain, so the message and the
//   two literals go first, then the line string is built; the five intermediate temporaries die in reverse order at the
//   end of the statement and the finished text lives in the dead `message` argument slot. The callback is a
//   `__fastcall` function pointer (string in ecx).
struct CCharString {                     // one CCharStringData pointer
    void* PStringData;
    ~CCharString();                      // 0x0099EAE0
};
CCharString __fastcall GFIntToCharString(long value);                                  // 0x0099F830
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
struct CCursorPos {                      // PDB: InQuotes / StringPos / Line (12 bytes)
    bool          InQuotes;
    unsigned long StringPos;
    unsigned long Line;
};
// Retail layout (0x28 bytes; the Ego_r PDB's debug-build layout is 0x30):
struct CStringParser {
    virtual ~CStringParser();
    char*          String;               // +0x04
    unsigned long  BaseLineOffset;       // +0x08
    CCharString    Filename;             // +0x0c
    CCharString    TempCharString;       // +0x10
    void*          Comments;             // +0x14 STLport list node pointer
    void (__fastcall* ErrorCallback)(const CCharString&);   // +0x18
    CCursorPos     CursorPos;            // +0x1c
protected:
    void Error(const CCharString& message);
};
void CStringParser::Error(const CCharString& message)
{
    CCharString text = Filename + "(" + GFIntToCharString(CursorPos.Line + 1) + ")" + " : error : " + message;
    if (ErrorCallback)
        ErrorCallback(text);
}