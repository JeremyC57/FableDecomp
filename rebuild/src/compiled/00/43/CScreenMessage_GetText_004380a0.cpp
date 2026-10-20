// CScreenMessage::GetText() const -- the message text to display: a copy of Text with a leading "(HC)" marker stripped, cut to the number of characters revealed so far
// Leaf: GetText
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004380A0, length 269 bytes (`ret 4`: the CWideString result by hidden pointer).
// Callees: CWideString copy ctor @ 0x0099B720, CCharString(const char*, long) @ 0x0099EBF0 ("(HC)" @ 0x0123172C),
// CWideString::ConvertToCharString @ 0x0099B2C0, strncmp (MSVCR71 import thunk 0x00BFEAF8), ~CCharString @ 0x0099EAE0,
// CWideString::GetLength @ 0x0099B220, CCharString::GetSubString(unsigned long, unsigned long) @ 0x0099EC70,
// CCharString::ToWideString @ 0x0099E480, CWideString::operator= @ 0x0099B7D0, ~CWideString @ 0x0099B510,
// CWideString::SetLength @ 0x0099BC50. The empty string is 0x0129AAF4.
// Layout: Ego_r.pdb CScreenMessage: Text +0 (CWideString), FramesToShowFor +4, NumberOfCharactersShowing +8, LinesInText +0xc
// (as the landed CScreenMessage::Update 0x00438770).
// Shape: the prefix test is an inline CCharString member on the converted temporary (the prefix's text and length read with
// ONE null test, null case first, the temporary's text through a null-first accessor) returning `strncmp(..) ? false :
// true` (`neg ; sbb bl,bl ; inc bl`); the prefix literal lives in its own block so its destructor runs right after the
// temporary's; the strip is one chained expression (GetLength evaluated first, right to left); the clamp reads the count
// before the length call; the result is a copy of the local (no NRVO).
#include <string.h>

extern const char g_szEmpty[];                       // 0x0129AAF4 ""

struct CWideString;
struct CCharStringData {
    const char*   Chars;                             // +0
    unsigned long Length;                            // +4
};
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length);      // 0x0099EBF0
    ~CCharString();                                  // 0x0099EAE0
    CCharString GetSubString(unsigned long start, unsigned long count) const;   // 0x0099EC70
    CWideString ToWideString() const;                // 0x0099E480
    const char* GetCharPtr() const { return PStringData == 0 ? g_szEmpty : PStringData->Chars; }
    bool BeginsWith(const CCharString& prefix) const
    {
        const char*   prefixText;
        unsigned long prefixLength;
        if (prefix.PStringData == 0) {
            prefixLength = 0;
            prefixText = g_szEmpty;
        }
        else {
            prefixLength = prefix.PStringData->Length;
            prefixText = prefix.PStringData->Chars;
        }
        return strncmp(GetCharPtr(), prefixText, prefixLength) ? false : true;
    }
};

struct CWideString {
    void* m_pData;
    CWideString(const CWideString& other);           // 0x0099B720
    ~CWideString();                                  // 0x0099B510
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    long GetLength() const;                          // 0x0099B220
    void SetLength(long length);                     // 0x0099BC50
    CCharString ConvertToCharString() const;         // 0x0099B2C0
};

class CScreenMessage {
public:
    CWideString GetText() const;
private:
    CWideString Text;                                // +0x00
    long        FramesToShowFor;                     // +0x04
    long        NumberOfCharactersShowing;           // +0x08
    long        LinesInText;                         // +0x0C
};

CWideString CScreenMessage::GetText() const
{
    CWideString text(Text);
    bool hasPrefix;
    {
        CCharString prefix("(HC)", -1);
        hasPrefix = text.ConvertToCharString().BeginsWith(prefix);
    }
    if (hasPrefix)
        text = Text.ConvertToCharString().GetSubString(4, Text.GetLength()).ToWideString();
    if (NumberOfCharactersShowing < text.GetLength())
        text.SetLength(NumberOfCharactersShowing);
    return text;
}