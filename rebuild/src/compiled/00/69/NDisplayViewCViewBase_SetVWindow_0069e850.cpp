// NDisplayView::CViewBase::SetVWindow(C2DBoxF const &) -- does nothing
// Leaf: ?SetVWindow@CViewBase@NDisplayView@@UAEXABVC2DBoxF@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0069E850, thiscall, `ret 4`.
// Identity: slot 2 of NDisplayView::CViewBase's vtable 0x0125C7CC (defined here: the slot holds this body in 1 vtable(s), all of NDisplayView::CViewBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts SetVWindow(C2DBoxF const &) at this slot; Ego_r/FableWin name set: ?SetVWindow@CViewBase@NDisplayView@@UAEXABVC2DBoxF@@@Z; `ret` matches the signature.
class C2DBoxF;
namespace NDisplayView {
class CViewBase {
public:
    virtual void SetVWindow(C2DBoxF const &);
};
void CViewBase::SetVWindow(C2DBoxF const &)
{
}
}
