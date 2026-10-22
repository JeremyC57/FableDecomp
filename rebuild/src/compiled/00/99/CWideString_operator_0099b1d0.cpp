// CWideString::operator!=(wchar_t) const -- true unless the string holds exactly the one character c (a string with no data block is never equal)
// Leaf: ??9CWideString
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099B1D0, 71 bytes, `ret 4`. Callees: wcscmp (MSVCR71 import thunk 0x00BFE94C).
// Unreferenced in the image (recovered from a mis-cut catalog row); it sits between the landed
// CWideString members 0x0099B150 (operator!=(const wchar_t*)) and 0x0099B240.
// Shape: `return !(*this == c);` over an inline `operator==(wchar_t)` written with the data-block test first
// (retail keeps the compare path first): no data block -> false; else a two-wchar
// buffer {c, 0} built in the dead argument slot and compared with wcscmp (`neg ; sbb al,al ; inc al`), then
// the negation through `xor ecx,ecx ; test al,al ; sete cl` on both paths.
// Layout: the shared rebuild/include/fable_string.h CWideString has no character comparison, so a local
// model with the same single storage pointer is used (CWideStringData: text +0).
#include <wchar.h>    // wcscmp, MSVCR71 import thunk 0x00BFE94C (wchar_t is the unsigned short typedef under VC7.1)

struct CWideStringData {
    wchar_t*      text;                // +0x00
    unsigned long unknown04;           // +0x04
    unsigned long unknown08;           // +0x08
    long          owners;              // +0x0C
};

class CWideString {
public:
    bool operator==(wchar_t c) const
    {
        if (storage_ != 0) {
            wchar_t text[2];
            text[0] = c;
            text[1] = 0;
            return wcscmp(storage_->text, text) == 0;
        }
        return false;
    }
    bool operator!=(wchar_t c) const;
private:
    CWideStringData* storage_;         // +0x00
};

bool CWideString::operator!=(wchar_t c) const
{
    return !(*this == c);
}