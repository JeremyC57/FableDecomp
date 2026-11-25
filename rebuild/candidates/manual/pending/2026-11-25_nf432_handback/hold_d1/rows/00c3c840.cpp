// CCodaObjectBase::`scalar deleting destructor'(unsigned int) -- the GameCODA root class's deleting destructor: restore the class's vtable,
//   free through the class operator delete (CodaFree) when bit 0 of the flag is set, return `this`
// Leaf: ??_GCCodaObjectBase@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3C840, 31 bytes: test byte ptr [esp+4],1 ; push esi ; mov esi,ecx ; mov [esi],0x012B5868 ; je ;
//   push esi ; call CodaFree 0x00C3C370 ; pop ecx ; mov eax,esi ; pop esi ; ret 4.
// Names (NF432, EXTRA_RULES D3): the class is CCodaObjectBase (vtable 0x012B5868, engine/CCodaObjectBase.h); this body is slot 0 of that
//   vtable (retail) and stores 0x012B5868 itself (the inline destructor's vptr restore). The table already binds ??_7CCodaObjectBase@@6B@
//   there (witness 0x00C3E2A0). The inline destructor (FABLE_RETAIL_CCODAOBJECTBASE_INLINE_DTOR, NF404: 0x00C3C830 is its pick-any copy)
//   and the inline operator delete forwarding to CodaFree are the header's; the compiler-generated deleting destructor is emitted with the
//   vtable in the TU that constructs the object.
// Was: a local model `struct CVtable_012b5868` emitting ??_GCVtable_012b5868@@UAEPAXI@Z / ??_7CVtable_012b5868@@6B@.
// MakeOne: Fidelity: scaffolding, not retail -- the construction that makes VC7.1 emit the vtable and its deleting destructor (as at HEAD).
#define FABLE_RETAIL_CCODAOBJECTBASE_INLINE_DTOR
#include "engine/CCodaObjectBase.h"
void* MakeOne()
{
    return new CCodaObjectBase;
}
