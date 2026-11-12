// NCameraModes::CTrack::OnDieLeaf() -- does nothing
// Leaf: ?OnDieLeaf@CTrack@NCameraModes@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0087B830, thiscall, `ret`.
// Identity: slot 25 of NCameraModes::CTrack's vtable 0x0125C50C (defined here: the slot holds this body in 1 vtable(s), all of NCameraModes::CTrack or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 8 TLC-named slots of that vtable puts OnDieLeaf(void) at this slot; Ego_r/FableWin name set: ?OnDieLeaf@CTrack@NCameraModes@@UAEXXZ; `ret` matches the signature.
namespace NCameraModes {
class CTrack {
public:
    virtual void OnDieLeaf();
};
void CTrack::OnDieLeaf()
{
}
}
