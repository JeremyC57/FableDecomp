// NEnvironmentInternal::CBlendedEnvironmentTheme::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor of a
// non-polymorphic class: ~CBlendedEnvironmentTheme (0x006B5D80), then operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCBlendedEnvironmentTheme@NEnvironmentInternal@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B6340, 30 bytes, thiscall, `ret 4`; unreferenced (no call or pointer to it in Fable.exe).
// NF39 (2026-11-16): moved off its `T::VecDel` / `T::Dtor_006b5d80` stand-in (family len30 x22, propagated label "CLandscapeBackgroundPatch::`vector
//   deleting destructor'") onto the shared engine/CBlendedEnvironmentTheme.h. Identity: the only callee is 0x006B5D80, which both PDB name sets name
//   ??1CBlendedEnvironmentTheme@NEnvironmentInternal@@QAE@XZ (and the landed atexit stub 0x01229390 destroys interpolated_theme through it); a non-virtual
//   class's `delete p` emits the Q-access ??_G as its own COMDAT, as for the landed G042 family (0x0075F0B0 CKeyCamera).
// Callees: NEnvironmentInternal::CBlendedEnvironmentTheme::~CBlendedEnvironmentTheme() 0x006B5D80, operator delete 0x00BFE9BC.
// Shape: `delete p` on the class emits the non-virtual ??_G as its own COMDAT (the helper below is not graded).
#include "engine/CBlendedEnvironmentTheme.h"

void Kill_006b6340(NEnvironmentInternal::CBlendedEnvironmentTheme* object)   // forces the scalar deleting destructor into the object; not part of the retail body
{
    delete object;
}
