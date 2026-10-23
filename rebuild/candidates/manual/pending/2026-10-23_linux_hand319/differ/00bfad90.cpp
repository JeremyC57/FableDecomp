// Grade: DIFFER(18v1151) -- first-pass stub only (one grade); the body is not reconstructed yet, see the header for the decoded shape
// CLocalDetailPrimitiveMeshZSpriteBatch::RenderUpdate(5 dword arguments, `ret 0x14`) -- clip the batch against the view
//   (ClipPrimitive), compute the fade alpha / distance with x87 math and add each visible static mesh to the engine
//   primitive renderer
// Leaf: RenderUpdate
// Stub: x87-heavy; first-pass skeleton only, not reconstructed yet.
class CLocalDetailPrimitiveMeshZSpriteBatch {
public:
    bool ClipPrimitive(long a, long b);                // 0x00BF9AA0 (CLocalDetailPrimitiveBase)
    void RenderUpdate(long a1, long a2, long a3, long a4, long a5);
};
void CLocalDetailPrimitiveMeshZSpriteBatch::RenderUpdate(long a1, long a2, long a3, long a4, long a5)
{
    if (!ClipPrimitive(a1, a2))
        return;
}
