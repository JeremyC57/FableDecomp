// CEngineLayerRenderer::CEngineLayerRenderer(void* scene, void* layer, void* target) -- CBase, the scene at +4, own vtable, layer and target, three cleared words
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B29720, length 64 bytes.
// Callees: CBase::CBase @ 0x0099A2F0. Vtable 0x012A0F04 = CEngineLayerRenderer (rtti-vtables.tsv); the chunk
// label "CVertexShaderData::CVertexShaderData" is propagated over this constructor-shell shape.
// Shape: first argument stored at +4 before the vtable store (`__declspec(novtable)` argument base), then
// `xor eax,eax`, the vtable, the two remaining arguments at +8 / +0xc and the zero register stored at
// +0x10, +0x14, +0x18 by the inline constructor of an empty 12-byte vector member (plain init-list zeros
// are hoisted above the vtable store -- wave 24i); `ret 0Ch`.
struct CBase {
    virtual void v00();
    CBase();                                  // 0x0099A2F0
};

struct __declspec(novtable) CEngineLayerRendererBase : CBase {
    void* m_pScene;                           // +0x04
    CEngineLayerRendererBase(void* scene) : CBase(), m_pScene(scene) { }
};

struct CRenderItemVector {                    // 12-byte retail vector {first, last, end}; the inline
    void* m_pFirst;                           //   constructor's zero stores are scheduled AFTER the vtable
    void* m_pLast;                            //   store (plain init-list zeros would be hoisted above it)
    void* m_pEnd;
    CRenderItemVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
};

struct CEngineLayerRenderer : CEngineLayerRendererBase {      // vtable 0x012A0F04
    void*             m_pLayer;               // +0x08
    void*             m_pTarget;              // +0x0c
    CRenderItemVector m_items;                // +0x10
    virtual void v00();
    CEngineLayerRenderer(void* scene, void* layer, void* target);
};

CEngineLayerRenderer::CEngineLayerRenderer(void* scene, void* layer, void* target)
    : CEngineLayerRendererBase(scene), m_pLayer(layer), m_pTarget(target), m_items()
{
}