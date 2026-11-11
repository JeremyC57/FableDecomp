// CLocalDetailPrimitiveMeshZSpriteBatch::GetType() const -- returns (LOCAL_DETAIL_PRIMITIVE_TYPE)2
// Leaf: ?GetType@CLocalDetailPrimitiveMeshZSpriteBatch@@UBE?AW4LOCAL_DETAIL_PRIMITIVE_TYPE@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BFBBB0, thiscall, `ret`.
// Identity: slot 9 of CLocalDetailPrimitiveMeshZSpriteBatch's vtable 0x012A82D8 (defined here: the slot holds this body in 1 vtable(s), all of CLocalDetailPrimitiveMeshZSpriteBatch or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CLocalDetailPrimitiveMeshZSpriteBatch@@UBE?AW4LOCAL_DETAIL_PRIMITIVE_TYPE@@XZ; `ret` matches the signature.
enum LOCAL_DETAIL_PRIMITIVE_TYPE { };
class CLocalDetailPrimitiveMeshZSpriteBatch {
public:
    virtual LOCAL_DETAIL_PRIMITIVE_TYPE GetType() const;
};
LOCAL_DETAIL_PRIMITIVE_TYPE CLocalDetailPrimitiveMeshZSpriteBatch::GetType() const
{
    return (LOCAL_DETAIL_PRIMITIVE_TYPE)2;
}
