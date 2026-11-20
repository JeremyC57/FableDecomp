// NUISystem::CText::GetValue() -- returns the displayed text (Value, +0x15C) by reference
// Leaf: ?GetValue@CText@NUISystem@@UAEABVCWideString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0054FF10, 7 bytes: `lea eax,[ecx+15Ch] ; ret` (thiscall, no callees).
// Identity (two sources): CText vtable 0x01249CCC slot 142 (.rdata 0x01249F04, the only reference) holds this body; FableWin.pdb names
//   ?GetValue@CText@NUISystem@@UAEABVCWideString@@XZ (0x0228AD10, first of the donor-order set GetValue, SetValue x3, GetFont, SetTextWindow,
//   GetTextWindow, ForceWidth, GetAlignment = slots 142..150); it returns the CText member Value (+0x15C, engine/CText.h
//   FABLE_RETAIL_CTEXT_LAYOUT), which the landed CText::Draw 0x0054EF00 also reads.
// Was: the gapscan placeholder T::GetAddr (rebuild/src/compiled/00/54/gapscan_sub0054ff10_0054ff10.cpp).
// Header: engine/CText.h (FABLE_RETAIL_CTEXT + _LAYOUT; it declares the non-const GetValue FableWin.pdb names).
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
const CWideString& CText::GetValue()
{
    return Value;
}
}
