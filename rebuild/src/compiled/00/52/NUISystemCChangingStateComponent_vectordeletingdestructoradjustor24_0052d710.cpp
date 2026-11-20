// NUISystem::CChangingStateComponent::`vector deleting destructor'(unsigned int) `adjustor{24}' -- CActionParam-subobject (+0x18) vtable
//   entry: this -= 0x18, jump to the deleting destructor 0x0052D770
// Leaf: ??_ECChangingStateComponent@NUISystem@@WBI@AEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052D710, 8 bytes: `sub ecx,18h ; jmp 0x0052D770`. Compiler-generated adjustor thunk.
// Identity (two sources): slot 0 of CChangingStateComponent's CActionParam vtable 0x01245DB4 at +0x18 (its only reference; RTTI .?AVCChangingStateComponent@NUISystem@@),
//   and its jump target 0x0052D770 is slot 0 of the primary vtable 0x01245DE4, the class's ??_G (it stores the three vptrs and destroys
//   the members); VC7.1 spells the +0x18 thunk ??_E...W BI@ (adjustor 24), as objdump -t of this object shows. Twin of the matched +4
//   thunk 0x0052D700 (??_ECChangingStateComponent@NUISystem@@W3AEPAXI@Z).
// Was: the placeholder CMulti_0052d770::Override_0052d770 adjustor (local CFirstBase / CSecondBase / CMulti_0052d770 model,
//   rebuild/src/compiled/00/52/unattributed_Override0052d770_0052d710.cpp).
// Callee: ??_ECChangingStateComponent@NUISystem@@UAEPAXI@Z 0x0052D770 (the weak ??_E alias of the ??_G), as 0x0052D700 binds it.
// TU: as 0x0052D700: the destructor's TU, ~CChangingStateComponent (0x0052CCF0, an empty user body; its own row, not graded here).
// Header: engine/NUISystem_CChangingStateComponent.h.
#include "engine/NUISystem_CChangingStateComponent.h"
namespace NUISystem {
CChangingStateComponent::~CChangingStateComponent()
{
}
}
