// NUISystem::CSprite::GetWidth() -- the sprite's width in whole units: the component Size.X (+0xCC) converted to unsigned long
// Leaf: ?GetWidth@CSprite@NUISystem@@UAEKXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0041C6C0, 11 bytes: `fld dword ptr [ecx+0CCh] ; jmp __ftol2` (the float-to-integer conversion
//   tail-calls the CRT helper 0x00BFEA70; the result is the low dword).
// Identity: CSprite vtable 0x0122F5D4 slot 147 (catalog "vfunc_147"); Ego_r.pdb / FableWin.pdb ?GetWidth@CSprite@NUISystem@@UAEKXZ.
//   Slot order 142..148 = SetAngle, SetWidth, SetHeight, SetGraphicIndex, GetGraphicIndex, GetWidth, GetHeight: the bodies at
//   0x0041C680.. store +0x164 / +0x168 / +0x16C (Angle, Width, Height = Ego_r +0x194.. minus the 0x30 base shrink), 0x0041B550 stores
//   +0x178 (GraphicIndex) and reloads the texture, 0x0041C6B0 returns +0x178; FableWin.pdb emits the inline ones in exactly that
//   order (0x01D2CCD0 SetAngle .. 0x01D2CD90 GetHeight). Size (+0xCC) is CComponent's C2DVector (engine/NUISystem_CComponent.h).
//
// TU: the ~CSprite translation unit (landed body 0x0041AB40: `Handle.Clear();`, then the member destructors). Defining the virtual
// destructor makes VC7.1 emit CSprite's vtables here, and with them the ??_G 0x0041C760, both deleting-destructor adjustor thunks
// and the inline virtuals of engine/NUISystem_CSprite.h (SetAngle .. GetHeight, slots 142..148) as COMDATs; retail places them
// together after the class's code (0x0041C680..0x0041C6F0). Precedent: the CText / CMesh / CTextSlider thunks (chunk 749).
// Header: engine/NUISystem_CSprite.h (new, chunk 753).
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