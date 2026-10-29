// NUISystem::CText::FitInWindow(CWideString& text, const C2DBoxF& window) -- word-wrap the text to the window width line by line, rebuilding it with explicit newlines, and return the wrapped height in font lines (one font height when the window is empty)
// Leaf: ?FitInWindow@CText@NUISystem
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0054F8E0, length 560 bytes, `ret 8`.
//   FableWin.pdb: ?FitInWindow@CText@NUISystem@@IAEJAAVCWideString@@ABVC2DBoxF@@@Z.
// Callees: CWideString ctor() 0x0099AED0 (catalog "CCharString" is a byte twin), GetAsCharArray()
//          0x0099AF10, Contains 0x0099AF20, FindCharPos 0x0099B070, GetLength 0x0099B220, ~CWideString 0x0099B510,
//          ctor(const unsigned short*) 0x0099B6B0, SetAsSubString(text, length) 0x0099B770 (PDB ?SetAsSubString@CWideString@@QAEXPB_WJ@Z; catalog "CCharString_Replace"),
//          operator=(const CWideString&) 0x0099B7D0, operator=(const unsigned short*) 0x0099B800, operator+= 0x0099B8D0,
//          operator+(const CWideString&, const unsigned short*) 0x0099BF30 (__fastcall, hidden result first);
//          the word-wrap helpers SetOption 0x00A0FDB0, SetCallback 0x00A0FD90, FindNextLineW 0x00A10390 (__fastcall),
//          __ftol2 0x00BFEA70; the width callback NUISystem::CText::GetCharWidthW 0x0054F4B0 (FableWin.pdb
//          ?WordWrap_SetCallback@@YIXP6II_W@ZP6IIXZ@Z, ?WordWrap_FindNextLineW@@YIPB_WPB_WIPAPB_W@Z,
//          ?GetCharWidthW@CText@NUISystem@@SII_W@Z; wchar_t modelled as unsigned short).
// Strings: L"\n" 0x01231728; 0.0f 0x0122DEDC.
// Layout: retail NUISystem::CText = Ego_r.pdb minus 0x30 (landed ctor 0x0054F5C0): PFont (CCountedPointer<CFontBank>)
//         +0x160; the font bank keeps its line height at +4.
// Shape: the per-line width is `(long)(window.BRX - left)` with `float left = window.TLX;` read first in the loop body:
//        retail loads TLX, keeps it on the x87 stack through __ftol2 and drops it with `fstp st(0)`; the same named
//        local also restores the inner newline loop's head alignment. Exits are written out in full.
class CWideString { public:
    void* PStringData;
    CWideString();                                          // 0x0099AED0
    CWideString(const unsigned short* text);                // 0x0099B6B0
    ~CWideString();                                         // 0x0099B510
    const unsigned short* GetAsCharArray() const;                 // 0x0099AF10
    bool Contains(const CWideString& text) const;           // 0x0099AF20
    long FindCharPos(unsigned short c) const;               // 0x0099B070
    long GetLength() const;                                 // 0x0099B220
    void SetAsSubString(const unsigned short* text, long length);   // 0x0099B770
    const CWideString& operator=(const CWideString& other);       // 0x0099B7D0
    const CWideString& operator=(const unsigned short* text);     // 0x0099B800
    const CWideString& operator+=(const CWideString& other);      // 0x0099B8D0
};
CWideString __fastcall operator+(const CWideString& left, const unsigned short* right);   // 0x0099BF30

typedef unsigned int (__fastcall* TWordWrapCharWidthCallback)(unsigned short c);
typedef unsigned int (__fastcall* TWordWrapCallback2)();
void __fastcall WordWrap_SetOption(long option, long value);                            // 0x00A0FDB0
void __fastcall WordWrap_SetCallback(TWordWrapCharWidthCallback charWidth, TWordWrapCallback2 callback2);   // 0x00A0FD90
const unsigned short* __fastcall WordWrap_FindNextLineW(const unsigned short* text, unsigned int width, const unsigned short** lineEnd);   // 0x00A10390

class C2DBoxF { public: float TLX, TLY, BRX, BRY; };
class CCPPointerInfo;
class CFontBank { public:
    void* _vf;
    long Height;                                            // +0x04
};
template <class T> class CCountedPointer { public:
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
class CDraggableInto : public CComponentInterface, public CDrawableInterface, public CDraggableInterface { public:
    unsigned char _pad1c[0x15c - 0x1c];
};
class CText : public CDraggableInto { public:
    CWideString            Value;                  // +0x15c
    CCountedPointer<CFontBank> PFont;              // +0x160
    virtual void v000();
    static unsigned int __fastcall GetCharWidthW(unsigned short c);   // 0x0054F4B0
    long FitInWindow(CWideString& text, const C2DBoxF& window);
};

long CText::FitInWindow(CWideString& text, const C2DBoxF& window)
{
    long height = 0;
    CWideString result;
    const unsigned short* start = text.GetAsCharArray();
    if (window.BRX - window.TLX <= 0.0f && window.BRY - window.TLY <= 0.0f)
        return PFont->Height;
    WordWrap_SetOption(0, 1);
    WordWrap_SetCallback(GetCharWidthW, 0);
    const unsigned short* next;
    do {
        float left = window.TLX;
        const unsigned short* lineEnd = 0;
        next = WordWrap_FindNextLineW(start, (long)(window.BRX - left), &lineEnd);
        CWideString line;
        if (next)
            line.SetAsSubString(start, next - start);
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