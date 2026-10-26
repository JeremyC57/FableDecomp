// CAFile::ParserReadPastComponent(CStringParser& parser, CCharString& component) -- read one path component (up to the next separator or the end) into component; false when none is there, it holds a forbidden symbol, or it ends in a space or a stray dot
// Leaf: ?ParserReadPastComponent@CAFile@@
// Flags: /Oa
// Retail: Fable.exe (TLC, VC7.1) @ 0x00998010, 343 bytes (+ int3 padding). FableWin.pdb
//   `?ParserReadPastComponent@CAFile@@CI_NAAVCStringParser@@AAVCCharString@@@Z`: a private static `__fastcall` (parser in
//   ecx, component in edx), plain `ret`. Callers: the CAFile path parsers 0x009981EF / 0x00998B1D / 0x00998B64.
// Callees: CAFile::ParserIsSeparatorNext 0x00997DA0 (landed), CStringParser::NextItemExists 0x009BA4F0,
//   CStringParser::SetStringPos(unsigned long) 0x009B9940, CCharString(const char*, long = -1) 0x0099EBF0,
//   CCharString::GetSubString(unsigned long, unsigned long) const 0x0099EC70, CCharString::operator= 0x0099EFB0,
//   ~CCharString 0x0099EAE0, CAFile::IsForbiddenSymbolInComponent 0x009975E0 (landed), CBasicString<char>::operator==
//   (const char*) const 0x004115A0. Data: "." 0x01238794, ".." 0x0129A7B8, "" 0x0122D70E, the CCharString empty text
//   0x0129AAF4.
// Layout: retail CStringParser (Ego_r.pdb names; the STLport comment list is 4 bytes, so the cursor position sits at
//   +0x20, PDB +0x28): String +0x04. CCharString's data block {PData +0, StringLength +4}.
// Shape: the parser's whole text is copied into a CCharString, scanned from GetStringPos() one character at a time
//   (the last one kept), moving the parser along until a separator or the end; GetSubString(start, pos + 1) is built
//   in the dead `start` slot; `result` is a named bool returned through the shared destructor tail. CCharString's
//   inline GetLength / GetString test the data block, its `operator==(const char*)` is the forced-inline form of the
//   landed CAFile::ParserReadPastDriveLabel 0x00998170 (intrinsic strcmp against "" without a block).
//   /Oa is the retail TU's: component's data block is loaded once and reused across the out-of-line `== "."`
//   call for the `== ".."` test, and the component reference stays in its frame slot through the scan (without /Oa,
//   VC7.1 keeps it in ebp and reloads the block: 350 bytes).
#include <string.h>
#pragma intrinsic(strcmp)

extern const char g_emptyLiteral_0129aaf4[];            // 0x0129AAF4: ""

struct CBasicStringChar {                               // CBasicString<char> (packed 13 bytes in retail)
    char*         PData;                                // +0x00
    long          StringLength;                         // +0x04
    bool operator==(const char* text) const;            // 0x004115A0
};
class CCharStringData { public:
    CBasicStringChar Data;                              // +0x00
};

class CCharString
{
public:
    CCharStringData* PStringData;                       // +0x00
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
    CCharString GetSubString(unsigned long start, unsigned long end) const;   // 0x0099EC70
    long GetLength() const { return PStringData == 0 ? 0 : PStringData->Data.StringLength; }
    const char* GetString() const { return PStringData == 0 ? g_emptyLiteral_0129aaf4 : PStringData->Data.PData; }
    char operator[](long index) const { return GetString()[index]; }
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};

class CStringParser                                     // retail layout (Ego_r.pdb names)
{
public:
    void*         __vftable;                            // +0x00
    const char*   String;                               // +0x04
    unsigned char _pad08[0x20 - 0x08];
    unsigned long StringPos;                            // +0x20 (CursorPos)
    bool NextItemExists();                              // 0x009BA4F0
    unsigned long GetStringPos() const { return StringPos; }
    void SetStringPos(unsigned long pos);               // 0x009B9940
};

class CAFile
{
public:
    static bool __fastcall ParserIsSeparatorNext(CStringParser& parser);              // 0x00997DA0
    static bool __fastcall IsForbiddenSymbolInComponent(const CCharString& component); // 0x009975E0
    static bool __fastcall ParserReadPastComponent(CStringParser& parser, CCharString& component);
};

bool __fastcall CAFile::ParserReadPastComponent(CStringParser& parser, CCharString& component)
{
    if (ParserIsSeparatorNext(parser) || !parser.NextItemExists())
        return false;
    bool result = false;
    CCharString text(parser.String);
    long start = parser.GetStringPos();
    long pos = start;
    char last = ' ';
    while (pos < text.GetLength()) {
        last = text[pos];
        parser.SetStringPos(pos + 1);
        if (ParserIsSeparatorNext(parser) || !parser.NextItemExists())
            break;
        ++pos;
    }
    component = text.GetSubString(start, pos + 1);
    if (!IsForbiddenSymbolInComponent(component) && last != ' ' &&
        (last != '.' || component == "." || component == ".."))
        result = true;
    return result;
}