// NUISystem::CText::GetTextWindow() -- returns the box the text is laid out in (Window, +0x178) by reference
// Leaf: ?GetTextWindow@CText@NUISystem@@UAEAAVC2DBoxF@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0054FFE0, 7 bytes: `lea eax,[ecx+178h] ; ret` (thiscall, no callees).
// Identity (two sources): CText vtable 0x01249CCC slot 148 (.rdata 0x01249F1C, the only reference) holds this body; Ego_r.pdb (0x005CA160)
//   and FableWin.pdb (0x0228AE70) name ?GetTextWindow@CText@NUISystem@@UAEAAVC2DBoxF@@XZ, next to SetTextWindow (slot 147, landed 0x0054FFB0,
//   which stores the same member); it returns the CText member Window (+0x178, engine/CText.h FABLE_RETAIL_CTEXT_LAYOUT).
// Was: the gapscan placeholder T::GetAddr (rebuild/src/compiled/00/54/gapscan_sub0054ffe0_0054ffe0.cpp).
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
C2DBoxF& CText::GetTextWindow()
{
    return Window;
}
}
