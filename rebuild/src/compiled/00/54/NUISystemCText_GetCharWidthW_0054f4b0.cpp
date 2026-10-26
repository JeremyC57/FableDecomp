// NUISystem::CText::GetCharWidthW(unsigned short c) -- the pixel width of one character in the ENG_ARIAL_16 font bank: format it as a one-character wide string, fetch the bank from the system manager's font manager and truncate the measured extent's X
// Leaf: ?GetCharWidthW@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0054F4B0, 200 bytes, plain `ret` (Ego_r.pdb `?GetCharWidthW@CText@NUISystem@@SII_W@Z`:
//   static __fastcall, the character in cx, unsigned result; wchar_t is modelled as `unsigned short`, no /Zc:wchar_t).
// Callees: CWideString() 0x0099AED0, FormatWideString(CWideString*, const wchar_t*, ...) 0x0099BA70 (__cdecl, L"%c"
//   0x01238154), CCharString(const char*, long = -1) 0x0099EBF0 ("ENG_ARIAL_16" 0x01231168), GFGetSystemManager
//   0x009A4EC0, CFontManager::GetFont(const CCharString&) 0x009E2C80 (counted pointer by hidden pointer), ~CCharString
//   0x0099EAE0, CWideString::operator const wchar_t* 0x0099AF10, the bank's slot 1 GetTextExtent (C2DVector by hidden
//   pointer, flags 0), __ftol2 0x00BFEA70, the count block's destroy (+4) and operator delete 0x00BFE9BC, ~CWideString
//   0x0099B510.
// Callers: NUISystem::CEditBox::KeyPressed 0x00540320 (call 0x00540541) and UpdateDisplayText 0x00540980 (0x00540BE1,
//   landed), CText::ForceWidth 0x0054FB10 (0x0054FB4D), CText::FitInWindow 0x0054F8E0 (the address at 0x0054F95D).
// Label: catalogued as "GetCharWidthW" (unscoped); the same static name exists on CMovieSubtitles, CProgressDisplay,
//   CSubtitleRenderer and CXMVPlayer, and the NUISystem callers fix it as CText's.
// Layout: retail CSystemManager PFontManager (CCountedPointer<CFontManager>, Ego_r.pdb +0x7c) at +0x84 (the landed
//   CIMEFont::GetTextExtent 0x00404F60 reads the same word); CCountedPointer { Data, Info }, count block { References,
//   Destroy, Object }.
// Shape: the name is the implicit `const char*` -> CCharString conversion temporary (its address pushed with `lea`, an
//   explicit `CCharString(...)` pushes the ctor's eax); the font manager comes from an inline pointer accessor on the
//   system manager, which puts GFGetSystemManager() after the temporary and before the pushes (a plain member read sinks
//   the call below the pushes, a counted-pointer operator-> adds `add eax,84h`); the bank pointer is a named local (esi,
//   the vtable in ebx); the width is computed before the counted pointer and the string are destroyed.
void __cdecl operator delete(void* block);                  // 0x00BFE9BC

class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CWideString {
public:
    void* PStringData;
    CWideString();                                          // 0x0099AED0
    ~CWideString();                                         // 0x0099B510
    operator const unsigned short*() const;                 // 0x0099AF10
};
void __cdecl FormatWideString(CWideString* text, const unsigned short* format, ...);   // 0x0099BA70

class CCPPointerInfo { public:
    long  References;                                       // +0x00
    void (__fastcall* Destroy)(void* object);               // +0x04
    void* Object;                                           // +0x08
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    void Release()
    {
        if (Info) {
            --Info->References;
            if (Info->References == 0) {
                Info->Destroy(Info->Object);
                operator delete(Info);
            }
        }
    }
    ~CCountedPointer() { Release(); Data = 0; Info = 0; }
    T* operator->() const { return Data; }
};

class C2DVector {
public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};

class CFontBank {
public:
    virtual void v00();
    virtual C2DVector GetTextExtent(const unsigned short* text, long flags);   // slot 1 (+4)
};
class CFontManager {
public:
    CCountedPointer<CFontBank> GetFont(const CCharString& name);   // 0x009E2C80
};
class CSystemManager {
public:
    unsigned char                 _pad00[0x84];
    CCountedPointer<CFontManager> PFontManager;             // +0x84
    CFontManager* GetFontManager() const { return PFontManager.Data; }
};
CSystemManager* __fastcall GFGetSystemManager();            // 0x009A4EC0

namespace NUISystem {
class CText {
public:
    static unsigned int __fastcall GetCharWidthW(unsigned short c);
};

unsigned int __fastcall CText::GetCharWidthW(unsigned short c)
{
    CWideString text;
    FormatWideString(&text, L"%c", c);
    CCountedPointer<CFontBank> font = GFGetSystemManager()->GetFontManager()->GetFont("ENG_ARIAL_16");
    CFontBank* bank = font.Data;
    return (unsigned int)bank->GetTextExtent(text, 0).X;
}
}