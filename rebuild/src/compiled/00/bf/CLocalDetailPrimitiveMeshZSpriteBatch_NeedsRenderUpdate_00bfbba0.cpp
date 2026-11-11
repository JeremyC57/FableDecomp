// CLocalDetailPrimitiveMeshZSpriteBatch::NeedsRenderUpdate() -- returns true
// Leaf: ?NeedsRenderUpdate@CLocalDetailPrimitiveMeshZSpriteBatch@@UAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BFBBA0, thiscall, `ret`.
// Identity: slot 6 of CLocalDetailPrimitiveMeshZSpriteBatch's vtable 0x012A82D8 (defined here: the slot holds this body in 1 vtable(s), all of CLocalDetailPrimitiveMeshZSpriteBatch or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts NeedsRenderUpdate(void) at this slot; Ego_r/FableWin name set: ?NeedsRenderUpdate@CLocalDetailPrimitiveMeshZSpriteBatch@@UAE_NXZ; `ret` matches the signature.
class CLocalDetailPrimitiveMeshZSpriteBatch {
public:
    virtual bool NeedsRenderUpdate();
};
bool CLocalDetailPrimitiveMeshZSpriteBatch::NeedsRenderUpdate()
{
    return true;
}
