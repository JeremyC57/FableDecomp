// NUISystem::CScrollingViewport::~CScrollingViewport() -- unregisters the viewport from the UI manager and clears the mouse-listener global
// Leaf: ??1CScrollingViewport@NUISystem@@UAE@XZ
// 
// Retail 0x0053E950, 66 bytes (vtable 0x012468D4 slot 0 reaches it through ??_G 0x0053FC30). Stores the three
// CScrollingViewport vtables (primary 0x012468D4, CObserver base +4 0x012468AC, CActionParam base +0x18 0x012468A4),
// calls CManager::GetInstance 0x0041E5F2 and its RemoveObserver (vtable slot 5, +0x14) with the CObserver subobject
// (`lea edi,[esi+4]`, no null test: `this` is never null in a destructor), clears the global
// g_pActiveScrollingViewport 0x013B8AAC when it points at this viewport (the inline static
// Get/SetPViewportListeningToMouse accessors; FableWin.pdb has both), then tail-jumps to the base destructor
// ~CViewport 0x0054EAC0.
// Shape: the vtable stores are the compiler's; the user body is the RemoveObserver call and the compare-and-clear.
// Header: engine/NUISystem_CScrollingViewport.h (new, PDB layout with the retail offsets: CViewport 0x174, this 0x19C).
#include "engine/NUISystem_CScrollingViewport.h"
#include "engine/NUISystem_CManager.h"
NUISystem::CScrollingViewport::~CScrollingViewport()
{
    CManager::GetInstance()->RemoveObserver(this);
    if (GetPViewportListeningToMouse() == this)
        SetPViewportListeningToMouse(0);
}