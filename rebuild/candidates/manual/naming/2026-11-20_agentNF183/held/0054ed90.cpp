// NUISystem::CText::GetDataFromDef() -- read the text component's UI def: the font bank named by the def's font string, the text window, the scale-text flag, the def's text value (with its line-break flag), and the alignment bit
// Leaf: GetDataFromDef@CText
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCPPointerInfo members References->RefCount, Destroy->DeleteFunc, Object->Data are the header CCPPointerInfo's (same offsets). Spellings unified: 009e2c80: ?GetFont@CFontBankSource@NUISystem@@QAE?AU?$CCountedPointer@UCFontBank@NUISystem@@@2@ABVCCharString@@@Z -> ?GetFont@CFontBankSource@NUISystem@@QAE?AV?$CCountedPointer@UCFontBank@NUISystem@@@2@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0054ED90, length 365 bytes (plain `ret`, no frame pointer; Ego_r.pdb
//   `?GetDataFromDef@CText@NUISystem@@QAEXXZ`). Called by the CText constructor 0x0054F5C0 (landed).
// Callees: CComponent::LoadAndReturnDef() const (vtable slot 108, +0x1B0; CDefPointer<const CUIDef> by hidden pointer, as
//   the landed CTextContainer::GetDataFromDef 0x00546CD0); CDefStringTable::GetString(long) 0x009D49B0 on
//   GDefStringTable 0x013CA828 (as landed); GFGetSystemManager 0x009A4EC0 (__fastcall; its +0x84 font bank, as the landed
//   InitialiseGraphics 0x0042DDB3); the bank's GetFont(const CCharString&) 0x009E2C80 (landed label; returns the counted
//   pointer by hidden pointer); the count block's destroy function (+4, object in ecx) and operator delete 0x00BFE9BC on
//   the last release; ~CCharString 0x0099EAE0; CText::SetValue(const CWideString&, bool, bool) vtable slot 144 (+0x240);
//   the def's virtual slot 1 (destroy) on the last release.
// Layout: retail CText (the landed constructor: Ego_r.pdb minus 0x30): PFont +0x160 (Data) / +0x164 (Info), Window
//   +0x178, ScaleText +0x188; CComponent FlagsSetTwo +0x12E (bits 3/4/5 = left/centre/right alignment, as the landed
//   vfunc_170..172 0x0053CE32..50). Retail CUIDef (PDB minus 4 for the def base, minus 4 per STLport vector/map before
//   the field): Font (CDefString) +0x50, TextValue +0x54, TextWindow +0xA4, TextLineBreak +0xBD, ScaleText +0xBE,
//   Alignement +0x1FC.
// Shape: the font name comes through an inline CDefString accessor returning GetString's result by value (a direct
//   GetString temporary pushes its address before the system-manager call; a named local loses the temporary's
//   clearing stores); the bank through an inline CSystemManager getter (eax, then `mov ecx, eax`); the font is assigned
//   with CCountedPointer's inline operator= (the source words read into ebx/edi first, count-block compare, release
//   re-reading the member, add-ref) and the temporary's release clears both words; Window is a struct copy; the
//   alignment switch ORs one bit per case into the flag byte.
// Naming (NF183, 2026-11-20, q4288 / q4442): moved off the local models (CCharString, NUISystem::CSystemManager / CFontBankSource /
//   CFontBank / CUIDef / CText stand-ins) onto the shared headers, as its landed twin CIconText::GetDataFromDef 0x00541FD0: 0x009A4EC0
//   is GFGetSystemManager (?GFGetSystemManager@@YIPAVCSystemManager@@XZ, Ego_r.pdb + FableWin.pdb) and +0x84 its inline
//   GetFontManager(); 0x009E2C80 is CFontManager::GetFont (?GetFont@CFontManager@@QAE?AV?$CCountedPointer@VCFontBank@@@@ABVCCharString@@@Z,
//   Ego_r.pdb + FableWin.pdb, engine/CFontManager.h). Header opt-in FABLE_RETAIL_CTEXT_GETDATAFROMDEF (NF183 overlay) declares the member.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CSYSTEMMANAGER_FONTMANAGER
#define FABLE_RETAIL_CTEXT_VSLOTS
#define FABLE_RETAIL_CTEXT_GETDATAFROMDEF
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_CUIDef.h"
#include "engine/CDefPointer.h"
#include "engine/CSystemManager.h"
#include "engine/CFontManager.h"
namespace NUISystem {
void CText::GetDataFromDef()
{
    CDefPointer<const CUIDef> def = LoadAndReturnDef();
    PFont = GFGetSystemManager()->GetFontManager()->GetFont(def->Font.GetString());
    Window = def->TextWindow;
    ScaleText = def->ScaleText;
    SetValue(def->TextValue, true, def->TextLineBreak);
    switch (def->Alignement) {
    case LEFT:
        FlagsSetTwo |= 0x08;
        break;
    case CENTER:
        FlagsSetTwo |= 0x10;
        break;
    case RIGHT:
        FlagsSetTwo |= 0x20;
        break;
    }
}
}
