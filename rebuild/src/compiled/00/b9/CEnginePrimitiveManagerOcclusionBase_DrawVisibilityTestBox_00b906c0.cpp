// CEnginePrimitiveManagerOcclusionBase::DrawVisibilityTestBox(void* box, void* colour) -- bracket an untextured render of the occlusion data's test mesh (+0x94 of the object at +0x14) between the global renderer's begin and end calls
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B906C0, length 54 bytes.
// Callees: the renderer's BeginUntextured @ 0x00B8D090 and EndUntextured(colour) @ 0x00B8D0A0 (both empty in retail; on the global @ 0x01436E5C), RenderUntexturedStaticMeshPrimitive
// @ 0x00B903D0 (on `this`: the mesh, the box). Shape: `this` stays in esi, the global is re-read for the end call, the arguments come straight from the stack.
struct CEngineRenderer_00b906c0 {
    void BeginUntextured();                        // 0x00B8D090
    void EndUntextured(void* colour);              // 0x00B8D0A0
};
extern CEngineRenderer_00b906c0* g_pRenderer_01436e5c;   // 0x01436E5C

struct COcclusionData_00b906c0 {
    unsigned char _pad[0x94];
    void*         m_pTestMesh;                     // +0x94
};

struct CEnginePrimitiveManagerOcclusionBase {
    unsigned char           _pad[0x14];
    COcclusionData_00b906c0* m_pOcclusionData;     // +0x14
    void RenderUntexturedStaticMeshPrimitive(void* mesh, void* box);   // 0x00B903D0
    void DrawVisibilityTestBox(void* box, void* colour);
};

void CEnginePrimitiveManagerOcclusionBase::DrawVisibilityTestBox(void* box, void* colour)
{
    g_pRenderer_01436e5c->BeginUntextured();
    RenderUntexturedStaticMeshPrimitive(m_pOcclusionData->m_pTestMesh, box);
    g_pRenderer_01436e5c->EndUntextured(colour);
}