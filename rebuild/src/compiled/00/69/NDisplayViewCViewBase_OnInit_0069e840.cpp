// NDisplayView::CViewBase::OnInit() -- does nothing
// Leaf: ?OnInit@CViewBase@NDisplayView@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0069E840, thiscall, `ret`.
// Identity: slot 1 of NDisplayView::CView3DThirdPerson's vtable 0x0125C7F8 (defined here: the slot holds this body in 3 vtable(s), all of NDisplayView::CViewBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 2 TLC-named slots of that vtable puts OnInit(void) at this slot; Ego_r/FableWin name set: ?OnInit@CViewBase@NDisplayView@@UAEXXZ; `ret` matches the signature.
namespace NDisplayView {
class CViewBase {
public:
    virtual void OnInit();
};
void CViewBase::OnInit()
{
}
}
