// NUISystem::CMesh::`vector deleting destructor'(unsigned int) adjustor thunk -- this-0x18 (the CActionParam subobject at +0x18; `sub ecx,18h ; jmp ??_G`)
// Leaf: ??_ECMesh@NUISystem@@WBI@AEPAXI@Z
// Flags: none
//
// Retail 0x00550C20, 8 bytes: the VC7.1 adjustor thunk in slot 0 of CMesh's CActionParam vtable 0x01249F6C (+0x18); it moves
// `this` back to the full object and jumps to the deleting destructor ??_G 0x00550C30. VC7.1 names the slot-0 entry of a
// class with no array delete `??_E` and emits the thunk in the TU that defines the virtual destructor (precedent:
// CScrollingViewport 0x0053FC10 / 0x0053FC20), so this source is the destructor's TU: ~CMesh (0x00550230, landed: the
// user body is `Handle.Clear();`) defined here makes the compiler emit ??_G and both thunks.
// Callees: ??_GCMesh@NUISystem@@UAEPAXI@Z 0x00550C30.
// Header: engine/NUISystem_CMesh.h (new).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#include "engine/NUISystem_CMesh.h"
namespace NUISystem {
CMesh::~CMesh()
{
    Handle.Clear();
}
}