// NUISystem::CText::FitInWindow(CWideString& text, const C2DBoxF& window) -- word-wrap the text to the window width line by line, rebuilding it with explicit newlines, and return the wrapped height in font lines (one font height when the window is empty)
// Leaf: ?FitInWindow@CText@NUISystem
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0054F8E0, length 560 bytes, `ret 8`.
//   FableWin.pdb: ?FitInWindow@CText@NUISystem@@IAEJAAVCWideString@@ABVC2DBoxF@@@Z.
// Callees: CWideString ctor() 0x0099AED0 (catalog "CCharString" is a byte twin), operator const unsigned short*
//          0x0099AF10, Contains 0x0099AF20, FindCharPos 0x0099B070, GetLength 0x0099B220, ~CWideString 0x0099B510,
//          ctor(const unsigned short*) 0x0099B6B0, Assign(text, length) 0x0099B770 (catalog "CCharString_Replace"),
//          operator=(const CWideString&) 0x0099B7D0, operator=(const unsigned short*) 0x0099B800, operator+= 0x0099B8D0,
//          operator+(const CWideString&, const unsigned short*) 0x0099BF30 (__fastcall, hidden result first);
//          the word-wrap helpers SetOption 0x00A0FDB0, SetCallback 0x00A0FD90, FindNextLineW 0x00A10390 (__fastcall),
//          __ftol2 0x00BFEA70; the width callback NUISystem::CText measure function 0x0054F4B0.
// Strings: L"\n" 0x01231728; 0.0f 0x0122DEDC.
// Layout: retail NUISystem::CText = Ego_r.pdb minus 0x30 (landed ctor 0x0054F5C0): PFont (CCountedPointer<CFontBank>)
//         +0x160; the font bank keeps its line height at +4.
// Shape: the per-line width is `(long)(window.BRX - left)` with `float left = window.TLX;` read first in the loop body:
//        retail loads TLX, keeps it on the x87 stack through __ftol2 and drops it with `fstp st(0)`; the same named
//        local also restores the inner newline loop's head alignment. Exits are written out in full.
struct CWideString {
    void* PStringData;
    CWideString();                                          // 0x0099AED0
    CWideString(const unsigned short* text);                // 0x0099B6B0
    ~CWideString();                                         // 0x0099B510
    operator const unsigned short*() const;                 // 0x0099AF10
    bool Contains(const CWideString& text) const;           // 0x0099AF20
    long FindCharPos(unsigned short c) const;               // 0x0099B070
    long GetLength() const;                                 // 0x0099B220
    void Assign(const unsigned short* text, long length);   // 0x0099B770
    CWideString& operator=(const CWideString& other);       // 0x0099B7D0
    CWideString& operator=(const unsigned short* text);     // 0x0099B800
    CWideString& operator+=(const CWideString& other);      // 0x0099B8D0
};
CWideString __fastcall operator+(const CWideString& left, const unsigned short* right);   // 0x0099BF30

typedef long (__fastcall* TWordWrapWidthCallback)(const unsigned short* text, long length);
void __fastcall WordWrap_SetOption(long option, long value);                            // 0x00A0FDB0
void __fastcall WordWrap_SetCallback(TWordWrapWidthCallback callback, long user);       // 0x00A0FD90
const unsigned short* __fastcall WordWrap_FindNextLineW(const unsigned short* text, long width, long* lineWidth);   // 0x00A10390

struct C2DBoxF { float TLX, TLY, BRX, BRY; };
struct CCPPointerInfo;
struct CFontBank {
    void* _vf;
    long Height;                                            // +0x04
};
template <class T> struct CCountedPointer {
    T* Data;
    CCPPointerInfo* Info;
    T* operator->() const { return Data; }
};

namespace NUISystem {
struct CComponentInterface {
    virtual void v000();
};
struct CDrawableInterface {
    virtual void v000();
    unsigned char _pad04[0x10];
};
struct CDraggableInterface {
    virtual void v000();
};
struct CDraggableInto : CComponentInterface, CDrawableInterface, CDraggableInterface {
    unsigned char _pad1c[0x15c - 0x1c];
};
struct CText : CDraggableInto {
    CWideString            Value;                  // +0x15c
    CCountedPointer<CFontBank> PFont;              // +0x160
    virtual void v000();
    static long __fastcall MeasureText(const unsigned short* text, long length);   // 0x0054F4B0
    long FitInWindow(CWideString& text, const C2DBoxF& window);
};

long CText::FitInWindow(CWideString& text, const C2DBoxF& window)
{
    long height = 0;
    CWideString result;
    const unsigned short* start = text;
    if (window.BRX - window.TLX <= 0.0f && window.BRY - window.TLY <= 0.0f)
        return PFont->Height;
    WordWrap_SetOption(0, 1);
    WordWrap_SetCallback(MeasureText, 0);
    const unsigned short* next;
    do {
        float left = window.TLX;
        long lineWidth = 0;
        next = WordWrap_FindNextLineW(start, (long)(window.BRX - left), &lineWidth);
        CWideString line;
        if (next)
            line.Assign(start, next - start);
        else
            line = start;
        if (!line.Contains(L"\n") && next)
            result += line + L"\n";
        else
            result += line;
        if (line.Contains(L"\n")) {
            long pos = line.FindCharPos(L'\n');
            for (long i = 0; i < line.GetLength() - pos - 1; ++i)
                height += PFont->Height;
        }
        height += PFont->Height;
        start = next;
    } while (next);
    text = result;
    return height;
}
}
