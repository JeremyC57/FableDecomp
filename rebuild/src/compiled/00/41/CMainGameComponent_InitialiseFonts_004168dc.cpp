// CMainGameComponent::InitialiseFonts() -- show the "Init Fonts" progress text, then fetch the ENG_ARIAL_18 font from the font bank into the component's font pointer (+0x1614c)
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004168DC, length 119 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, DisplayProgress @ 0x009E9F40 (`__fastcall`:
//          string in ecx, the first bool in dl, the float and the second bool on the stack), CCharString::~CCharString
//          @ 0x0099EAE0, the font bank's GetFont(const CCharString&) @ 0x009E2C80 (bank at 0x013B838C, a counted
//          pointer returned by hidden pointer), CCountedPointer<CFont>::operator= @ 0x00419463,
//          CCountedPointer<CFont>::~CCountedPointer @ 0x004057A0.
// Shape: size TU with a frame pointer (`optimize("s", on)` + `("y", off)`: `leave`, the -1.0f via `fld ; push ecx ;
// fstp [esp]`); both strings are named locals sharing [ebp-4] in their
// own blocks; the returned pointer is a temporary destroyed after the assignment.
#pragma optimize("s", on)
#pragma optimize("y", off)
class CCharString {
public:
    CCharString(const char* text, long length);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
private:
    void* m_pData;
};
void __fastcall DisplayProgress(const CCharString& text, bool flag, float fraction, bool flag2);   // 0x009E9F40
struct CFont;
#include "engine/CCountedPointer.h"
class CFontBank { public:
    CCountedPointer<CFont> GetFont(const CCharString& name);   // 0x009E2C80
};
class CFontManager;
extern CFontManager* PFontManager;                     // 0x013B838C
class CMainGameComponent { public:
    unsigned char          _pad00[0x1614c];
    CCountedPointer<CFont> m_font;                 // +0x1614c
    void InitialiseFonts();
};

void CMainGameComponent::InitialiseFonts()
{
    {
        CCharString text("Init Fonts", -1);
        DisplayProgress(text, false, -1.0f, false);
    }
    {
        CCharString name("ENG_ARIAL_18", -1);
        m_font = ((CFontBank*)PFontManager)->GetFont(name);
    }
}