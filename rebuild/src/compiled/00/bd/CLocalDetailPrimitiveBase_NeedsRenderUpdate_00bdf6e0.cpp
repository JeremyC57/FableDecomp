// CLocalDetailPrimitiveBase::NeedsRenderUpdate() -- returns false
// Leaf: ?NeedsRenderUpdate@CLocalDetailPrimitiveBase@@UAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BDF6E0, thiscall, `ret`.
// Identity: slot 6 of CLocalDetailPrimitiveRepeatedMesh's vtable 0x012A82AC (defined here: the slot holds this body in 3 vtable(s), all of CLocalDetailPrimitiveBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts NeedsRenderUpdate(void) at this slot; Ego_r/FableWin name set: ?NeedsRenderUpdate@CLocalDetailPrimitiveBase@@UAE_NXZ; `ret` matches the signature.
class CLocalDetailPrimitiveBase {
public:
    virtual bool NeedsRenderUpdate();
};
bool CLocalDetailPrimitiveBase::NeedsRenderUpdate()
{
    return false;
}
