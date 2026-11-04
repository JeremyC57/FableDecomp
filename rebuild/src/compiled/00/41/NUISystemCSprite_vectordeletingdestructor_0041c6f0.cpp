// [thunk] NUISystem::CSprite::`vector deleting destructor' `adjustor{24}'(unsigned int) -- CActionParam-subobject (+0x18) vtable entry: this -= 0x18, jump to the deleting destructor 0x0041C760
// Leaf: ??_ECSprite@NUISystem@@WBI@AEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0041C6F0, 8 bytes: `sub ecx,18h ; jmp 0x0041C760`. Slot 0 of CSprite's CActionParam vtable
//   0x0122F5A4 (rtti-vtables.tsv; the catalog's "vfunc_0"). Compiler-generated adjustor thunk, ??_E alias of ??_G 0x0041C760.
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