// CTexture::`scalar deleting destructor'(unsigned int flags) -- the inline destructor (?Uninitialise@CTexture@@QAEXXZ 0x009F9F70), then
//   operator delete on flag bit 0; returns this
// Leaf: ??_GCTexture@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0041BB00, 30 bytes, thiscall `ret 4`.
// Names (NF142, 2026-11-20, q4296 binder): FableWin.pdb ??_GCTexture@@QAEPAXI@Z (0x018BC3F0), and the body is byte for byte the scalar
//   deleting destructor VC7.1 emits for engine/CTexture.h (~CTexture() { Uninitialise(); } inline: call 0x009F9F70, `test byte [esp+8],1`,
//   operator delete, `mov eax,esi`); it is the only catalog function of that shape calling 0x009F9F70. Was the stand-in
//   ?VecDel@T@@QAEPAXI@Z binding 0x009F9F70 as ?Dtor_009f9f70@T@@QAEXXZ. No landed row binds 0x0041BB00 (the exe-scan hit in 0x00784260 is
//   not a relocation: its object binds nothing there).
// Shape: a compiler-generated COMDAT (CTexture is not polymorphic: VC7.1 emits ??_G for a delete of the class); the TU-internal
//   scaffolding function below makes this TU emit it as a pick-any COMDAT (the nf86 pattern).
#define FABLE_RETAIL_CTEXTURE
#include "fable_compiler.h"
#include "engine/CTexture.h"
namespace {
// Fidelity: scaffolding, not retail: the smallest delete of a CTexture, so this TU emits CTexture's scalar deleting destructor (a
// pick-any COMDAT). Internal to this TU.
void ReferenceTextureScalarDeletingDestructor(CTexture* texture)
{
    delete texture;
}
}
