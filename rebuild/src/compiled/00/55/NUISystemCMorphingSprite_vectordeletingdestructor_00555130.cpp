// NUISystem::CMorphingSprite::`vector deleting destructor'(unsigned int) adjustor thunk -- this-0x18 (the CActionParam subobject at +0x18; `sub ecx,18h ; jmp ??_G`)
// Leaf: ??_ECMorphingSprite@NUISystem@@WBI@AEPAXI@Z
// Flags: none
//
// Retail 0x00555130, 8 bytes: the VC7.1 adjustor thunk in slot 0 of CMorphingSprite's CActionParam vtable 0x0124A46C (+0x18); it moves
// `this` back to the full object and jumps to the deleting destructor ??_G 0x00555140 (slot 0 of the primary vtable 0x0124A49C).
// VC7.1 names the slot-0 entry of a class with no array delete `??_E` and emits the thunks in the TU that defines the virtual
// destructor (precedent: CDraggableInto 0x00559020 / 0x00559030, CScrollingViewport 0x0053FC10 / 0x0053FC20), so this source
// is the destructor's TU: ~CMorphingSprite (0x005541E0, landed: delete every handle, clear the list, then the member dtors)
// defined here makes the compiler emit ??_G and both thunks.
// Callees: ??_GCMorphingSprite@NUISystem@@UAEPAXI@Z 0x00555140.
// Header: engine/NUISystem_CMorphingSprite.h (new, chunk 752).
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/NUISystem_CMorphingSprite.h"
namespace NUISystem {
CMorphingSprite::~CMorphingSprite()
{
    for (std::list<CEnginePrimitiveHandle*>::iterator it = Handles.begin(); it != Handles.end(); ++it)
        delete *it;
    Handles.clear();
}
}