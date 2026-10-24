// FormatTextForVWindow(const C2DBoxF& box, const CFontBank& font, const CWideString& text, CWideString& outText, C2DBoxF& outBox) -- word-wrap a subtitle into the window box: the wrapped text and the unchanged box; always returns false
// Leaf: ?FormatTextForVWindow@@YI_NABVC2DBoxF@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BC9D0, 71 bytes (+ int3 padding), __fastcall (box in ecx, font in
// edx, text / outText / outBox on the stack, `ret 0Ch`).
// Name: Ego_r.pdb `?FormatTextForVWindow@@YI_NABVC2DBoxF@@ABVCFontBank@@ABVCWideString@@AAV3@AAV1@@Z` (free twin of
// the CProgressDisplay / CSubtitleRenderer members). Caller: the subtitle update at 0x009BD910 (0x009BD9E5).
// Callees: WordWrapText(const C2DBoxF&, const CFontBank&, const CWideString&) @ 0x009BC900 (landed, __fastcall with
// the hidden result in ecx), CWideString::operator=(const CWideString&) @ 0x0099B7D0, ~CWideString @ 0x0099B510.
// Shape: the wrapped-text temporary is built in the dead `text` argument slot (`lea ecx,[esp+10h]`), assigned and
// destroyed; the box copy is C2DBoxF's implicit operator= (four dword moves through edx/eax); `return false` is
// `xor al,al`.

class C2DVector {
public:
    float X, Y;
};

class C2DBoxF {
public:
    C2DVector TopLeft;                                  // +0x00
    C2DVector BottomRight;                              // +0x08
};

class CFontBank;

class CWideString {
public:
    ~CWideString();                                     // 0x0099B510
    const CWideString& operator=(const CWideString& other);   // 0x0099B7D0
private:
    void* PStringData;
};

CWideString __fastcall WordWrapText(const C2DBoxF& box, const CFontBank& font, const CWideString& text);   // 0x009BC900

bool __fastcall FormatTextForVWindow(const C2DBoxF& box, const CFontBank& font, const CWideString& text, CWideString& outText, C2DBoxF& outBox)
{
    outText = WordWrapText(box, font, text);
    outBox = box;
    return false;
}