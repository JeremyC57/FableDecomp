// NUISystem::CText::GetFont() -- returns the counted font-bank pointer (PFont, +0x160) by reference
// Leaf: ?GetFont@CText@NUISystem@@UAEAAV?$CCountedPointer@VCFontBank@@@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0054FF20, 7 bytes: `lea eax,[ecx+160h] ; ret` (thiscall, no callees).
// Identity (two sources): CText vtable 0x01249CCC slot 146 (.rdata 0x01249F14, the only reference) holds this body; Ego_r.pdb (0x005CA100)
//   and FableWin.pdb (0x0228AD30) name ?GetFont@CText@NUISystem@@UAEAAV?$CCountedPointer@VCFontBank@@@@XZ, the fifth of the donor-order
//   set at slots 142..150; it returns the CText member PFont (+0x160, engine/CText.h FABLE_RETAIL_CTEXT_LAYOUT).
// Was: the gapscan placeholder T::GetAddr (rebuild/src/compiled/00/54/gapscan_sub0054ff20_0054ff20.cpp).
// Header: engine/CText.h (FABLE_RETAIL_CTEXT + _LAYOUT).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CTEXT
#define FABLE_RETAIL_CTEXT_LAYOUT
#include "engine/CText.h"
namespace NUISystem {
CCountedPointer<CFontBank>& CText::GetFont()
{
    return PFont;
}
}
