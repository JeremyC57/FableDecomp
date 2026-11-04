#include "fable_render_window.h"
// Leaf: ?SetAWindow@CRenderManagerCoreWindowView@@QAEXABVC2DBoxF@@@Z

void CRenderManagerCoreWindowView::SetAWindow(
    const C2DBoxF& window)
{
    displayManager3A3C->SetViewport(window);
}
