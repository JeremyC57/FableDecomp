// CSubtitleRenderer::GetCharWidthW(wchar_t character) -- word-wrap callback: the width of one character in the word-wrap font, in whole virtual units
// Retail: Fable.exe (TLC, VC7.1) @ 0x0048E012, 80 bytes, plain `ret`.
// Callees: CWideString::CWideString() @ 0x0099AED0, CWideString::Sprintf(const wchar_t*, ...) @ 0x0099BA70
// (cdecl varargs, format L"%c" @ 0x01238154), CFontBank::GetVStringExtents(const CWideString&) const @ 0x00437210
// (C2DExtentsF by hidden pointer), __ftol2 @ 0x00BFEA70, CWideString::~CWideString @ 0x0099B510.
// Global: the word-wrap font g_pWordWrapFont @ 0x013B87AC.
// Shape: Ego_r.pdb `?GetCharWidthW@CSubtitleRenderer@@CII_W@Z` -- a private STATIC `__fastcall` returning unsigned
// int (the character arrives in cx and is parked in si). One-character string, formatted, measured, the width
// truncated with __ftol2 before the string's destructor; no EH frame (the retail module builds without /EHsc).
// The landed FormatTextForVWindow (0x0048F50C) hands this function's address to the word-wrapper; its local
// declaration `long __cdecl WordWrapCharWidth(wchar)` has the wrong convention (the body reads cx) but only the
// address is taken there.
// Pragma: the CSubtitleRenderer TU's `optimize("y", off)` + `("s", on)` (ebp frame, `leave`), as the landed
// IsInInteractiveState 0x00490A22.
// Leaf: GetCharWidthW
#pragma optimize("y", off)
#pragma optimize("s", on)

typedef unsigned short wchar_t16;

class CWideString {
public:
    void* PStringData;
    CWideString();                                          // 0x0099AED0
    ~CWideString();                                         // 0x0099B510
    void __cdecl Sprintf(const wchar_t16* format, ...);   // 0x0099BA70
};

class C2DExtentsF {
public:
    float Width;
    float Height;
    C2DExtentsF(float width, float height) : Width(width), Height(height) { }
};

class CFontBank {
public:
    C2DExtentsF GetVStringExtents(const CWideString& text) const;   // 0x00437210
};
extern const CFontBank* g_pWordWrapFont;                    // 0x013B87AC


class CSubtitleRenderer {
public:
    static unsigned int __fastcall GetCharWidthW(wchar_t16 character);
};

unsigned int __fastcall CSubtitleRenderer::GetCharWidthW(wchar_t16 character)
{
    CWideString text;
    text.Sprintf(L"%c", character);   // L"%c" @ 0x01238154
    return (unsigned int)g_pWordWrapFont->GetVStringExtents(text).Width;
}