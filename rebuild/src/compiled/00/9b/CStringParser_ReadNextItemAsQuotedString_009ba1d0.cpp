// CStringParser::ReadNextItemAsQuotedString(CCharString& text) -- skip white space, find the closing quote after the cursor (honouring backslash escapes), copy the quoted text with "\\" collapsed to one backslash and lone backslashes dropped, step the cursor past the closing quote; false when unterminated
// Leaf: ReadNextItemAsQuotedString
// Retail: Fable.exe (TLC, VC7.1) @ 0x009BA1D0, 255 bytes, `ret 4`.
// Callees: CStringParser::SkipWhitespace 0x009B9C60, CCharString::CCharString() 0x0099E4B0,
//   CCharString::operator+=(char) 0x0099F180, CCharString::operator=(const CCharString&) 0x0099EFB0,
//   ~CCharString 0x0099EAE0. CalcInQuotes (out of line at 0x009B9570) is inlined here through MoveStringPos.
// Layout: the retail 0x28-byte CStringParser of the landed siblings (String +0x04, CursorPos +0x1c = InQuotes, StringPos
//   +0x20, Line +0x24; engine/CStringParser.h carries the PDB's 0x30-byte debug layout, so the model is local).
class CCharStringData { public:
    const char*   Chars;                 // +0
    unsigned long Length;                // +4
};
class CCharString { public:                     // one CCharStringData pointer
    CCharStringData* PStringData;
    CCharString();                       // 0x0099E4B0
    ~CCharString();                      // 0x0099EAE0
    CCharString& operator+=(char c);     // 0x0099F180
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
struct CCursorPos {                      // PDB: InQuotes / StringPos / Line (12 bytes)
    bool          InQuotes;
    unsigned long StringPos;
    unsigned long Line;
};
// Retail layout (0x28 bytes; the Ego_r PDB's debug-build layout is 0x30):
class CStringParser { public:
    virtual ~CStringParser();
    char*          String;               // +0x04
    unsigned long  BaseLineOffset;       // +0x08
    CCharString    Filename;             // +0x0c
    CCharString    TempCharString;       // +0x10
    void*          Comments;             // +0x14 STLport list node pointer
    void (__fastcall* ErrorCallback)(const CCharString&);   // +0x18
    CCursorPos     CursorPos;            // +0x1c
protected:
    long SkipWhitespace();               // 0x009B9C60
    bool CalcInQuotes(bool inQuotes, const char* from, const char* to) const   // 0x009B9570 when out of line
    {
        while (from != to) {
            char c = *from;
            if (c == '"' || c == '\'')
                inQuotes = !inQuotes;
            ++from;
        }
        return inQuotes;
    }
    void MoveStringPos(unsigned long n)
    {
        if (n > 0) {
            CursorPos.InQuotes = CalcInQuotes(CursorPos.InQuotes, String + CursorPos.StringPos, String + CursorPos.StringPos + n);
            CursorPos.StringPos += n;
        }
    }
public:
    bool ReadNextItemAsQuotedString(CCharString& text);
};

bool CStringParser::ReadNextItemAsQuotedString(CCharString& text)
{
    SkipWhitespace();
    long end = CursorPos.StringPos;
    bool escaped = false;
    for (++end; String[end]; ++end) {
        if (escaped)
            escaped = false;
        else if (String[end] == '\\')
            escaped = true;
        else if (String[end] == '"' || String[end] == '\'') {
            CCharString quoted;
            for (long i = CursorPos.StringPos + 1; i < end; ++i) {
                if (String[i] == '\\') {
                    if (i + 1 < end && String[i + 1] == '\\') {
                        quoted += '\\';
                        ++i;
                    }
                } else
                    quoted += String[i];
            }
            MoveStringPos(end - CursorPos.StringPos + 1);
            text = quoted;
            return true;
        }
    }
    return false;
}