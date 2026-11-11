// NDisplayView::CViewBase::Render(CInterpolationInfo const &, bool) -- does nothing
// Leaf: ?Render@CViewBase@NDisplayView@@UAEXABVCInterpolationInfo@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0069E860, thiscall, `ret 8`.
// Identity: slot 3 of NDisplayView::CViewBase's vtable 0x0125C7CC (defined here: the slot holds this body in 1 vtable(s), all of NDisplayView::CViewBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts Render(CInterpolationInfo const &, bool) at this slot; Ego_r/FableWin name set: ?Render@CViewBase@NDisplayView@@UAEXABVCInterpolationInfo@@_N@Z; `ret` matches the signature.
class CInterpolationInfo;
namespace NDisplayView {
class CViewBase {
public:
    virtual void Render(CInterpolationInfo const &, bool);
};
void CViewBase::Render(CInterpolationInfo const &, bool)
{
}
}
