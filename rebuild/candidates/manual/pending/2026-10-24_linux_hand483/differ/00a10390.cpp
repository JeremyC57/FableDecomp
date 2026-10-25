// WordWrap_FindNextLineW(const wchar_t* text, unsigned int maxWidth, const wchar_t** lineEnd) -- wide word-wrap line finder: measure characters through the width callback until a newline, the end or maxWidth; then pick the break (whitespace, a Hangul boundary, or any pair allowed by the kinsoku start/end rules), store the line's last character in *lineEnd and return the next line's start (0 at the end of the text)
// Grade: DIFFER(396v458) -- lane gap: custom register convention (LTCG-style) in the callees: IsHangulChar 0x00A10240, IsNonBeginningChar 0x00A0FE80 and IsNonEndingChar 0x00A10170 take their character in ax (`mov eax,ecx ; call`, `mov ax,[esi-2] ; call`) and return in al, which no VC7.1 C++ declaration reproduces without /GL; the reference body also differs in block layout (TrimSpaceBackward is expanded once in retail, ours calls it at every site)
// Leaf: WordWrap_FindNextLineW
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A10390, 457 bytes, __fastcall `ret 4` (ecx text, edx maxWidth, stack lineEnd).
//   Caller: WordWrap_FindNextLineA 0x00A10560 (its multibyte front end). Name: the chunk label (descriptive; file-static set).
// Globals: the character-width callback 0x013CE758 (null = word wrap disabled).
// Callees: TrimSpaceBackward 0x00A10350 (ecx p, edx start; also expanded once in the break search), SkipSpaceForward
//   0x00A10310 (ecx p), the three ax-argument kinsoku / Hangul tests above.
// Shape: whitespace is tab, CR, space and U+3000 (CR/LF excluded from the measured run); the break search walks back from the
//   overflow character; with no break the line is cut at the overflow character.
typedef unsigned short wchar_t;                     // VC7.1 default /Zc:wchar_t-

extern int (__fastcall* g_pfnWordWrapGetCharWidth)(wchar_t ch);   // 0x013CE758

const wchar_t* __fastcall TrimSpaceBackward(const wchar_t* p, const wchar_t* start);   // 0x00A10350
const wchar_t* __fastcall SkipSpaceForward(const wchar_t* p);                          // 0x00A10310
bool __fastcall IsHangulChar(wchar_t ch);           // 0x00A10240 (retail: ch in ax)
bool __fastcall IsNonBeginningChar(wchar_t ch);     // 0x00A0FE80 (retail: ch in ax)
bool __fastcall IsNonEndingChar(wchar_t ch);        // 0x00A10170 (retail: ch in ax)

inline bool IsWrapSpace(wchar_t ch) { return ch == 9 || ch == 13 || ch == 32 || ch == 0x3000; }

const wchar_t* __fastcall WordWrap_FindNextLineW(const wchar_t* text, unsigned int maxWidth, const wchar_t** lineEnd)
{
    if (g_pfnWordWrapGetCharWidth == 0 || text == 0 || *text == 0) {
        *lineEnd = 0;
        return 0;
    }
    const wchar_t* p = text;
    unsigned int width = 0;
    do {
        if (*p == 10)
            break;
        width += g_pfnWordWrapGetCharWidth(*p);
        if (width > maxWidth)
            break;
        ++p;
    } while (*p != 0);

    if (p == text) {
        *lineEnd = TrimSpaceBackward(p, text);
        return SkipSpaceForward(p + 1);
    }
    if (*p == 10)
        ++p;
    if (width <= maxWidth) {
        const wchar_t* last = p - 1;
        *lineEnd = TrimSpaceBackward(last, text);
        if (p - 1 >= text && *last == 10)
            return p;
        if (*p == 0)
            return 0;
        return SkipSpaceForward(p);
    }

    const wchar_t* overflow = p;
    if (p > text) {
        do {
            if (IsWrapSpace(*p)) {
                const wchar_t* end = TrimSpaceBackward(p, text);
                *lineEnd = end;
                if (end == 0)
                    return SkipSpaceForward(p + 1);
                p = end + 1;
            }
            if (p != text) {
                wchar_t ch = *p;
                if (IsWrapSpace(ch) || IsHangulChar(ch) || IsHangulChar(p[-1])) {
                    if (!IsNonBeginningChar(ch) && !IsNonEndingChar(p[-1])) {
                        if (p > text) {
                            *lineEnd = p - 1;
                            return SkipSpaceForward(p);
                        }
                        break;
                    }
                }
            }
            --p;
        } while (p > text);
    }
    *lineEnd = overflow - 1;
    return overflow;
}
