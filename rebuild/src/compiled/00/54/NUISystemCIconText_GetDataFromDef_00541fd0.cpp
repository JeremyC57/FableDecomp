// NUISystem::CIconText::GetDataFromDef() -- read the icon text's UI def: the font bank named by the def's font string, the text window, the position offset, the def's text value and the alignment bit
// Leaf: ?GetDataFromDef@CIconText@NUISystem@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00541FD0, length 366 bytes (0x00541FD0..0x0054213D, plain `ret`, no frame pointer,
//   no SEH frame). Ego_r.pdb 0x005BB900 / FableWin.pdb 0x02262EA0 ?GetDataFromDef@CIconText@NUISystem@@QAEXXZ. Called by the
//   CIconText(long) constructor 0x00542330 (0x005423CF) and the copy constructor (0x00542490).
// Callees: CComponent::LoadAndReturnDef() const (vtable slot 108, +0x1B0; hidden CDefPointer<const CUIDef> result);
//   CDefStringTable::GetString(long) const 0x009D49B0 on GDefStringTable 0x013CA828 (through the inline CDefString::GetString);
//   GFGetSystemManager 0x009A4EC0 (__fastcall) and its inline GetFontManager() (PFontManager +0x84);
//   CFontManager::GetFont(const CCharString&) 0x009E2C80 (hidden CCountedPointer<CFontBank> result); the count block's
//   DeleteFunc (+4) and operator delete 0x00BFE9BC on a last release; ~CCharString 0x0099EAE0;
//   CIconText::SetValue(const CWideString&) (vtable slot 144, +0x240, 0x00541F20); the def's Delete (vtable slot 1) on its last release.
// Layout (shared headers): CIconText PFont +0x164, Window +0x198, PositionOffset +0x1A8; CComponent FlagsSetTwo +0x12E (bits
//   3/4/5 = left/centre/right); retail CUIDef Font +0x50, TextValue +0x54, TextWindow +0xA4, PositionOffset +0x142 (packed),
//   Alignement +0x1FC.
// Shape: the twin of the landed CText::GetDataFromDef 0x0054ED90 -- the font is assigned in ONE full expression, so GetString's
//   temporary is built before GFGetSystemManager and the CCountedPointer temporary's release keeps its two clearing stores;
//   PFont's inline operator= reads the source words into ebx/edi first; Window is a 16-byte struct copy; the def release is
//   inline with no null test. No pragma, no extra flags.
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
#include "engine/NUISystem_CIconText.h"
#include "engine/NUISystem_CUIDef.h"
#include "engine/CDefPointer.h"
#include "engine/CSystemManager.h"
#include "engine/CFontManager.h"
namespace NUISystem {
void CIconText::GetDataFromDef()
{
    CDefPointer<const CUIDef> def = LoadAndReturnDef();
    PFont = GFGetSystemManager()->GetFontManager()->GetFont(def->Font.GetString());
    Window = def->TextWindow;
    PositionOffset = def->PositionOffset;
    SetValue(def->TextValue);
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