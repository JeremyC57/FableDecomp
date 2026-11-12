// CTCWallMountEffects::OnInitialActivate() -- does nothing
// Leaf: ?OnInitialActivate@CTCWallMountEffects@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007DF3A0, thiscall, `ret`.
// Identity: slot 6 of CTCWallMountEffects's vtable 0x0126FC84 (defined here: the slot holds this body in 1 vtable(s), all of CTCWallMountEffects or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts OnInitialActivate(void) at this slot; Ego_r/FableWin name set: ?OnInitialActivate@CTCWallMountEffects@@UAEXXZ; `ret` matches the signature.
class CTCWallMountEffects {
public:
    virtual void OnInitialActivate();
};
void CTCWallMountEffects::OnInitialActivate()
{
}
