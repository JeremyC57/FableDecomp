// CLocalDetailPrimitiveMesh::GetType() const -- returns (LOCAL_DETAIL_PRIMITIVE_TYPE)0
// Leaf: ?GetType@CLocalDetailPrimitiveMesh@@UBE?AW4LOCAL_DETAIL_PRIMITIVE_TYPE@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BDFBA0, thiscall, `ret`.
// Identity: slot 9 of CLocalDetailPrimitiveMesh's vtable 0x012A7E04 (defined here: the slot holds this body in 1 vtable(s), all of CLocalDetailPrimitiveMesh or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 5 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CLocalDetailPrimitiveMesh@@UBE?AW4LOCAL_DETAIL_PRIMITIVE_TYPE@@XZ; `ret` matches the signature.
enum LOCAL_DETAIL_PRIMITIVE_TYPE { };
class CLocalDetailPrimitiveMesh {
public:
    virtual LOCAL_DETAIL_PRIMITIVE_TYPE GetType() const;
};
LOCAL_DETAIL_PRIMITIVE_TYPE CLocalDetailPrimitiveMesh::GetType() const
{
    return (LOCAL_DETAIL_PRIMITIVE_TYPE)0;
}
