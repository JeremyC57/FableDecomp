#pragma optimize("s", on)
// CDisplayViewManager::SetView(CViewBase*) -- add a reference to the new view (virtual slot 1), then reseat the current-view counted pointer on it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00640050, length 28 bytes.
// Callees: the view's virtual slot 1 (AddRef), CCountedPointer<CViewBase>::Reset(CViewBase*) @ 0x006400B0 (on `this`: the manager IS the counted
// pointer, engine/CDisplayViewManager.h = { PCurrentView_Data +0, PCurrentView_Info +4 }). Shape: the view stays in esi for both uses, `this` in edi.
struct CViewBase {
    virtual void v00();
    virtual void AddRef();                         // slot 1
};

struct CDisplayViewManager {                       // engine/CDisplayViewManager.h: CCountedPointer<NDisplayView::CViewBase>
    CViewBase* PCurrentView_Data;                  // +0x00
    void*      PCurrentView_Info;                  // +0x04
    void ResetCurrentView(CViewBase* view);        // 0x006400B0 (CCountedPointer::Reset)
    void SetView(CViewBase* view);
};

void CDisplayViewManager::SetView(CViewBase* view)
{
    view->AddRef();
    ResetCurrentView(view);
}
