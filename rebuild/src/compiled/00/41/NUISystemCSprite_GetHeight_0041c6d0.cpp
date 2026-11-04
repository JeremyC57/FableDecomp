// NUISystem::CSprite::GetHeight() -- the sprite's height in whole units: the component Size.Y (+0xD0) converted to unsigned long
// Leaf: ?GetHeight@CSprite@NUISystem@@UAEKXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0041C6D0, 11 bytes: `fld dword ptr [ecx+0D0h] ; jmp __ftol2` (0x00BFEA70).
// Identity: CSprite vtable 0x0122F5D4 slot 148 (catalog "vfunc_148"), the last slot; Ego_r.pdb / FableWin.pdb
//   ?GetHeight@CSprite@NUISystem@@UAEKXZ (the FableWin donor emits it right after GetWidth). See 0x0041C6C0 for the slot proof.
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