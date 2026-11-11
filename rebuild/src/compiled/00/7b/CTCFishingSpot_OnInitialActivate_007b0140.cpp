// CTCFishingSpot::OnInitialActivate() -- does nothing
// Leaf: ?OnInitialActivate@CTCFishingSpot@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B0140, thiscall, `ret`.
// Identity: slot 6 of CTCFishingSpot's vtable 0x0126D51C (defined here: the slot holds this body in 1 vtable(s), all of CTCFishingSpot or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts OnInitialActivate(void) at this slot; Ego_r/FableWin name set: ?OnInitialActivate@CTCFishingSpot@@UAEXXZ; `ret` matches the signature.
class CTCFishingSpot {
public:
    virtual void OnInitialActivate();
};
void CTCFishingSpot::OnInitialActivate()
{
}
