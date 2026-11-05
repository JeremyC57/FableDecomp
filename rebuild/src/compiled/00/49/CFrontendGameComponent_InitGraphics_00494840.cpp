// CFrontendGameComponent::InitGraphics() -- debug front end: take the ENG_ARIAL_16 font from the global font bank into MainFont; always succeeds
// Leaf: ?InitGraphics@CFrontendGameComponent@@AAE_NXZ (Ego_r.pdb, private; modelled public)
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCPPointerInfo members References->RefCount, Destroy->DeleteFunc, Object->Data are the header CCPPointerInfo's (same offsets).
// Retail: Fable.exe @ 0x00494840, 187 bytes, plain `ret`.
// Body: MainFont = g_pFontBank->GetFont("ENG_ARIAL_16") (implicit conversion: the temporary is addressed with lea) -- CCharString(const char*, long) 0x0099EBF0
// (literal 0x01231168), CFontBank::GetFont(const CCharString&) 0x009E2C80 (bank pointer 0x013B838C, CCountedPointer by
// hidden pointer); inline CCountedPointer operator= (source words into ebx/edi first, count-block compare, release
// re-reading the member, add-ref) and the temporary's inline release (destroy + operator delete 0x00BFE9BC, then both
// words cleared); ~CCharString 0x0099EAE0. Returns true.
// Layout: CFrontendGameComponent MainFont +0x1E04 (Data) / +0x1E08 (Info), as Ego_r.pdb (no container precedes it).
// Shape: the counted-pointer model of the landed CText::GetDataFromDef 0x0054ED90.
// Opt: /O2 (16-aligned entry).
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
#include "engine/CCountedPointer.h"
class CFontBank {
public:
    CCountedPointer<const CFontBank> GetFont(const CCharString& name);   // 0x009E2C80
};
class CFontManager;
extern CFontManager* PFontManager;                          // 0x013B838C

class CFrontendGameComponent {
public:
    unsigned char                    _pad0000[0x1e04];
    CCountedPointer<const CFontBank> MainFont;          // +0x1e04
    bool InitGraphics();
};

bool CFrontendGameComponent::InitGraphics()
{
    MainFont = ((CFontBank*)PFontManager)->GetFont("ENG_ARIAL_16");
    return true;
}