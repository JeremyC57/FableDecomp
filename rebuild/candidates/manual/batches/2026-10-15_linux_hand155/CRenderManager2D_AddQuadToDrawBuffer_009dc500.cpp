// CRenderManager2D::AddQuadToDrawBuffer(const CTVertexRHWColSpecTex1& v0, v1, v2, v3, unsigned long blendMode, CTexture* texture) -- split a 2D quad into the triangles (v0,v1,v2) and (v0,v2,v3): opaque triangles for blend mode 1, alpha triangles otherwise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009DC500, length 119 bytes (`ret 18h`).
// Callees: CRenderManager2D::AddTri @ 0x009DB810, CRenderManager2D::AddAlphaTri @ 0x009DBD10 (both
// (v0, v1, v2, long mode, CTexture*)).
// Shape: `if (blendMode == 1)` passes the literal 1; the first call of each arm reuses the incoming ecx (`this`
// untouched since `mov esi,ecx`), the second reloads it from esi.
// Leaf: AddQuadToDrawBuffer
struct CTVertexRHWColSpecTex1;
struct CTexture;

struct CRenderManager2D {
    void AddTri(const CTVertexRHWColSpecTex1& v0, const CTVertexRHWColSpecTex1& v1, const CTVertexRHWColSpecTex1& v2,
                long mode, CTexture* texture);                               // 0x009DB810
    void AddAlphaTri(const CTVertexRHWColSpecTex1& v0, const CTVertexRHWColSpecTex1& v1, const CTVertexRHWColSpecTex1& v2,
                     long mode, CTexture* texture);                          // 0x009DBD10
    void AddQuadToDrawBuffer(const CTVertexRHWColSpecTex1& v0, const CTVertexRHWColSpecTex1& v1,
                             const CTVertexRHWColSpecTex1& v2, const CTVertexRHWColSpecTex1& v3,
                             unsigned long blendMode, CTexture* texture);
};

void CRenderManager2D::AddQuadToDrawBuffer(const CTVertexRHWColSpecTex1& v0, const CTVertexRHWColSpecTex1& v1,
                                           const CTVertexRHWColSpecTex1& v2, const CTVertexRHWColSpecTex1& v3,
                                           unsigned long blendMode, CTexture* texture)
{
    if (blendMode == 1) {
        AddTri(v0, v1, v2, 1, texture);
        AddTri(v0, v2, v3, 1, texture);
    } else {
        AddAlphaTri(v0, v1, v2, blendMode, texture);
        AddAlphaTri(v0, v2, v3, blendMode, texture);
    }
}
