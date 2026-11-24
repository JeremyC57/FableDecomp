// Leaf: ?InitialiseFonts@CMainGameComponent@@IAEXXZ
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
// Name fix (agentNF6 2026-11-10): NProgressDisplay::DisplayProgress 0x009E9F40 is ?DisplayProgress@NProgressDisplay@@YIXABVCCharString@@M_N1@Z (Ego_r.pdb / FableWin.pdb; namespace NProgressDisplay, (text, float, bool, bool)); the float takes the stack slot either way under __fastcall, so the call bytes are unchanged.
#pragma optimize("s", on)
#pragma optimize("y", off)
class CCharString {
public:
    CCharString(const char* text, long length);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
private:
    void* m_pData;
};
namespace NProgressDisplay { void __fastcall DisplayProgress(const CCharString& text, float fraction, bool flag, bool flag2); }   // 0x009E9F40
// NF221 (family q12226 / q12402): +0x1614c is Ego_r.pdb PCutSceneMainFont, a CCountedPointer<CFontBank> (was CCountedPointer<CFont>: no
//   CCountedPointer<CFont> exists in any of the three PDBs), filled from CFontManager::GetFont (engine/CFontManager.h; FableWin.pdb /
//   Ego_r.pdb ?GetFont@CFontManager@@QAE?AV?$CCountedPointer@VCFontBank@@@@ABVCCharString@@@Z, the self leaf of 0x009E2C80; was a local
//   CFontBank::GetFont stand-in), assigned by the out-of-line ??4<CFontBank> 0x00419463 (ASSIGN_DECL_ONLY) and the temporary released
//   by ReleaseData<CFontBank> 0x004057A0 (RELEASEDATA_DECL_ONLY; FableWin.pdb names, their own rows).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#define FABLE_RETAIL_CCOUNTEDPOINTER_ASSIGN_DECL_ONLY
#include "engine/CCountedPointer.h"
#include "engine/CFontManager.h"
class CFontBank;
extern CFontManager* PFontManager;                     // 0x013B838C
class CMainGameComponent { public:
    unsigned char          _pad00[0x1614c];
    CCountedPointer<CFontBank> PCutSceneMainFont;  // +0x1614c
    protected:
    void InitialiseFonts();
    public:
};

void CMainGameComponent::InitialiseFonts()
{
    {
        CCharString text("Init Fonts", -1);
        NProgressDisplay::DisplayProgress(text, -1.0f, false, false);
    }
    {
        CCharString name("ENG_ARIAL_18", -1);
        PCutSceneMainFont = PFontManager->GetFont(name);
    }
}