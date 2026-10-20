// CProgressDisplay::WordWrapText(const C2DBoxF& box, const CFontBank& font, const CWideString& text) const -- break the text into lines that fit the box width, each line followed by a newline
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00497A60, length 202 bytes (+ int3 padding), `ret 10h` (hidden result + 3).
// Identity: Ego_r.pdb `?WordWrapText@CProgressDisplay@@ABE?AVCWideString@@ABVC2DBoxF@@ABVCFontBank@@ABV2@@Z` (the free
// __fastcall twin `?WordWrapText@@YI...` is 0x009BC900); the font is not read.
// Callees: CWideString default ctor 0x0099AED0, operator const wchar_t* 0x0099AF10, __ftol2 0x00BFEA70,
// WordWrap_FindNextLineW(const wchar_t*, unsigned, const wchar_t**) 0x00A10390 (__fastcall),
// CWideString::SetAsSubString(const wchar_t*, long) 0x0099B770, operator=(const wchar_t*) 0x0099B800,
// operator+(const CWideString&, const wchar_t*) 0x0099BF30 (__fastcall, hidden result), operator+= 0x0099B8D0,
// ~CWideString 0x0099B510, the copy ctor 0x0099B720 into the hidden result.
// Layout: C2DBoxF {TopLeft (X +0, Y +4), BottomRight (X +8, Y +0xC)}; its inline GetWidth() reads the left edge into a
// local first, which is why TopLeft.X is loaded first and `fsub st(1)` leaves it on the x87 stack (popped after
// __ftol2) -- the direct `BottomRight.X - TopLeft.X` folds into `fsub [ebx]`.
// Shape: the line pointer stays in edi, the line end in esi; the per-line `next` out pointer is zeroed each pass and,
// like the per-line string, lives in a dead argument slot (box and text are held in registers by then); the newline
// sum is a temporary (frame word) appended and destroyed per line; VC7.1 has no NRVO, so the result is copied out.
typedef unsigned short wchar_t;

struct C2DVector { float X, Y; };
struct C2DBoxF {
    C2DVector TopLeft;                                  // +0x00
    C2DVector BottomRight;                              // +0x08
    float GetWidth() const { float left = TopLeft.X; float right = BottomRight.X; return right - left; }
};
struct CFontBank { };

struct CWideString {
    void* PStringData;
    CWideString();                                      // 0x0099AED0
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    operator const wchar_t*() const;                    // 0x0099AF10
    void SetAsSubString(const wchar_t* text, long length);   // 0x0099B770
    const CWideString& operator=(const wchar_t* text);  // 0x0099B800
    const CWideString& operator+=(const CWideString& other); // 0x0099B8D0
};
CWideString __fastcall operator+(const CWideString& left, const wchar_t* right);   // 0x0099BF30

const wchar_t* __fastcall WordWrap_FindNextLineW(const wchar_t* text, unsigned int width, const wchar_t** next);   // 0x00A10390

static const wchar_t s_newLine[] = { '\n', 0 };          // 0x01231728

struct CProgressDisplay {
    CWideString WordWrapText(const C2DBoxF& box, const CFontBank& font, const CWideString& text) const;
};

CWideString CProgressDisplay::WordWrapText(const C2DBoxF& box, const CFontBank& font, const CWideString& text) const
{
    (void)font;
    CWideString wrapped;
    const wchar_t* line = text;
    const wchar_t* end;
    do {
        const wchar_t* next = 0;
        end = WordWrap_FindNextLineW(line, (unsigned int)box.GetWidth(), &next);
        CWideString piece;
        if (end)
            piece.SetAsSubString(line, end - line);
        else
            piece = line;
        wrapped += piece + s_newLine;
        line = end;
    } while (end);
    return wrapped;
}
