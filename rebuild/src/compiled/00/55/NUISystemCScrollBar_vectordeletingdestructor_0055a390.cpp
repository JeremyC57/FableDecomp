// NUISystem::CScrollBar::`vector deleting destructor'(unsigned int) adjustor thunk -- this-0x18 (the CActionParam subobject at +0x18; `sub ecx,18h ; jmp ??_G`)
// Leaf: ??_ECScrollBar@NUISystem@@WBI@AEPAXI@Z
// Flags: none
//
// Retail 0x0055A390, 8 bytes: the VC7.1 adjustor thunk in slot 0 of CScrollBar's CActionParam vtable 0x0124B7B4 (+0x18); it moves
// `this` back to the full object and jumps to the deleting destructor ??_G 0x0055A3A0 (slot 0 of the primary vtable 0x0124B7E4).
// VC7.1 names the slot-0 entry of a class with no array delete `??_E` and emits the thunks in the TU that defines the virtual
// destructor (precedent: CDraggableInto 0x00559020 / 0x00559030, CScrollingViewport 0x0053FC10 / 0x0053FC20), so this source
// is the destructor's TU: ~CScrollBar (0x00559A10, landed: delete the dragged-down, then the dragged-up action list) defined
// here makes the compiler emit ??_G and both thunks.
// Callees: ??_GCScrollBar@NUISystem@@UAEPAXI@Z 0x0055A3A0.
// Header: engine/NUISystem_CScrollBar.h (new, chunk 726 model).
#include "engine/NUISystem_CScrollBar.h"
namespace NUISystem {
CScrollBar::~CScrollBar()
{
    delete PActionsOnDraggedDown;
    delete PActionsOnDraggedUp;
}
}