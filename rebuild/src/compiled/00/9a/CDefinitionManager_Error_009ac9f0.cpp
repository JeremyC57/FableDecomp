// CDefinitionManager::Error(const CStringParser& parser, const CCharString& message) -- report a parse error as "<file>(<line>) : error : <message>" through the manager's error callback
// Leaf: Error
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009AC9F0, length 175 bytes, `ret 8`.
// Callees: the integer-to-string formatter @ 0x0099F830 (static __fastcall: result in ecx, value in edx; catalog
// "GFIntToCharString_API"), operator+(const CCharString&, const char*) @ 0x0099F600 and operator+(const CCharString&,
// const CCharString&) @ 0x0099F570 (__fastcall globals, result in ecx, left in edx), ~CCharString @ 0x0099EAE0, the
// ErrorCallback member (`static void __fastcall ErrorCallback(const CCharString&)`, e.g.
// ?ErrorCallback@CGameDefinitionManager@@KIXABVCCharString@@@Z). Literals "(" @ 0x0122E028, ")" @ 0x0122E020,
// " : error : " @ 0x0129B084.
// Layout: retail CStringParser (0x28 bytes): Filename +0xc, ErrorCallback +0x18, CursorPos {InQuotes +0x1c, StringPos
// +0x20, Line +0x24} (PDB list is 16 bytes, retail STLport list 4). Retail CDefinitionManager keeps ErrorCallback at
// +0xb0 (engine/CDefinitionManager.h has the PDB offset +0xcc: its vectors/CVectorMaps/CArrays are 4 bytes narrower in
// retail); the member is declared on a local view.
// Shape: the line number is read into a local first (formatting `Line + 1` inline loads the message argument early
// and swaps the two dead-slot temporaries); the message is a named local built by one operator+ chain (VC evaluates the right operands first: the message
// and the two literals are pushed before the line number is formatted; the last two temporaries reuse the dead
// argument slots), all temporaries die before the callback call, the local after it.
class CCharString { public:
    void* PStringData;
    ~CCharString();                                                      // 0x0099EAE0
};
CCharString __fastcall GFIntToCharString(long value);                     // 0x0099F830
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600

struct CCursorPos {
    bool InQuotes;                                 // +0x00
    unsigned long StringPos;                       // +0x04
    unsigned long Line;                            // +0x08
};
class CStringParser { public:                             // retail 0x28 bytes
    void* __vftable;                               // +0x00
    char* String;                                  // +0x04
    unsigned long BaseLineOffset;                  // +0x08
    CCharString Filename;                          // +0x0c
    CCharString TempCharString;                    // +0x10
    void* Comments;                                // +0x14 STLport list (4 bytes)
    void (__fastcall* ErrorCallback)(const CCharString&);   // +0x18
    CCursorPos CursorPos;                          // +0x1c
};

class CDefinitionManager {
public:
    unsigned char _pad[0xb0];
    void (__fastcall* ErrorCallback)(const CCharString& text);   // +0xb0 (PDB +0xcc)
    void Error(const CStringParser& parser, const CCharString& message);
};

void CDefinitionManager::Error(const CStringParser& parser, const CCharString& message)
{
    long line = parser.CursorPos.Line;
    CCharString text = parser.Filename + "(" + GFIntToCharString(line + 1) + ")" + " : error : " + message;
    ErrorCallback(text);
}