// NUISystem::CScrollingViewport::`vector deleting destructor'(unsigned int) adjustor thunk -- this-0x18 (the CActionParam subobject at +0x18; `sub ecx,18h ; jmp ??_G`)
// Leaf: ??_ECScrollingViewport@NUISystem@@WBI@AEPAXI@Z
// 
// Retail 0x0053FC20, 8 bytes: the VC7.1 adjustor thunk the secondary vtable of CScrollingViewport uses for its
// slot 0 (`??_ECScrollingViewport@NUISystem@@WBI@AEPAXI@Z`); it moves `this` back to the full object and jumps to the deleting destructor ??_G 0x0053FC30
// (landed). VC7.1 names the slot-0 entry of a class with no array delete `??_E` and emits the thunk in the TU that
// defines the virtual destructor (precedent: CWorldMap 0x0051CDF0), so this source is the destructor's TU:
// ~CScrollingViewport (0x0053E950) defined here makes the compiler emit ??_G and both thunks.
// Callees: ??_GCScrollingViewport@NUISystem@@UAEPAXI@Z 0x0053FC30.
// Header: engine/NUISystem_CScrollingViewport.h (new).
#include "engine/NUISystem_CScrollingViewport.h"
#include "engine/NUISystem_CManager.h"
NUISystem::CScrollingViewport::~CScrollingViewport()
{
    CManager::GetInstance()->RemoveObserver(this);
    if (GetPViewportListeningToMouse() == this)
        SetPViewportListeningToMouse(0);
}
