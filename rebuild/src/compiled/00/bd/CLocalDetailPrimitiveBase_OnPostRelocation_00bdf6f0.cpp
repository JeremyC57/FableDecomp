// CLocalDetailPrimitiveBase::OnPostRelocation() -- does nothing
// Leaf: ?OnPostRelocation@CLocalDetailPrimitiveBase@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BDF6F0, thiscall, `ret`.
// Identity: slot 7 of CLocalDetailPrimitiveMesh's vtable 0x012A7E04 (defined here: the slot holds this body in 2 vtable(s), all of CLocalDetailPrimitiveBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 5 TLC-named slots of that vtable puts OnPostRelocation(void) at this slot; Ego_r/FableWin name set: ?OnPostRelocation@CLocalDetailPrimitiveBase@@UAEXXZ; `ret` matches the signature.
class CLocalDetailPrimitiveBase {
public:
    virtual void OnPostRelocation();
};
void CLocalDetailPrimitiveBase::OnPostRelocation()
{
}
