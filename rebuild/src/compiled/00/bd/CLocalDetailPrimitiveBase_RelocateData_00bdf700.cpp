// CLocalDetailPrimitiveBase::RelocateData(long, void*, void*, long) -- does nothing
// Leaf: ?RelocateData@CLocalDetailPrimitiveBase@@UAEXJPAX0J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BDF700, thiscall, `ret 16`.
// Identity: slot 1 of CLocalDetailPrimitiveMesh's vtable 0x012A7E04 (defined here: the slot holds this body in 2 vtable(s), all of CLocalDetailPrimitiveBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 5 TLC-named slots of that vtable puts RelocateData(long, void *, void *, long) at this slot; Ego_r/FableWin name set: ?RelocateData@CLocalDetailPrimitiveBase@@UAEXJPAX0J@Z; `ret` matches the signature.
class CLocalDetailPrimitiveBase {
public:
    virtual void RelocateData(long, void*, void*, long);
};
void CLocalDetailPrimitiveBase::RelocateData(long, void*, void*, long)
{
}
