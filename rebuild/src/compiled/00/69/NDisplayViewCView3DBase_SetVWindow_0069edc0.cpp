// NDisplayView::CView3DBase::SetVWindow(const C2DBoxF& window) -- vtable slot 2: stores the view's screen window box
// Leaf: ?SetVWindow@CView3DBase@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069EDC0, 32 bytes.
// Name: FableWin.pdb ?SetVWindow@CView3DBase@NDisplayView@@UAEXABVC2DBoxF@@@Z; slot 2 of the vtable 0x0125C7E0
// (rtti-vtables.tsv; slot 3 is the landed Render 0x0069E980).
// Callees: none.
// Layout: Ego_r.pdb CView3DBase (156 bytes) = { vptr, CIEngine* PEngine +4, C2DBoxF Window +8,
// CCameraMovementInfo CameraMovementInfo +0x18 }; C2DBoxF is four floats (Left, Top, Right, Bottom), as the
// landed Render model.
// Shape: the implicit copy of the 16-byte POD box: `add ecx,8` forms &Window once, then four dword moves
// through edx (the last through eax). `ret 4`.
class C2DBoxF { public: float Left, Top, Right, Bottom; };
class CIEngine;

namespace NDisplayView {

struct CView3DBase {
    virtual void v00();
    CIEngine* PEngine;   // +0x04
    C2DBoxF   Window;    // +0x08
    void SetVWindow(const C2DBoxF& window);
};

void CView3DBase::SetVWindow(const C2DBoxF& window)
{
    Window = window;
}

} // namespace NDisplayView