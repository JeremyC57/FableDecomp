// CSubtitleRenderer::WordWrapText(const C2DBoxF& box, ..., const CWideString& text) -- break the text into lines that fit the box width, each followed by "\n"; a line starting with ',' is pulled back onto the previous line (",\n")
#pragma optimize("y", off)
#pragma optimize("s", on)
// Leaf: WordWrapText
// Retail: Fable.exe (TLC, VC7.1) @ 0x0048DC8B, 265 bytes, `ret 10h` (hidden CWideString result + three arguments; the
//   second argument is never read). Called from 0x0048F543.
// Callees: CWideString::CWideString() 0x0099AED0, CWideString::operator const wchar_t* 0x0099AF10,
//   _ftol2 0x00BFEA70, WordWrap_FindNextLineW(const wchar_t*, unsigned long, const wchar_t**) 0x00A10390 (__fastcall),
//   CWideString::SetAsSubString(const wchar_t*, long) 0x0099B770, CWideString::operator=(const wchar_t*) 0x0099B800,
//   CWideString::GetLength() const 0x0099B220, CWideString::SetLength(long) 0x0099BC50,
//   CWideString::operator+=(const wchar_t*) 0x0099B940, CWideString::GetSubString(unsigned long, unsigned long) const
//   0x0099BDB0 (by value), CWideString::operator=(const CWideString&) 0x0099B7D0, ~CWideString 0x0099B510,
//   operator+(const CWideString&, const wchar_t*) 0x0099BF30 (__fastcall, hidden result in ecx),
//   CWideString::operator+=(const CWideString&) 0x0099B8D0, CWideString::CWideString(const CWideString&) 0x0099B720.
//   Literals L",\n" 0x01238114, L"\n" 0x01231728.
// Shape: the frame-pointer size TU of the landed CSubtitleRenderer rows. The result is a named local copied out (no
//   NRVO); the per-line CWideString is homed in the dead `text` slot and the finder's zeroed out-pointer in the dead
//   `box` slot (`and [ebp+0Ch],0`). The width is `right - left` over two named float locals read in that order (left
//   first, kept on the x87 stack and popped after _ftol2); `box.BRX - box.TLX` or an inline GetWidth() folds to
//   `fld [esi+8] ; fsub [esi]`.
#include <stddef.h>

class C2DBoxF { public: float TLX, TLY, BRX, BRY; };
class CFontBank;

class CWideString {
public:
    void* PStringData;
    CWideString();                                       // 0x0099AED0
    CWideString(const CWideString& other);               // 0x0099B720
    ~CWideString();                                      // 0x0099B510
    operator const wchar_t*() const;                     // 0x0099AF10
    const CWideString& operator=(const wchar_t* text);   // 0x0099B800
    const CWideString& operator=(const CWideString& other);    // 0x0099B7D0
    const CWideString& operator+=(const wchar_t* text);  // 0x0099B940
    const CWideString& operator+=(const CWideString& other);   // 0x0099B8D0
    void SetAsSubString(const wchar_t* text, long length);     // 0x0099B770
    long GetLength() const;                              // 0x0099B220
    void SetLength(long length);                         // 0x0099BC50
    CWideString GetSubString(unsigned long start, unsigned long end) const;   // 0x0099BDB0
};
CWideString __fastcall operator+(const CWideString& a, const wchar_t* b);    // 0x0099BF30

const wchar_t* __fastcall WordWrap_FindNextLineW(const wchar_t* text, unsigned long width, const wchar_t** lineEnd);   // 0x00A10390

class CSubtitleRenderer {
public:
    CWideString WordWrapText(const C2DBoxF& box, CFontBank* font, const CWideString& text);
};

CWideString CSubtitleRenderer::WordWrapText(const C2DBoxF& box, CFontBank* font, const CWideString& text)
{
    CWideString wrapped;
    const wchar_t* line = text;
    const wchar_t* next;
    do {
        float left = box.TLX;
        const wchar_t* lineEnd = 0;
        float right = box.BRX;
        next = WordWrap_FindNextLineW(line, (long)(right - left), &lineEnd);
        CWideString lineText;
        if (next)
            lineText.SetAsSubString(line, next - line);
        else
            lineText = line;
        if (*(const wchar_t*)lineText == L',') {
            wrapped.SetLength(wrapped.GetLength() - 1);
            wrapped += L",\n";
            lineText = lineText.GetSubString(1, lineText.GetLength());
        }
        wrapped += lineText + L"\n";
        line = next;
    } while (next);
    return wrapped;
}