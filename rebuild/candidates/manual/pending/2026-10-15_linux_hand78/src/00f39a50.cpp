// CSurfaceCompositor::CompositeQuad(CTexture* texture, const SCompositeVertex* vertices, ECompositeBlendType blend) -- draw one textured quad into the composition: set the blend mode, hand the render manager core a one-entry texture array, the four 24-byte vertices and the shared two-triangle index list, and count the quad
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F39A50, length 79 bytes.
// Callees: GFGetSystemManager @ 0x009A4EC0, CSurfaceCompositor::SetBlendMode @ 0x00F38A10,
// CRenderManagerCore::DrawAsIndexedTris @ 0x00A0AEA0 (textures, count, vertices, stride, vertex count, indices,
// triangle count); the quad index list @ 0x013AC928.
// Shape: `push ecx` = the one-slot texture array local, filled from the argument after the core fetch and
// before the SetBlendMode call; the seven pushes right to left with `lea ecx,[esp+20h]` for the array, the
// counter bumped as load/inc/store, `ret 0Ch`.
// Model: PDB CSystemManager (+0x60 PDisplayManager) / CDisplayManager (+8 PRenderManager); the quad counter at
// +0x28 is descriptive.
struct CTexture;

struct C2DBoxF {
    float left, top, right, bottom;
};

struct CRenderManagerCore {
    void SetAWindow(const C2DBoxF& window);        // 0x00A0AA80
    void DrawAsIndexedTris(CTexture** textures, unsigned long textureCount, const void* vertices,
        long vertexStride, long vertexCount, const void* indices, long triangleCount);   // 0x00A0AEA0
};

struct CDisplayManager {                           // PDB: +4 SystemManager, +8 PRenderManager (scoped_ptr)
    void*               __vftable;                 // +0x00
    void*               SystemManager;             // +0x04
    CRenderManagerCore* PRenderManager;            // +0x08
    bool SetRenderTargetAsBackScreen();            // 0x009BF220
    void EndRender();                              // 0x009BEF50
};

struct CSystemManager {                            // PDB: +0x60 CCountedPointer<CDisplayManager> PDisplayManager
    unsigned char    _pad00[0x60];
    CDisplayManager* PDisplayManager;              // +0x60 (the counted pointer's object word)
    void*            PDisplayManagerCount;         // +0x64
};
CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0

struct CRenderTarget {
    void DetachTarget();                           // 0x00A10E10
};

struct CRenderStateManager {
    void RestoreCaptureBlock();                    // 0x00A05840
};
extern CRenderStateManager* g_pRenderStateManager; // 0x013CB508
extern const unsigned short g_QuadIndices[6];     // 0x013AC928 (two triangles over four vertices)

enum ECompositeBlendType { COMPOSITE_BLEND_DEFAULT };

struct SCompositeVertex {                          // 24 bytes per vertex
    float x, y, z, rhw;
    float u, v;
};

struct CSurfaceCompositor {
    unsigned char  _pad00[0x28];
    long           m_quadsComposited;              // +0x28
    void SetBlendMode(ECompositeBlendType blend);  // 0x00F38A10
    void CompositeQuad(CTexture* texture, const SCompositeVertex* vertices, ECompositeBlendType blend);
};

void CSurfaceCompositor::CompositeQuad(CTexture* texture, const SCompositeVertex* vertices, ECompositeBlendType blend)
{
    CRenderManagerCore* core = GFGetSystemManager()->PDisplayManager->PRenderManager;
    CTexture* textures[1];
    textures[0] = texture;
    SetBlendMode(blend);
    core->DrawAsIndexedTris(textures, 1, vertices, sizeof(SCompositeVertex), 4, g_QuadIndices, 2);
    ++m_quadsComposited;
}
