// NUISystem::CSprite::GetGraphicIndex() -- the sprite graphic index (GraphicIndex +0x178)
// Leaf: ?GetGraphicIndex@CSprite@NUISystem@@UAEKXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0041C6B0, 7 bytes: `mov eax,[ecx+178h] ; ret`.
// NF104 (q4927, 2026-11-19): moved off its gapscan `T::Set` / `T::Get` pad stand-in onto the shared engine/NUISystem_CSprite.h (the inline
//   virtual, slot 146). Identity: CSprite vtable 0x0122F5D4 slot 146 (slot order 142..148 = SetAngle, SetWidth, SetHeight,
//   SetGraphicIndex, GetGraphicIndex, GetWidth, GetHeight; the header's offset note); name Ego_r.pdb 0x0042D3B0 + FableWin.pdb 0x01D2CD30 ?GetGraphicIndex@CSprite@NUISystem@@UAEKXZ.
// TU: the ~CSprite translation unit, as the landed siblings 0x0041C6C0 GetWidth / 0x0041C6D0 GetHeight: defining the virtual destructor
//   makes VC7.1 emit CSprite's vtables here, and with them the inline virtuals of engine/NUISystem_CSprite.h (slots 142..148) as COMDATs;
//   retail places them together after the class's code (0x0041C680..0x0041C6F0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#include "engine/NUISystem_CSprite.h"
namespace NUISystem {
CSprite::~CSprite()
{
    Handle.Clear();
}
}
