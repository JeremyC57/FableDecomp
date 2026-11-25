// Behaviour test for 0x00497A60 CProgressDisplay::WordWrapText: wraps the text line by line (WordWrap_FindNextLineW), appending L"\n" (retail 0x01231728)
// after every line.
// NF420 (2026-11-25): LINKED test -- it links the row object (the row function is declared here, not defined); the CWideString callees
// are defined under the row's own spellings over a small wide-string model, so the test asserts the wrapped text and the literal
// the row appends (L"\n"); a mutated literal fails.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
extern "C" void __cdecl NF420TrapHit(const char* sym) { printf("FAIL unexpected call %s\n", sym); exit(3); }
extern "C" char NF420TrapData[4096] = { 0 };
// ---- row declarations (copied from the row source; its function definition left out) ----
// CProgressDisplay::WordWrapText(const C2DBoxF& box, const CFontBank& font, const CWideString& text) const -- break the text into lines that fit the box width, each line followed by a newline
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00497A60, length 202 bytes (+ int3 padding), `ret 10h` (hidden result + 3).
// Identity: Ego_r.pdb `?WordWrapText@CProgressDisplay@@ABE?AVCWideString@@ABVC2DBoxF@@ABVCFontBank@@ABV2@@Z` (the free
// __fastcall twin `?WordWrapText@@YI...` is 0x009BC900); the font is not read.
// Callees: CWideString default ctor 0x0099AED0, GetAsCharArray() 0x0099AF10, __ftol2 0x00BFEA70,
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

class C2DVector { public: float X, Y; };
class C2DBoxF { public:
    C2DVector TopLeft;                                  // +0x00
    C2DVector BottomRight;                              // +0x08
    float GetWidth() const { float left = TopLeft.X; float right = BottomRight.X; return right - left; }
};
class CFontBank { public: };

class CWideString { public:
    void* PStringData;
    CWideString();                                      // 0x0099AED0
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    const wchar_t* GetAsCharArray() const;                    // 0x0099AF10
    void SetAsSubString(const wchar_t* text, long length);   // 0x0099B770
    const CWideString& operator=(const wchar_t* text);  // 0x0099B800
    const CWideString& operator+=(const CWideString& other); // 0x0099B8D0
};
CWideString __fastcall operator+(const CWideString& left, const wchar_t* right);   // 0x0099BF30

const wchar_t* __fastcall WordWrap_FindNextLineW(const wchar_t* text, unsigned int width, const wchar_t** next);   // 0x00A10390


struct CProgressDisplay {
    private:
    CWideString WordWrapText(const C2DBoxF& box, const CFontBank& font, const CWideString& text) const;
    public:
};

// ---- end of row declarations ----
static int Len(const unsigned short* s) { int n = 0; while (s[n]) ++n; return n; }
static unsigned short* Dup(const unsigned short* s, int n) { unsigned short* d = (unsigned short*)malloc((n + 1) * 2); memcpy(d, s, n * 2); d[n] = 0; return d; }
static unsigned short* Cat(const unsigned short* a, const unsigned short* b) { int x = Len(a), y = Len(b); unsigned short* d = (unsigned short*)malloc((x + y + 1) * 2); memcpy(d, a, x * 2); memcpy(d + x, b, y * 2); d[x + y] = 0; return d; }
static const unsigned short kEmpty[] = { 0 };
static int g_live = 0, g_appendNewline = 0, g_appendOther = 0; static unsigned int g_width = 0;
CWideString::CWideString() { PStringData = Dup(kEmpty, 0); ++g_live; }
CWideString::CWideString(const CWideString& other) { PStringData = Dup((unsigned short*)other.PStringData, Len((unsigned short*)other.PStringData)); ++g_live; }
CWideString::~CWideString() { free(PStringData); --g_live; }
const wchar_t* CWideString::GetAsCharArray() const { return (const wchar_t*)PStringData; }
void CWideString::SetAsSubString(const wchar_t* text, long length) { free(PStringData); PStringData = Dup(text, length); }
const CWideString& CWideString::operator=(const wchar_t* text) { free(PStringData); PStringData = Dup(text, Len(text)); return *this; }
const CWideString& CWideString::operator+=(const CWideString& other) { unsigned short* d = Cat((unsigned short*)PStringData, (unsigned short*)other.PStringData); free(PStringData); PStringData = d; return *this; }
CWideString __fastcall operator+(const CWideString& left, const wchar_t* right)
{
    if (right[0] == '\n' && right[1] == 0) ++g_appendNewline; else ++g_appendOther;
    CWideString r(left); unsigned short* d = Cat((unsigned short*)r.PStringData, right); free(r.PStringData); r.PStringData = d; return r;
}
// a line ends at the next space after its first character (none: the rest is the last line)
const wchar_t* __fastcall WordWrap_FindNextLineW(const wchar_t* text, unsigned int width, const wchar_t** next)
{ g_width = width; for (const wchar_t* p = text + 1; *p; ++p) if (*p == ' ') { *next = p; return p; } return 0; }
extern "C" CWideString* __fastcall NF420_Row(const CProgressDisplay* self, int edx, CWideString* ret, const C2DBoxF& box, const CFontBank& font, const CWideString& text);
#pragma comment(linker, "/alternatename:@NF420_Row@24=?WordWrapText@CProgressDisplay@@ABE?AVCWideString@@ABVC2DBoxF@@ABVCFontBank@@ABV2@@Z")   // the row function, called through the shim
// Row-object externals this test does not model (generated): a call reaches a trap that prints FAIL and exits; a data
// symbol reads a zeroed buffer.
// (none)
static double g_result[2];
int main()
{
    C2DBoxF box; box.TopLeft.X = 10.0f; box.TopLeft.Y = 0.0f; box.BottomRight.X = 50.0f; box.BottomRight.Y = 5.0f;
    CFontBank font;
    static const unsigned short kText[] = { 'a', 'b', ' ', 'c', 'd', 0 };
    CWideString input; input = kText;
    static double raw[2]; CWideString* res = NF420_Row((const CProgressDisplay*)raw, 0, (CWideString*)&g_result, box, font, input);
    const unsigned short* out = (const unsigned short*)res->PStringData;
    static const unsigned short kWant[] = { 'a', 'b', '\n', ' ', 'c', 'd', '\n', 0 };
    bool ok = res == (CWideString*)&g_result && Len(out) == 7 && memcmp(out, kWant, 16) == 0 && g_appendNewline == 2 && g_appendOther == 0 && g_width == 40;
    res->~CWideString();
    ok = ok && g_live == 1;
    if (ok) printf("00497a60_TEST PASS\n");
    else printf("FAIL len=%d newline=%d other=%d width=%u live=%d\n", Len(out), g_appendNewline, g_appendOther, g_width, g_live);
    return ok ? 0 : 1;
}
