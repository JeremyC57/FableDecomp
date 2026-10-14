// Leaf: ??1CShape@NShape
// NShape::CShape::~CShape() -- this class's vtable, then the edge and point vectors at +0x14 / +0x08 freed (copy @ 00764F70 of three: the inline destructor is emitted in three translation units)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00764F70.
// Callees: free @ 0x00BFEA14 twice. Vtable 0x0126A208 = NShape::CShape (rtti-vtables.tsv; the landed CBasicShape constructor @ 0x00762440 has the layout). Shape: the
// vtable store, then the members destroyed in reverse order, each `test ; je ; push ; call ; add esp,4`; the second block pointer is loaded into esi. The chunk
// label "CEngineScreenEffectDisplacementRenderer::FinishWithDynamicTextures" is propagated.
extern "C" void __cdecl free(void* block);        // 0x00BFEA14

struct CRetailVector_pod { void* m_pFirst; void* m_pLast; void* m_pEnd; ~CRetailVector_pod() { if (m_pFirst) free(m_pFirst); } };   // VC6 vector of PODs: the destructor frees the block

namespace NShape {
struct CShape {                                    // vtable 0x0126A208
    virtual void v00();
    long             m_id;                         // +0x04
    CRetailVector_pod m_points;                    // +0x08
    CRetailVector_pod m_edges;                     // +0x14
    ~CShape() { }
};
}

// The destructor is inline in the class; this out-of-line use makes the translation unit emit it.
void Destroy_CShape_00764f70(NShape::CShape* shape)
{
    shape->~CShape();
}