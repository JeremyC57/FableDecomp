// CEnginePrimitiveManagerOcclusionBase::DrawVisibilityTestBox(void* box, void* colour) -- bracket an untextured render of the occlusion data's test mesh (+0x94 of the object at +0x14) between the global renderer's begin and end calls
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B906C0, length 54 bytes.
// Callees: the renderer's BeginUntextured @ 0x00B8D090 and EndUntextured(colour) @ 0x00B8D0A0 (both empty in retail; on the global @ 0x01436E5C), RenderUntexturedStaticMeshPrimitive
// @ 0x00B903D0 (on `this`: the mesh, the box). Shape: `this` stays in esi, the global is re-read for the end call, the arguments come straight from the stack.
// NF37: 0x00B8D090 / 0x00B8D0A0 are CEngineOcclusionManager::StartOcclusionTest / EndOcclusionTest (Ego_r.pdb 0x00A347A0 / 0x00A347B0,
// the empty PC bodies) on NEngineInternal::GPOcclusionManager 0x01436E5C (the landed spelling of 0x00B8D420 and the cloth renderers).
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_OCCLUSIONTEST
#include "engine/CEngineOcclusionManager.h"
namespace NEngineInternal { extern CEngineOcclusionManager* GPOcclusionManager; }   // 0x01436E5C

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
    NEngineInternal::GPOcclusionManager->StartOcclusionTest();
    RenderUntexturedStaticMeshPrimitive(m_pOcclusionData->m_pTestMesh, box);
    NEngineInternal::GPOcclusionManager->EndOcclusionTest((unsigned long)colour);
}